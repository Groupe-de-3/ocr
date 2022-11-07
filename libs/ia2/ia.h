#pragma once
#include "struct.h"

Matrix(double) CalculateOutputs_NN(Matrix(double) input, neural_network NN);

Matrix(double) Classify(neural_network NN, DataPoint datapoint);

double Cost(Matrix(double) outputs, Matrix(double) expects);

double Average_Cost(double** outputs, double** expects, int size);

void Launch(neural_network NN, Data d);

void Learn(neural_network *NN, Data data, double learnRate);

