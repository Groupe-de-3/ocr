#pragma once
#include "struct.h"

/*! \brief Calcul output of the neural network
 *  
 *  Return the output of the pass forward of the neural network with
 *  the form of a vector 
 * 
 *  \param inputs data set in input
 *  \param NN neural network
 * 
 *  \return Matrix(double)
 */
Matrix(double) CalculateOutputs_NN(Matrix(double) input, neural_network NN);

void Print_array(Matrix(double) m);

double Cost(Matrix(double) outputs, Matrix(double) expects);

/*! \brief Start the neural network

 *  Print the result of the pass forward of each data in d
 * 
 *  \param NN neural_network
 *  \param data data that will be tested on
 * 
 *  \return void
 */
void Launch(neural_network NN, Matrix(double) input);

/*! \brief Start the learning of the neural network

 *  Train with each Data in data
 * 
 *  \param NN neural_network
 *  \param data data that will be tested on
 *  \param learnRate the speed of the learning (usualy set to 0.05)
 * 
 *  \return void
 */
void Learn(neural_network *NN, Data data, double learnRate);

