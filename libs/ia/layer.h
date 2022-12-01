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

/*! \brief Calcul loss of a layer
 *  
 *  Calcul the loss of the hidden layers (stock inside the struct)
 * 
 *  \param layer layer of the neural network
 *  \param next_layer next layer
 * 
 *  \return void
 */
void get_loss_hidden_layer(Layer layer, Layer next_layer);

/*! \brief Calcul loss of a layer
 *  
 *  Calcul the loss of the last layers (stock inside the struct)
 * 
 *  \param layer layer of the neural network
 *  \param expects the expected result
 * 
 *  \return void
 */
void get_loss_last_layer(Layer layer, Matrix(double) expects);

/*! \brief Get the gradient of the layer
 *  
 *  Initialize the weight and bias gradients (stock inside the struct)
 * 
 *  \param layer layer of the neural network
 *  \param inputs the inputs is the result of the previous layer
 * 
 *  \return void
 */
void get_layer_gradient(Layer layer, Matrix(double) inputs);
