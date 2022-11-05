#pragma once
#include <stdlib.h>
#include "matrices.h"

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


typedef struct {
    Matrix(double) input;
    Matrix(double) expect;
} DataPoint;

typedef struct {
    DataPoint *data;
    size_t size;
} Data;


typedef struct {
	LayerLearnData[] layerData;
} NetworkLearnData;


typedef struct {
	Matrix(double) inputs;
	Matrix(double) weightedInputs;
	Matrix(double) activations;
	Matrix(double) nodeValues;
} LayerLearnData;
