#include <stddef.h>
#include <stdio.h>
#include "ia.h"
#include "save.h"

int main() {
    
     printf("%s", "test\n");
     size_t layers_number = 2;
     size_t layers_sizes[] = {2,8,2};
     double inputs[] = {1,1};

     neural_network NN = ia_init(layers_number, layers_sizes);
     

     size_t i = Classify(NN, inputs);

     printf("result : %zu\n", i);
     ia_save(&NN);
     ia_memory_free(&NN); // free the memory

    return 0;
}
