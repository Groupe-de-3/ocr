#include <stddef.h>
#include <stdio.h>
#include "ia.h"
#include "save.h"

int main() {
    
     printf("%s", "test\n");
     size_t layers_number = 2;

     size_t * layers_sizes = malloc(sizeof(size_t) * (layers_number+1));
     layers_sizes[0] = 2;
     layers_sizes[1] = 8;
     layers_sizes[2] = 2;

     double inputs[] = {0,0};

     neural_network NN = ia_init(layers_number, layers_sizes);
     //neural_network NN = ia_load("neural_network.txt");

     size_t i = Classify(NN, inputs);

     printf("result : %zu\n", i);
     ia_save(&NN, "neural_network.txt");
     ia_memory_free(&NN); // free the memory

    return 0;
}
