#pragma once
#include "struct.h"

void Relu_Activate(Matrix(double) m);

void Relu_Derivative(Matrix(double) m);

void Sigmoid_Activate(Matrix(double) m);

void Sigmoid_Derivative(Matrix(double) m);

void Softmax_Activate(Matrix(double) inputs);

void Softmax_Derivative(Matrix(double) inputs);
