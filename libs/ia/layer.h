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

//void ApplyGradients_layer(Layer layer, double learnRate);

/*! \brief Initialize the gradient of the last layer
 *  
 *  Gradient is save inside the structure of layer
 *
 *  \param layer last layer of the neural network
 *  \param expected vector of the expected result
 * 
 *  \return void
 */
void get_last_layer_gradientW(Layer layer, Matrix(double) expected, Matrix(double) previous_activated);
void get_last_layer_gradientB(Layer layer, Matrix(double) expected);

/*! \brief Initialize the gradient of the inside layers
 *
 *  Gradient is save inside the structure of layer
 *
 *  \param layer a idden layer of the neural network
 *  \param loss_next_layer matrice of the loss of the next layer
 *  \param next_weight matrice of the weights of the next layer
 * 
 *  \return void
 */
void get_hidden_layer_gradientW(Layer layer, Matrix(double) loss_next_layer, Matrix(double) next_weight);
void get_hidden_layer_gradientB(Layer layer, Matrix(double) loss_next_layer, Matrix(double) next_weight);
