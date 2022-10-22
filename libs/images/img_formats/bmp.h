#include <stdio.h>

#include "images.h"

enum BmpLoadResult {
    BmpLoadResult_Ok = 0,
    BmpLoadResult_InvalidBmp,
    BmpLoadResult_UnsuportedBmp,
};

enum BmpLoadResult bmp_load_file(
    FILE *file, Image *image_out, _Bool detect_format,
    enum PixelFormat target_format
);
void bmp_save_to_file(FILE *file, Image *image);
void bmp_save_to_path(const char *filepath, Image *image);
