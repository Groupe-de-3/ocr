#pragma once
#include <stdlib.h>
 #define MAX_LAYERS 10
 #define MAX_SIZE_LAYER 10

// 10 is the current max of layers / size of layer
typedef struct {
    double bias;
    double *weights;
} Neurone;

typedef struct {
    int layer_size; // size of the layer
    Neurone *neural_list; // array of neural
} Layer;

typedef struct {
    Layer *layers_; // array of layers
    int layers_number; // number of layers in the neural network
    int *layers_sizes; // array of each layer size
} neural_network;
