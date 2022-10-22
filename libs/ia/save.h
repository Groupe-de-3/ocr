#pragma once
#include "struct.h"

/*! \file save.h
 *
 *  \brief This library help manipulate the neurla network 
 *
 *  This library contains the , #init, #load, #save function.
 */



/*! \brief Initialization of the neural network
 *
 *  
 *  Create the neural network with the given layers and size 
 *  with random value in the bias and weigth
 *
 *  \param layers_number Number of layer of the neural network
 *  \param layers_sizes a array of the lenght of each layer
 *  \param inputs a array of the inputs value
 * 
 *  \return neural_wetwork
 */
neural_network init(int layers_number, int layers_sizes[]);

/*! \brief Load of the neural network
 *
 *  Return the neural network from the given file
 *
 *  \param file_name name of the file that contain the neural network value
 * 
 *  \return neural_wetwork
 */
neural_network * load(char* file_name);


/*! \brief Save the neural network
 *
 *  Save the current neural network
 *  The file can be removed with #clear
 *  
 *  \param NN The neural network.
 *
 *  \return void
 */
void save(neural_network *NN);

/*! \brief Deletes files created by #save
 */
void clear(void);


void memory_free(neural_network *NN);

