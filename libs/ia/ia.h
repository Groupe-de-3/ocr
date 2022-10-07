#pragma once
#include "struct.h"

double* CalculateOutputs_layer(double inputs[], Layer layer);

double* CalculateOutputs_NN(double inputs[], neural_network NN);

int Classify(neural_network NN, double inputs[]);
