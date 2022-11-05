#pragma once

Matrix(double) CalculateOutputs_Layer(Layer layer, Matrix(double) input, int last);

void ApplyGradients_layer(Layer layer, Matrix(double) costB, Matrix(double) costW, double learnRate);
