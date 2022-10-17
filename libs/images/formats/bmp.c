#include "bmp.h"

#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

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
        printf(
            "Could not decode bmp: Unsuported header size (%d)\n", header_size
        );
        return BmpLoadResult_UnsuportedBmp;
    }

    uint32_t img_width  = readu32(file);
    uint32_t img_height = readu32(file);
    fseek(file, 2, SEEK_CUR); // Skip color planes
    uint16_t bits_per_pixel     = readu16(file);
    uint32_t compression_method = readu32(file);
    fseek(file, 4, SEEK_CUR); // Skip image data size
    fseek(file, 8, SEEK_CUR); // Skip image resolution

    uint32_t palette_length = readu32(file);
    if (palette_length == 0) // 0 is for the default value
        palette_length = 1 << bits_per_pixel;
    bool has_pallette = bits_per_pixel <= 8;

    if (bits_per_pixel != 24 && bits_per_pixel != 8) {
        printf(
            "Could not decode bmp: Unsuported bits per pixels (%d)\n",
            bits_per_pixel
        );
        return BmpLoadResult_UnsuportedBmp;
    }
    if (compression_method != 0) {
        printf(
            "Could not decode bmp: Unsuported compression method (%d)\n",
            compression_method
        );
        return BmpLoadResult_UnsuportedBmp;
    }

    // Seeking to the color palette
    fseek(file, 14 + header_size, SEEK_SET);
    uint8_t color_palette[palette_length][4];
    if (palette_length > 0)
        fread(color_palette, sizeof(uint8_t[4]), palette_length, file);

    // Seeking to the image data
    fseek(file, image_data_offset, SEEK_SET);

    *image_out = img_new(img_width, img_height, ImageType_Rgb8);

    // Calculating row padding, beacuse each row must be a multiple of 4 bytes
    // in size.
    long row_size    = img_width * (bits_per_pixel / 8);
    long row_padding = 0;
    while (((row_size + row_padding) & 0x3) != 0)
        row_padding++;

    uint8_t current_pixel[3];
    for (uint32_t y = 0; y < img_height; y++) {
        for (uint32_t x = 0; x < img_width; x++) {
            if (has_pallette) {
                uint8_t index;
                fread(&index, sizeof(uint8_t), bits_per_pixel / 8, file);
                memcpy(
                    current_pixel, &color_palette[index], sizeof(uint8_t) * 3
                );
            }
            else {
                fread(current_pixel, 1, 3, file);
            }
            img_set_pixel_rgb8(
                image_out, x, img_height - y - 1,
                (rgb8_pixel_t){
                    // RGB is acutally saved as BGR
                    .r = current_pixel[2],
                    .g = current_pixel[1],
                    .b = current_pixel[0],
                }
            );
        }
        fseek(file, row_padding, SEEK_CUR);
    }

    return BmpLoadResult_Ok;
}

static void writeu8(FILE *file, uint8_t value) {
    fwrite(&value, sizeof(uint8_t), 1, file);
}
static void writeu32(FILE *file, uint32_t value) {
    fwrite(&value, sizeof(uint32_t), 1, file);
}
static void writeu16(FILE *file, uint16_t value) {
    fwrite(&value, sizeof(uint16_t), 1, file);
}

void bmp_save_to_file(FILE *file, Image *image) {
    writeu8(file, (uint8_t)'B');
    writeu8(file, (uint8_t)'M'); // Magic first bytes
    writeu32(
        file,
        14 + // Size of bitmap header
            40 + // size of info header
            3 * (uint32_t)image->width *
                (uint32_t)image->height // Size of pixel data
    ); // The size of the whole file
    writeu32(file, 0); // Useless
    writeu32(file, 14 + 40); // Starting address of image data

    writeu32(file, 40); // The size of the BITMAPINFOHEADER
    writeu32(file, (uint32_t)image->width);
    writeu32(file, (uint32_t)image->height);
    writeu16(file, 1); // Color panes are hard coded
    writeu16(file, 24); // Bits per pixel here always 24
    writeu32(file, 0); // No compression
    writeu32(file, 0); // Image size is ignored since there is not compression
    writeu32(file, 3780); // X Resolution (here set to equivalent of 96dpi)
    writeu32(file, 3780); // Y Resolution
    writeu32(file, 0); // Empty palette
    writeu32(file, 0); // No important colors

    long row_size    = (long)image->width * 3;
    long row_padding = 0;
    while (((row_size + row_padding) & 0x3) != 0)
        row_padding++;

    for (uint32_t y = 0; y < image->height; y++) {
        for (uint32_t x = 0; x < image->width;
             x++) { // pixels are still left to right so no problem there
            rgb8_pixel_t pixel_value = img_get_pixel_rgb8(
                image, x,
                // BMP files have pixel bottom to top, while we want to load to
                // top to bottom
                image->height - y - 1
            );
            // Stored in reverse order
            writeu8(file, pixel_value.b);
            writeu8(file, pixel_value.g);
            writeu8(file, pixel_value.r);
        }
        for (int i = 0; i < row_padding; i++)
            writeu8(file, 0);
    }
}
