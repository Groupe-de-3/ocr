#pragma once

Matrix(double) CalculateOutputs_Layer(Layer layer, Matrix(double) input, int last);

void ApplyGradients_layer(Layer layer, double learnRate);

void get_last_layer_gradientW(Layer layer, Matrix(double) expected);

void get_hidden_layer_gradientW(Layer layer, Matrix(double) loss_next_layer, Matrix(double) next_weight);
