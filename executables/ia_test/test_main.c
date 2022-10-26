#include <stddef.h>
#include <stdio.h>
#include "ia.h"
#include "save.h"
#include "data.h"

int main() {
    
    printf("%s", "test\n");    
    size_t layers_number = 2;
    size_t layers_sizes[] = {2,8,2};

    size_t *layers_sizes_ = malloc(sizeof(size_t) * (layers_number+1));
    for (size_t i = 0; i < layers_number+1; i++)
        layers_sizes_[i] = layers_sizes[i];

    double inputs[] = {0,0};
    double expected_output[] = {0,1};
    //DataPoint datapoint = To_dataPoint(inputs, expected_output, layers_sizes[0]);

    neural_network NN = ia_init(layers_number, layers_sizes_);
    //neural_network NN = ia_load("neural_network.txt");    

    Launch(NN, inputs);

    //Learn(NN, datapoint);

    ia_save(&NN, "neural_network.txt");
    ia_memory_free(&NN); // free the memory

    return 0;
}
