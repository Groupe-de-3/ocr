#pragma once
#include <stdint.h>
#include <stdlib.h>

enum ImageType {
    ImageType_GrayScale,
    ImageType_Rgb8,
};
typedef float grayscale_pixel_t;
typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} rgb8_pixel_t;

typedef struct {
    enum ImageType type;
    union {
        grayscale_pixel_t *grayscale;
        rgb8_pixel_t      *rgb8;
    } data;

    size_t width;
    size_t height;
} Image;

#define GRAYSCALE_BLACK 0.f
#define GRAYSCALE_WHITE 1.f
#define RGB8_BLACK             \
    (rgb8_pixel_t) {           \
        .r = 0, .g = 0, .b = 0 \
    }
#define RGB8_WHITE                   \
    (rgb8_pixel_t) {                 \
        .r = 255, .g = 255, .b = 255 \
    }

Image img_new(size_t width, size_t height, enum ImageType type);
void  img_destroy(Image image);

_Bool img_in_bound(Image *image, size_t x, size_t y);

typedef struct {
    _Bool success;
} ImageLoadResult;
ImageLoadResult img_load_file(char *filename, Image *image_out);

grayscale_pixel_t img_rbg8_to_grayscale(uint8_t r, uint8_t g, uint8_t b);
uint8_t           img_grayscale_to_rgb8(grayscale_pixel_t value);

grayscale_pixel_t img_get_pixel_grayscale(Image *image, size_t x, size_t y);
void              img_set_pixel_grayscale(
                 Image *image, size_t x, size_t y, grayscale_pixel_t new_value
             );

rgb8_pixel_t img_get_pixel_rgb8(Image *image, size_t x, size_t y);
void         img_set_pixel_rgb8(
            Image *image, size_t x, size_t y, rgb8_pixel_t new_value
        );
