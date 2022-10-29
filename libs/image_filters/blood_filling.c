#include "blood_filling.h"

#include <stdbool.h>
#include <stdio.h>

#include "vec.h"

typedef struct {
    int x, y;
} PixelPos;

typedef struct {
    int sx, sy;
    int ex, ey;
    PixelPos *pixels;
} Blob;

static long get_blob_size(Blob blob) {
    return (long)vec_size(blob.pixels);
    int width = blob.ex - blob.sx + 1; // + 1 Because ex is exclusive
    int height = blob.ey - blob.sy + 1; // + 1 Because ex is exclusive
    return (long)width * (long)height;
}

static Blob find_blob_at(ImageView *img, int start_x, int start_y) {
    const any_pixel_t black = ANY_BLACK(img->image->format);
    // Stack of unvisited pixels
    PixelPos *poses = vec_new(PixelPos);
    imgv_set_pixel_any(img, start_x, start_y, black);
    *vec_push(&poses, PixelPos) = (PixelPos){ .x = start_x, .y = start_y };
    
    Blob extent = {
        .sx = start_x,
        .sy = start_y,
        .ex = start_x,
        .ey = start_y,
        .pixels = vec_new(PixelPos),
    };
    *vec_push(&extent.pixels, PixelPos) = (PixelPos){ .x = start_x, .y = start_y };
    
    while (vec_size(poses) > 0) {
        PixelPos pp = vec_pop(poses, PixelPos);
        int x = pp.x, y = pp.y;
        
        for (int dx = x-1; dx <= x+1; dx++) {
            for (int dy = y-1; dy <= y+1; dy++) {
                if (imgv_get_pixel_grayscale(img, dx, dy) > 0.5) {
                    imgv_set_pixel_any(img, dx, dy, black);
                    PixelPos dpp = (PixelPos){ .x = dx, .y = dy };
                    *vec_push(&poses, PixelPos) = dpp;
                    *vec_push(&extent.pixels, PixelPos) = dpp;

                    // Update the extent if the pixel isn't in the current one
                    if (dx < extent.sx)
                        extent.sx = dx;
                    else if (dx > extent.ex)
                        extent.ex = dx;

                    if (dy < extent.sy)
                        extent.sy = dy;
                    else if (dy > extent.ey)
                        extent.ey = dy;
                }
            }
        }
    }
    
    vec_destroy(poses);
    
    return extent;
}

void blood_fill_largest_blob(ImageView *img) {
    Blob biggest_extent = { .pixels = NULL };
    long biggest_extent_size = 0;

    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            if (imgv_get_pixel_grayscale(img, x, y) > 0.5) {
                Blob ex = find_blob_at(img, x, y);
                if (get_blob_size(ex) > biggest_extent_size) {
                    vec_destroy(biggest_extent.pixels);
                    biggest_extent = ex;
                    biggest_extent_size = get_blob_size(ex);
                }
                else {
                    vec_destroy(ex.pixels);
                }
            }
        }
    }
    
    const any_pixel_t white = ANY_WHITE(img->image->format);
    for (size_t i = 0; i < vec_size(biggest_extent.pixels); i++) {
        imgv_set_pixel_any(img, biggest_extent.pixels[i].x, biggest_extent.pixels[i].y, white);
    }
    vec_destroy(biggest_extent.pixels);
}
