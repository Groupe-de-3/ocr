#pragma once
#include "struct.h"

double single_relu_Activate(double val);

void Relu_Activate(Matrix(double) * m);

double Derivative(double inputs[], int index);
