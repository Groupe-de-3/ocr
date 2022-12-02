#pragma once
#include <stdlib.h>
#include <stdint.h>
#include "images.h"

typedef struct {
    size_t size;
    Image *images;
    uint8_t *labels;
} MnistDataSet;

MnistDataSet mnist_dataset_read(const char *images_pathname, const char *labels_pathname);
void mnist_dataset_destroy(MnistDataSet data_set);

