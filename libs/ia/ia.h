#pragma once
#include "struct.h"

Matrix(double) CalculateOutputs_NN(Matrix(double) input, neural_network NN);

size_t Classify(neural_network NN, double inputs[]);

double NodeCost(double output, double expected);

double Cost(double* outputs, double* expects, int size);

double Average_Cost(double** outputs, double** expects, int size);
