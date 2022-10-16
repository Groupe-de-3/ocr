#include "bmp.h"

#include <math.h>
#include <stdint.h>

#include "images.h"

static uint32_t readu32(FILE *file) {
    uint32_t value;
    fread(&value, sizeof(uint32_t), 1, file);
    return value;
}
static uint16_t readu16(FILE *file) {
    uint16_t value;
    fread(&value, sizeof(uint16_t), 1, file);
    return value;
}

enum BmpLoadResult bmp_load_file(FILE *file, Image *image_out) {
    // skiping useless values
    fseek(file, 10, SEEK_SET);

    uint32_t image_data_offset = readu32(file);
    uint32_t header_size       = readu32(file);

    if (header_size < 40) {
        printf("Could not decode bmp: Unsuported header size\n");
        return BmpLoadResult_UnsuportedBmp;
    }

    uint32_t img_width  = readu32(file);
    uint32_t img_height = readu32(file);
    fseek(file, 2, SEEK_CUR); // Skip color planes
    uint16_t bits_per_pixel     = readu16(file);
    uint32_t compression_method = readu32(file);
    fseek(file, 4, SEEK_CUR); // Skip image data size
    fseek(file, 8, SEEK_CUR); // Skip image resolution
    uint32_t palette_size = readu32(file);

    if (bits_per_pixel != 24 || palette_size != 0) {
        printf("Could not decode bmp: Unsuported bits per pixels\n");
        return BmpLoadResult_UnsuportedBmp;
    }
    if (compression_method != 0) {
        printf("Could not decode bmp: Unsuported compression method\n");
        return BmpLoadResult_UnsuportedBmp;
    }
    // Seeking to the image data
    fseek(file, image_data_offset, SEEK_SET);

    *image_out = img_new(img_width, img_height, ImageType_GrayScale);

    long row_size    = img_width * 3;
    long row_padding = 0;
    while (((row_size + row_padding) & 0x3) != 0)
        row_padding++;

    uint8_t current_pixel[3];
    for (uint32_t y = 0; y < img_height;
         y++) { // BMP files have pixel bottom to top, while we want to save to
                // top to bottom
        for (uint32_t x = 0; x < img_width;
             x++) { // pixels are still left to right so no problem there
            fread(current_pixel, 1, 3, file);
            img_set_pixel_grayscale(
                image_out, x, y,
                img_rbg8_to_gayscale(
                    current_pixel[0], current_pixel[1], current_pixel[1]
                )
            );
        }
        fseek(file, row_padding, SEEK_CUR);
    }

    return BmpLoadResult_Ok;
}
