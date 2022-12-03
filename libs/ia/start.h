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

void launch(neural_network NN, size_t nb_sample, MnistDataSet mnist);
void train(neural_network NN, size_t nb_training, size_t nb_sample, MnistDataSet mnist);