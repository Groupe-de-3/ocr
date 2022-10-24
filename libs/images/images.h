#pragma once
#include <stdint.h>
#include <stdlib.h>

#include "utils_math.h"

enum PixelFormat {
    PixelFormat_GrayScale,
    PixelFormat_Rgb8,
    PixelFormat_Rgbf,
};
typedef float grayscale_pixel_t;
typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} rgb8_pixel_t;
typedef struct {
    float r;
    float g;
    float b;
} rgbf_pixel_t;

typedef union {
    float             float_channels[1];
    grayscale_pixel_t grayscale;
    rgb8_pixel_t      rgb8;
    rgbf_pixel_t      rgbf;
} any_pixel_t;
typedef struct {
    enum PixelFormat format;
    any_pixel_t      value;
} some_pixel_t;

typedef struct {
    enum PixelFormat format;
    union {
        grayscale_pixel_t *grayscale;
        rgb8_pixel_t      *rgb8;
        rgbf_pixel_t      *rgbf;
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
#define RGBF_BLACK                   \
    (rgbf_pixel_t) {                 \
        .r = 0.f, .g = 0.f, .b = 0.f \
    }
#define RGBF_WHITE                   \
    (rgbf_pixel_t) {                 \
        .r = 1.f, .g = 1.f, .b = 1.f \
    }

#define ANY_BLACK(FORMAT)                                                    \
    (((any_pixel_t[]                                                         \
    ){[PixelFormat_GrayScale] = (any_pixel_t){.grayscale = GRAYSCALE_BLACK}, \
      [PixelFormat_Rgb8]      = (any_pixel_t){.rgb8 = RGB8_BLACK},           \
      [PixelFormat_Rgbf]      = (any_pixel_t){.rgbf = RGBF_BLACK}})[FORMAT])
#define ANY_WHITE(FORMAT)                                                    \
    (((any_pixel_t[]                                                         \
    ){[PixelFormat_GrayScale] = (any_pixel_t){.grayscale = GRAYSCALE_WHITE}, \
      [PixelFormat_Rgbf]      = (any_pixel_t){.rgbf = RGBF_WHITE},           \
      [PixelFormat_Rgb8]      = (any_pixel_t){.rgb8 = RGB8_WHITE}})[FORMAT])

Image img_new(size_t width, size_t height, enum PixelFormat format);
void  img_destroy(Image image);

_Bool img_in_bound(Image *image, size_t x, size_t y);

typedef struct {
    _Bool success;
} ImageLoadResult;
ImageLoadResult img_load_file(
    char *filename, Image *image_out, _Bool detect_format,
    enum PixelFormat target_format
);

size_t           img_float_channels_length(enum PixelFormat format);
enum PixelFormat img_equivalent_float_channels_format(enum PixelFormat from);

inline grayscale_pixel_t img_some_to_grayscale(some_pixel_t value) {
    switch (value.format) {
    case PixelFormat_Rgb8:
        return (grayscale_pixel_t
        ){0.299f * ((float)value.value.rgb8.r / 255.f) +
          0.587f * ((float)value.value.rgb8.g / 255.f) +
          0.114f * ((float)value.value.rgb8.b / 255.f)};
    case PixelFormat_GrayScale:
        return value.value.grayscale;
    case PixelFormat_Rgbf:
        return (grayscale_pixel_t
        ){0.299f * value.value.rgbf.r + 0.587f * value.value.rgbf.g +
          0.114f * value.value.rgbf.b};
    }
}
inline rgb8_pixel_t img_some_to_rgb8(some_pixel_t value) {
    switch (value.format) {
    case PixelFormat_Rgb8:
        return value.value.rgb8;
    case PixelFormat_GrayScale:
        {
            uint8_t v =
                (uint8_t)(clamp(value.value.grayscale, 0., 1.) * (grayscale_pixel_t)255);
            return (rgb8_pixel_t){v, v, v};
        }
    case PixelFormat_Rgbf:
        return (rgb8_pixel_t){
            (uint8_t)(clamp(value.value.rgbf.r, 0., 1.) * 255.f),
            (uint8_t)(clamp(value.value.rgbf.g, 0., 1.) * 255.f),
            (uint8_t)(clamp(value.value.rgbf.b, 0., 1.) * 255.f),
        };
    }
}
inline rgbf_pixel_t img_some_to_rgbf(some_pixel_t value) {
    switch (value.format) {
    case PixelFormat_Rgb8:
        return (rgbf_pixel_t){
            ((float)value.value.rgb8.r / 255.f),
            ((float)value.value.rgb8.g / 255.f),
            ((float)value.value.rgb8.b / 255.f),
        };
    case PixelFormat_GrayScale:
        return (rgbf_pixel_t
        ){value.value.grayscale, value.value.grayscale, value.value.grayscale};
    case PixelFormat_Rgbf:
        return value.value.rgbf;
    }
}
inline any_pixel_t
img_some_to_any(some_pixel_t value, enum PixelFormat target) {
    switch (target) {
    case PixelFormat_Rgb8:
        return (any_pixel_t){.rgb8 = img_some_to_rgb8(value)};
    case PixelFormat_GrayScale:
        return (any_pixel_t){.grayscale = img_some_to_grayscale(value)};
    case PixelFormat_Rgbf:
        return (any_pixel_t){.rgbf = img_some_to_rgbf(value)};
    }
}

any_pixel_t img_get_pixel_any(Image *image, size_t x, size_t y);
void img_set_pixel_any(Image *image, size_t x, size_t y, any_pixel_t new_value);

some_pixel_t img_get_pixel_some(Image *image, size_t x, size_t y);
void         img_set_pixel_some(
            Image *image, size_t x, size_t y, some_pixel_t new_value
        );

grayscale_pixel_t img_get_pixel_grayscale(Image *image, size_t x, size_t y);
void              img_set_pixel_grayscale(
                 Image *image, size_t x, size_t y, grayscale_pixel_t new_value
             );

rgb8_pixel_t img_get_pixel_rgb8(Image *image, size_t x, size_t y);
void         img_set_pixel_rgb8(
            Image *image, size_t x, size_t y, rgb8_pixel_t new_value
        );

rgbf_pixel_t img_get_pixel_rgbf(Image *image, size_t x, size_t y);
void         img_set_pixel_rgbf(
            Image *image, size_t x, size_t y, rgbf_pixel_t new_value
        );
