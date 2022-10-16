#include "images.h"
#include <err.h>

Image image_new(size_t width, size_t height, enum ImageType type) {
    Image image = {
        .width = width,
        .height = height,
        .type = type,
    };

    switch (type) {
        case ImageType_GrayScale:
            image.data.grayscale = calloc(sizeof(float), width * height);
            return image;
        default:
            errx(1, "image_new: Invalid ImageType");
    }
    
}

void image_destroy(Image image) {
    switch (image.type) {
        case ImageType_GrayScale:
            free(image.data.grayscale);
            break;
        default:
            errx(1, "image_destroy: Invalid ImageType");
    }
}
