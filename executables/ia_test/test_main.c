#include <stddef.h>
#include <stdio.h>

#include "save.h"

int main() {
    
     printf("%s", "test\n");
     size_t layers_number = 3;
     size_t layers_sizes[] = {3,4,2};
     double inputs[] = {0,1, 0};

     neural_network NN = ia_init(layers_number, layers_sizes);
     ia_save(&NN);

     printf("%zu\n", NN.layers_number);

     ia_memory_free(&NN); // free the memory
    

    printf("fin\n");

    return 0;
}
