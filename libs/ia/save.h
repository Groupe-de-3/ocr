#pragma once

typedef struct Neurone {
    double bias;
    double weights[10];
} N;

typedef struct Layer {
    N layer_[10];
} Layer;

typedef struct neural_network {
    Layer layers_[10];
} neural_network;

/*! \brief Initialize a new neural network with random weights and biases.
 */
neural_network init(void);

/*! \brief Loads the neural network saved in "neural_network.txt"
 */
neural_network load(void);
/*! \brief Serialize a neural network into a file named "neural_network.txt".
 *
 *  The file can be removed with #clear
 *
 *  \param NN The neural network.
 */
void save(neural_network NN);
/*! \brief Deletes files created by #save
 */
void clear(void);
