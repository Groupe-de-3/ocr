#include "image_view.h"

#include <assert.h>
#include <err.h>
#include <stdbool.h>

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

static void wrap_coords(ImageView *imagev, int *x, int *y) {
    switch (imagev->wraping_mode) {
    case WrappingMode_Clamp:
        if (*x >= imagev->width)
            *x = imagev->width - 1;
        else if (*x < 0)
            *x = 0;

        if (*y >= imagev->height)
            *y = imagev->height - 1;
        else if (*y < 0)
            *y = 0;
    case WrappingMode_Repeat:
        *x = (*x % imagev->width + imagev->width) % imagev->width;
        *y = (*y % imagev->height + imagev->height) % imagev->height;
    default:
        errx(1, "Cannot wrap with given wraping mode");
    }
}

grayscale_pixel_t imgv_get_pixel_grayscale(ImageView *imagev, int x, int y) {
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
            return GRAYSCALE_BLACK;
        case WrappingMode_White:
            return GRAYSCALE_WHITE;
        case WrappingMode_Clamp:
        case WrappingMode_Repeat:
            wrap_coords(imagev, &x, &y);
        }
    }

    // Now, x and y should be valid coordinates
    return img_get_pixel_grayscale(
        imagev->image, (size_t)(x + imagev->x), (size_t)(y + imagev->y)
    );
}
void imgv_set_pixel_grayscale(
    ImageView *imagev, int x, int y, grayscale_pixel_t new_value
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
    img_set_pixel_grayscale(
        imagev->image, (size_t)(x + imagev->x), (size_t)(y + imagev->y),
        new_value
    );
}

rgb8_pixel_t imgv_get_pixel_rgb8(ImageView *imagev, int x, int y) {
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
            return RGB8_BLACK;
        case WrappingMode_White:
            return RGB8_WHITE;
        case WrappingMode_Clamp:
        case WrappingMode_Repeat:
            wrap_coords(imagev, &x, &y);
        }
    }

    // Now, x and y should be valid coordinates
    return img_get_pixel_rgb8(
        imagev->image, (size_t)(x + imagev->x), (size_t)(y + imagev->y)
    );
}
void imgv_set_pixel_rgb8(
    ImageView *imagev, int x, int y, rgb8_pixel_t new_value
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
    img_set_pixel_rgb8(
        imagev->image, (size_t)(x + imagev->x), (size_t)(y + imagev->y),
        new_value
    );
}
