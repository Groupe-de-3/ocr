#include "image_view.h"

#include <assert.h>
#include <err.h>
#include <stdbool.h>

#include "utils_math.h"

_Bool imgv_in_bound(ImageView *imagev, int x, int y) {
    return x >= 0 && y >= 0 && x < (int)imagev->width &&
           y < (int)imagev->height;
}

ImageView imgv_default(Image *image) {
    return (ImageView){
        .image              = image,
        .wraping_mode       = WrappingMode_None,
        .ignore_invalid_set = false,
        .x                  = 0,
        .y                  = 0,
        .width              = (int)image->width,
        .height             = (int)image->height,
    };
}

void imgv_copy(ImageView *from, ImageView *to) {
    assert(from->width == to->width);
    assert(from->height == to->height);

    for (int y = 0; y < from->height; y++) {
        for (int x = 0; x < from->width; x++) {
            imgv_set_pixel_some(to, x, y, imgv_get_pixel_some(from, x, y));
        }
    }
}

static void wrap_coords(ImageView *imagev, int *x, int *y) {
    switch (imagev->wraping_mode) {
    case WrappingMode_Clamp:
        *x = clamp(*x, 0, imagev->width - 1);
        *y = clamp(*y, 0, imagev->height - 1);
        break;
    case WrappingMode_Repeat:
        // Weird modulo for negative values
        *x = (*x % imagev->width + imagev->width) % imagev->width;
        *y = (*y % imagev->height + imagev->height) % imagev->height;
        break;
    default:
        errx(1, "Cannot wrap with given wraping mode");
    }
}

any_pixel_t imgv_get_pixel_any(ImageView *imagev, int x, int y) {
    // Try to resolve the out of bound if applicable
    if (!imgv_in_bound(imagev, x, y)) {
        switch (imagev->wraping_mode) {
        case WrappingMode_None:
            errx(
                1,
                "Image coords out of bounds: (%d, %d) not in bounds (%d x %d)",
                x, y, imagev->width, imagev->height
            );
        case WrappingMode_Black:
            return ANY_BLACK(imagev->image->format);
        case WrappingMode_White:
            return ANY_WHITE(imagev->image->format);
        case WrappingMode_Clamp:
        case WrappingMode_Repeat:
            wrap_coords(imagev, &x, &y);
        }
    }

    // Now, x and y should be valid coordinates
    return img_get_pixel_any(
        imagev->image, (size_t)(x + imagev->x), (size_t)(y + imagev->y)
    );
}
void imgv_set_pixel_any(
    ImageView *imagev, int x, int y, any_pixel_t new_value
) {
    // Try to resolve the out of bound if applicable
    if (!imgv_in_bound(imagev, x, y)) {
        switch (imagev->wraping_mode) {
        case WrappingMode_None:
        case WrappingMode_Black:
        case WrappingMode_White:
            if (imagev->ignore_invalid_set)
                return;
            else
                errx(
                    1,
                    "Image coords out of bounds: (%d, %d) not in bounds (%d x "
                    "%d)",
                    x, y, imagev->width, imagev->height
                );
        case WrappingMode_Clamp:
        case WrappingMode_Repeat:
            wrap_coords(imagev, &x, &y);
        }
    }

    // Now, x and y should be valid coordinates
    img_set_pixel_any(
        imagev->image, (size_t)(x + imagev->x), (size_t)(y + imagev->y),
        new_value
    );
}

some_pixel_t imgv_get_pixel_some(ImageView *imagev, int x, int y) {
    return (some_pixel_t
    ){.format = imagev->image->format,
      .value  = imgv_get_pixel_any(imagev, x, y)};
}
void imgv_set_pixel_some(
    ImageView *imagev, int x, int y, some_pixel_t new_value
) {
    imgv_set_pixel_any(
        imagev, x, y, img_some_to_any(new_value, imagev->image->format)
    );
}

grayscale_pixel_t imgv_get_pixel_grayscale(ImageView *imagev, int x, int y) {
    return img_some_to_grayscale(imgv_get_pixel_some(imagev, x, y));
}
void imgv_set_pixel_grayscale(
    ImageView *imagev, int x, int y, grayscale_pixel_t new_value
) {
    imgv_set_pixel_some(
        imagev, x, y,
        (some_pixel_t
        ){.format = PixelFormat_GrayScale, .value = {.grayscale = new_value}}
    );
}

rgb8_pixel_t imgv_get_pixel_rgb8(ImageView *imagev, int x, int y) {
    return img_some_to_rgb8(imgv_get_pixel_some(imagev, x, y));
}
void imgv_set_pixel_rgb8(
    ImageView *imagev, int x, int y, rgb8_pixel_t new_value
) {
    imgv_set_pixel_some(
        imagev, x, y,
        (some_pixel_t){.format = PixelFormat_Rgb8, .value = {.rgb8 = new_value}}
    );
}

rgbf_pixel_t imgv_get_pixel_rgbf(ImageView *imagev, int x, int y) {
    return img_some_to_rgbf(imgv_get_pixel_some(imagev, x, y));
}
void imgv_set_pixel_rgbf(
    ImageView *imagev, int x, int y, rgbf_pixel_t new_value
) {
    imgv_set_pixel_some(
        imagev, x, y,
        (some_pixel_t){.format = PixelFormat_Rgbf, .value = {.rgbf = new_value}}
    );
}
