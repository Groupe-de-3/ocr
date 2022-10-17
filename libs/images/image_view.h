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

grayscale_pixel_t imgv_get_pixel_grayscale(ImageView *imagev, int x, int y);
void              imgv_set_pixel_grayscale(
                 ImageView *image, int x, int y, grayscale_pixel_t new_value
             );

rgb8_pixel_t imgv_get_pixel_rgb8(ImageView *imagev, int x, int y);
void         imgv_set_pixel_rgb8(
            ImageView *imagev, int x, int y, rgb8_pixel_t new_value
        );
