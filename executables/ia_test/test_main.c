#include <stddef.h>
#include <stdio.h>

#include "save.h"

int main() {
    
     printf("%s", "test\n");
     size_t layers_number = 3;
     size_t layers_sizes[] = {2,3,2};
     double inputs[] = {0,1};

     neural_network NN = ia_init(layers_number, layers_sizes);
     //save(&NN);
     printf("test\n");

     printf("%zu", NN.layers_number);
     //printf("%d",NN->layers_[0].neural_list[0].bias);

     ia_memory_free(&NN); // free the memory
    

    //neural_network * NN = init("neural_network.txt");

    //memory_free(NN);
    printf("fin\n");

    return 0;
}
