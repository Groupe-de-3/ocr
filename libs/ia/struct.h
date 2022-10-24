#pragma once
#include <stdlib.h>
#include "matrices.h"
 #define MAX_LAYERS 10
 #define MAX_SIZE_LAYER 10

typedef struct {
    size_t layer_size;
    Matrix(double) m_bias;
    Matrix(double) m_weight;
} Layer;

typedef struct {
    Layer *layers_; // array of layers
    size_t layers_number; // number of layers in the neural network
    size_t *layers_sizes; // array of each layer size
} neural_network;
