#include <assert.h>
#include <err.h>
#include <stdio.h>

#include "images.h"
#include "img_formats/bmp.h"

int main(int argc, char **argv) {
    assert(argc >= 2);

    Image           image;
    ImageLoadResult rslt = img_load_file(argv[1], &image);
    if (!rslt.success) {
        errx(1, "Could not load");
    }

    FILE *file = fopen("out.bmp", "wb");
    bmp_save_to_file(file, &image);
    fclose(file);

    img_destroy(image);
    return 0;
}
