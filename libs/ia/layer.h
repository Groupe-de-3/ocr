#pragma once


/*! \brief Calcul output of a layer
 *  
 *  Return the output of the pass forward of a layer
 *  Initialize last_output_activated and last_activated in the layer
 * 
 *  \param layer layer of the neural network
 *  \param last bool to know if it's the last layer
 * 
 *  \return Matrix(double)
 */
Matrix(double) CalculateOutputs_Layer(Layer layer, Matrix(double) input, int last);

void get_loss_hidden_layer(Layer layer, Layer next_layer);

void get_loss_last_layer(Layer layer, Matrix(double) expects);

void get_layer_gradient(Layer layer, Matrix(double) inputs);
