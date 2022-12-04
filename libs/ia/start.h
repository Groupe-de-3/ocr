#pragma once
#include "matrices.h"
#include "struct.h"
#include "ia.h"
#include "activation.h"
#include "cost.h"
#include "layer.h"
#include "image_view.h"
#include "data.h"
#include "mnist.h"


#include <err.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*! \brief Start the neural network

 *  Print the result of the pass forward of the ImageView
 * 
 *  \param NN neural_network
 *  \param im the image of a number (input)
 * 
 *  \return the guess number
 */
int ia_launch(neural_network NN, ImageView im);

void launch(neural_network NN, size_t nb_sample, MnistDataSet mnist);
void train(neural_network NN, size_t nb_training, size_t nb_sample, MnistDataSet mnist);