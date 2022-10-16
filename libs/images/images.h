#pragma once
#include <stdlib.h>

enum ImageType {
    ImageType_GrayScale,
};
typedef float GRAYSCALE_PIXEL_TY;

typedef struct {
    enum ImageType type;
    union { 
        GRAYSCALE_PIXEL_TY *grayscale;
    } data;
    
    size_t width;
    size_t height;
} Image;

Image image_new(size_t width, size_t height, enum ImageType type);
void image_destroy(Image image);
