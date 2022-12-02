#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <endian.h>
#include "mnist.h"

MnistDataSet mnist_dataset_read(const char *images_pathname, const char *labels_pathname) {
    FILE *images_file = fopen(images_pathname, "rb");
    if (images_file == NULL) {
        printf("Could not open file (%s): System error %i\n", images_pathname, errno);
        exit(1);
    }
    FILE *labels_file = fopen(labels_pathname, "rb");
    if (labels_file == NULL) {
        printf("Could not open file (%s): System error %i\n", images_pathname, errno);
        exit(1);
    }

    uint32_t magic_number;
    fread(&magic_number, sizeof(uint32_t), 1, images_file);
    magic_number = be32toh(magic_number);
    if (magic_number != 2051) {
        printf("Could not decode file (%s): Invalid file format\n", images_pathname);
        exit(1);
    }
    fread(&magic_number, sizeof(uint32_t), 1, labels_file);
    magic_number = be32toh(magic_number);
    if (magic_number != 2049) {
        printf("Could not decode file (%s): Invalid file format\n", labels_pathname);
        exit(1);
    }

    uint32_t images_count;
    fread(&images_count, sizeof(uint32_t), 1, images_file);
    images_count = be32toh(images_count);
    uint32_t labels_count;
    fread(&labels_count, sizeof(uint32_t), 1, labels_file);
    labels_count = be32toh(labels_count);

    if (images_count != labels_count) {
        printf("Could not decode mnist data (images: %s) (labels: %s): Images and labels count does not match", images_pathname, labels_pathname);
        exit(1);
    }

    uint32_t images_width;
    uint32_t images_height;
    fread(&images_height, sizeof(uint32_t), 1, images_file);
    images_height = be32toh(images_height);
    fread(&images_width, sizeof(uint32_t), 1, images_file);
    images_width = be32toh(images_width);

    MnistDataSet data_set;
    data_set.size = images_count;
    data_set.images = malloc(sizeof(Image) * data_set.size);
    data_set.labels = malloc(sizeof(uint8_t) * data_set.size);

    for (size_t i = 0; i < data_set.size; i++) {
        fread(&data_set.labels[i], sizeof(uint8_t), 1, labels_file);

        Image *current_image = &data_set.images[i];
        *current_image = img_new(images_width, images_height, PixelFormat_GrayScale);
        for (size_t y = 0; y < images_height; y++) {
            for (size_t x = 0; x < images_width; x++) {
                uint8_t pixel;
                fread(&pixel, sizeof(uint8_t), 1, images_file);
                img_set_pixel_grayscale(
                    current_image, x, y,
                    (float)pixel / 255.f
                );
            }
        }
    }

    return data_set;
}

void mnist_dataset_destroy(MnistDataSet data_set) {
    for (size_t image_i = 0; image_i < data_set.size; image_i++) {
        img_destroy(data_set.images[image_i]);
    }
    free(data_set.labels);
    free(data_set.images);
}

