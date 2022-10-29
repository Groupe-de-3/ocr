#include "image_mask.h"

void image_mask_run(ImageView *target, ImageView *mask) {
    const any_pixel_t masked_color = ANY_BLACK(target->image->format);
    for (int y = 0; y < target->height; y++) {
        for (int x = 0; x < target->width; x++) {
            if (imgv_get_pixel_grayscale(mask, x, y) < 0.5)
                imgv_set_pixel_any(target, x, y, masked_color);
        }
    }
}
