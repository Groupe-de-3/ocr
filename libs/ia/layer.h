#pragma once

Matrix(double) CalculateOutputs_Layer(Layer layer, Matrix(double) input, int last);

void ApplyGradients_layer(Layer layer, double learnRate);

Matrix(double) get_last_layer_gradient(Layer layer, Matrix(double) expected);

Matrix(double) get_hidden_layer_gradient(Layer layer, Matrix(double) loss_next_layer, Matrix(double) next_weight);
