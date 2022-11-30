#pragma once
#include "struct.h"

double CostFunction(Matrix(double) predictedOutputs, Matrix(double) expectedOutputs);

double CostDerivative(double predictedOutput, double expectedOutput);

Matrix(double) CostFunction_derivative(Matrix(double) predictedOutputs, Matrix(double) expectedOutputs);

