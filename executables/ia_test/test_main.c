#include <stddef.h>
#include <stdio.h>
#include "ia.h"
#include "save.h"
#include "data.h"

int main() {
    printf("%s", "Launch\n\n");    
    size_t layers_number = 2;
    size_t layers_sizes[] = {2,8,2};

    size_t *layers_sizes_ = malloc(sizeof(size_t) * (layers_number+1));
    for (size_t i = 0; i < layers_number+1; i++)
        layers_sizes_[i] = layers_sizes[i];

    //data
    double inputs[4][2] = {{0,0}, {1,0}, {0,1}, {1,1}};
    double expects[4][2] = {{0,0}, {0,1}, {0,1}, {0,0}};
    Data d = data_init(4);
    
    for (size_t i = 0; i < d.size; i++)
        d.data[i] = To_dataPoint(inputs[i], expects[i], layers_sizes[0]);
    
    //init neural network
    neural_network NN = ia_init(layers_number, layers_sizes_);
    //neural_network NN = ia_load("neural_network.txt");    


    for (size_t i = 0; i < 10000; i++)
    {
        Learn(&NN, d, 1);
    }
    
    
    // start forward
    Launch(NN, d);

    // start learning
    //Learn(NN, datapoint);

    //ia_save(&NN, "neural_network.txt");


    // free the memory
    ia_memory_free(&NN); 
    data_Destroy(d);


    return 0;
}
