#pragma once
#include <stdlib.h>
#include "matrices.h"
#include "image_view.h"

typedef struct {
    /*! \brief The number of neurons in the layer.
     */
    size_t layer_size;
    /*! \brief Column matrix of the biases of each neurons.
     *
     *  Its height should be equal to the number of neurons (#layer_size).
     */
    Matrix(double) m_bias;
    /*! \brief Matrix for the weights of each neurons.
     *
     *  Its height should be equal to the number of neurons (#layer_size).
     *  Its width should be equal to the number of neurons in the previous layer.
     */
    Matrix(double) m_weight;
    /*! \brief Matrix of the gradients of the weights.
     *
     *  Initialized with zeros and accumulated during training.
     *  Its size must be the same as #m_weight.
     */
    Matrix(double) m_gradW;
    /*! \brief Matrix of the gradients of the biases.
     *
     *  Initialized with zeros and accumulated during training.
     *  Its size must be the same as #m_bias.
     */
    Matrix(double) m_gradB;
    /*! \brief Matrix containing the result of the last output before activation during training.
     *
     *  Its size must be the same as #m_bias.
     */
    Matrix(double) last_output;
    /*! \brief Matrix containing the previous prediction (after activation) during training.
     *
     *  Its size must be the same as #m_bias.
     */
    Matrix(double) last_output_activated;
    /*! \brief Matrix containing the loss after the training.
     *
     *  Its size must be the same as #m_bias.
     */
    Matrix(double) loss;
} Layer;

typedef struct {
    Layer *layers_;       // array of layers
    size_t layers_number; // number of layers in the neural network
    size_t *layers_sizes; // array of each layer size
} neural_network;


typedef struct {
    Matrix(double) input;
    Matrix(double) expect;
} DataPoint;

typedef struct {
    DataPoint *data;
    int size;
} Data;


typedef struct {
    Matrix(ImageView) sudoku_imgs;
    Matrix(bool) sudoku_mask;
} coople;


