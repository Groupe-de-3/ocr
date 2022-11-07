#pragma once
#include "struct.h"

double CostFunction(Matrix(double) predictedOutputs, Matrix(double) expectedOutputs);

double CostDerivative(double predictedOutput, double expectedOutput);

double NodeCost_derivate(double output, double expected);

double NodeCost(double output, double expected);

double Cost(Matrix(double) outputs, Matrix(double) expects);

