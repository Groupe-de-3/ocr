#include "blood_filling.h"

#include <stdbool.h>
#include <stdio.h>
#include <limits.h>

#include "vec.h"

typedef struct {
    int x, y;
} PixelPos;

typedef struct {
    int       sx, sy;
    int       ex, ey;
    long total_weight;
    PixelPos *pixels;
} Blob;

static long get_blob_size(Blob blob) {
    return blob.total_weight;
}

static Blob find_blob_at(ImageView *img, int start_x, int start_y, blood_fill_weighter weighter) {
    const any_pixel_t black = ANY_BLACK(img->image->format);
    // Stack of unvisited pixels
    PixelPos *poses = vec_new(PixelPos);
    imgv_set_pixel_any(img, start_x, start_y, black);
    *vec_push(&poses, PixelPos) = (PixelPos){.x = start_x, .y = start_y};

    Blob extent = {
        .sx     = start_x,
        .sy     = start_y,
        .ex     = start_x,
        .ey     = start_y,
        .pixels = vec_new(PixelPos),
    };
    *vec_push(&extent.pixels, PixelPos) =
        (PixelPos){.x = start_x, .y = start_y};
    extent.total_weight = weighter(img, start_x, start_y);

    while (vec_size(poses) > 0) {
        PixelPos pp = vec_pop(poses, PixelPos);
        int      x = pp.x, y = pp.y;

        for (int dx = x - 1; dx <= x + 1; dx++) {
            for (int dy = y - 1; dy <= y + 1; dy++) {
                if (imgv_get_pixel_grayscale(img, dx, dy) > 0.5) {
                    imgv_set_pixel_any(img, dx, dy, black);
                    PixelPos dpp                = (PixelPos){.x = dx, .y = dy};
                    *vec_push(&poses, PixelPos) = dpp;
                    *vec_push(&extent.pixels, PixelPos) = dpp;
                    extent.total_weight += weighter(img, dx, dy);

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

long blood_fill_largest_weighted_blob(ImageView *img, blood_fill_weighter weighter, bool center) {
    Blob biggest_extent      = {.pixels = NULL};
    long biggest_extent_size = LONG_MIN;

    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            if (imgv_get_pixel_grayscale(img, x, y) > 0.5) {
                Blob ex = find_blob_at(img, x, y, weighter);
                if (get_blob_size(ex) > biggest_extent_size) {
                    vec_destroy(biggest_extent.pixels);
                    biggest_extent      = ex;
                    biggest_extent_size = get_blob_size(ex);
                }
                else {
                    vec_destroy(ex.pixels);
                }
            }
        }
    }
    
    if (biggest_extent.pixels == NULL)
        return 0;
    
    int delta_x = 0,
        delta_y = 0;
    
    if (center) {
        delta_x = -biggest_extent.sx;
        delta_y = -biggest_extent.sy;

        delta_x += img->height / 2;
        delta_y += img->width  / 2;
        
        delta_x -= (biggest_extent.ex - biggest_extent.sx) / 2;
        delta_y -= (biggest_extent.ey - biggest_extent.sy) / 2;
    }

    const any_pixel_t white = ANY_WHITE(img->image->format);
    for (size_t i = 0; i < vec_size(biggest_extent.pixels); i++) {
        int x = biggest_extent.pixels[i].x + delta_x;
        int y = biggest_extent.pixels[i].y + delta_y;
        if (!imgv_in_bound(img, x, y))
            continue;
        imgv_set_pixel_any(
            img, x, y, white
        );
    }
    vec_destroy(biggest_extent.pixels);
    
    return biggest_extent_size;
}

static long id() {
    return 1;
} 

long blood_fill_largest_blob(ImageView *img, bool center) {
    return blood_fill_largest_weighted_blob(img, id, center);
}

long blood_fill_center_weighter(ImageView *imgv, int x, int y) {
    double dx = (x - (double)imgv->width/2)*(x - (double)imgv->width/2);
    double dy = (y - (double)imgv->height/2)*(y - (double)imgv->height/2);
    return (long)sqrt(imgv->width*imgv->width + imgv->height*imgv->height) - (long)sqrt(dx*dx + dy*dy);
}
