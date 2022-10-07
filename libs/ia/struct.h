#pragma once
 #define MAX_LAYERS 10
 #define MAX_SIZE_LAYER 10

// 10 is the current max of layers / size of layer
typedef struct Neurone {
    double bias;
    double weights[MAX_SIZE_LAYER];
} N;

typedef struct Layer {
    int layer_size; // size of the layer
    N neural_list[MAX_SIZE_LAYER]; // array of neural
} Layer;

typedef struct neural_network {
    Layer layers_[MAX_LAYERS]; // array of layers
    int layers_number; // number of layers in the neural network
    int layers_sizes[]; // array of each layer size
} neural_network;
