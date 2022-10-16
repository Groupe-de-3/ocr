#include <stdio.h>

#include "images.h"

enum BmpLoadResult {
    BmpLoadResult_Ok = 0,
    BmpLoadResult_InvalidBmp,
    BmpLoadResult_UnsuportedBmp,
};

enum BmpLoadResult bmp_load_file(FILE *file, Image *image_out);
