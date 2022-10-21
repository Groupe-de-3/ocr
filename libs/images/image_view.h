#pragma once
#include "images.h"

enum ImgvWrapingMode {
    WrappingMode_None,
    WrappingMode_Clamp,
    WrappingMode_Repeat,
    WrappingMode_White,
    WrappingMode_Black,
};

typedef struct {
    Image               *image;
    enum ImgvWrapingMode wraping_mode;
    _Bool                ignore_invalid_set;
    int                  x, y;
    int                  width, height;
} ImageView;

_Bool imgv_in_bound(ImageView *imagev, int x, int y);

ImageView imgv_default(Image *image);

any_pixel_t imgv_get_pixel_any(ImageView *imagev, int x, int y);
void imgv_set_pixel_any(ImageView *image, int x, int y, any_pixel_t new_value);

some_pixel_t imgv_get_pixel_some(ImageView *imagev, int x, int y);
void         imgv_set_pixel_some(
            ImageView *image, int x, int y, some_pixel_t new_value
        );

grayscale_pixel_t imgv_get_pixel_grayscale(ImageView *imagev, int x, int y);
void              imgv_set_pixel_grayscale(
                 ImageView *image, int x, int y, grayscale_pixel_t new_value
             );

rgb8_pixel_t imgv_get_pixel_rgb8(ImageView *imagev, int x, int y);
void         imgv_set_pixel_rgb8(
            ImageView *imagev, int x, int y, rgb8_pixel_t new_value
        );

rgbf_pixel_t imgv_get_pixel_rgbf(ImageView *imagev, int x, int y);
void         imgv_set_pixel_rgbf(
            ImageView *imagev, int x, int y, rgbf_pixel_t new_value
        );
