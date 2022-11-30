#include <stddef.h>
#include <stdio.h>

#include "data.h"
#include "ia.h"
#include "save.h"


void launch(neural_network NN)
{
    
    double val1 = 0;
    double val2 = 0;
    printf("value of input number 1: ");
    scanf("%lf", &val1);
    printf("value of input number 2: ");
    scanf("%lf", &val2);
    printf("\n");
    
    //data
    double inputs[2] = {val1, val2};
    double expects[2] = {0,0};
    Data d = data_init(1);
    d.data[0] = To_dataPoint(inputs, expects, NN.layers_sizes[0]);

    Launch(NN, d);

    data_Destroy(d);
}


void train(neural_network NN, size_t nb_training)
{
    //data
    /*
    double inputs[4][2] = {
        {0, 0},
        {0, 1},
        {1, 0},
        {1, 1}
    };
    double expects[4][2] = {
        {1, 0},
        {0, 1},
        {0, 1},
        {1, 0}
    }*/
    
    double inputs[4][2] = {
        {1, 0},
    };
    double expects[4][2] = {
        {0, 1},
    };

    Data d = data_init(2);

    for (size_t i = 0; i < d.size; i++)
        d.data[i] = To_dataPoint(inputs[i], expects[i], NN.layers_sizes[0]);


    for (size_t i = 0; i < nb_training; i++)
    {
        Learn(&NN, d, 0.1);
        Launch(NN, d);
    }

    data_Destroy(d);

}

void input_user(neural_network NN)
{
    char *filename = malloc(sizeof(char) * 30);
    char *output = malloc(sizeof(char) * 10);
    while (output[0] != 'q')
    {
        printf("Command : ");

        scanf("%s", output);
        printf("\n");
        switch (output[0])
        {
            case 't':
                printf("number of training? ");
                size_t nb_training = 0;
                scanf("%zu", &nb_training);
                train(NN, nb_training);
                printf("\n");
                break;

            case 's':
                printf("name of the neural network to save? ");
                scanf("%s", filename);
                ia_save(&NN, filename);
                printf("Neural network have been save in the file %s\n", filename);
                printf("\n");
                break;

             case 'l':
                printf("Launch\n");
                launch(NN);
                break;
            
            case 'L':
                printf("Name of the neural network to load? ");
                scanf("%s", filename);
                ia_memory_free(&NN);
                NN = ia_load(filename); 
                printf("Neural network have been load from the file %s\n", filename);
                printf("\n");
                break;

            case 'q': 
                printf("Quit\n");
                break;
            
            default:
                printf("Only commands are: \n q : quit\n t : train\n l : learn\n L : load\n s : save\n\n");
                break;
        }
    }

    free(output);
    free(filename);
}


int main() {

    
    printf("%s", "Launch\n\n");    
    size_t layers_number = 2;
    size_t layers_sizes[] = {2,5,2};

    size_t *layers_sizes_ = malloc(sizeof(size_t) * (layers_number + 1));
    for (size_t i = 0; i < layers_number + 1; i++)
        layers_sizes_[i] = layers_sizes[i];

    //init neural network
    neural_network NN = ia_init(layers_number, layers_sizes_);
    //neural_network NN = ia_load("xor.txt");

    input_user(NN);
    // free the memory
    ia_memory_free(&NN);


    
    return 0;
}
