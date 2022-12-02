#include <stddef.h>
#include <stdio.h>

#include "data.h"
#include "ia.h"
#include "matrices.h"
#include "save.h"
#include "mnist.h"
#include "struct.h"


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
    Matrix(double) input = m_new(double, 1, 2);
    input[0] = val1;
    input[1] = val2;

    Launch(NN, input, 1);
    m_destroy(input);
    
}

void train(neural_network NN, size_t nb_training)
{
    
    MnistDataSet mnist = mnist_dataset_read("train-images-idx3-ubyte", "train-labels-idx1-ubyte");
    //data
    
    Data d = data_init(2); // a modif

    for (int i = 0; i < d.size; i++)
    {
        Matrix(double) input = arr_to_mat(imgv_default(&mnist.images[i]));
        uint8_t e = mnist.labels[i];
        Matrix(double) output = m_new(double, 1, NN.layers_sizes[NN.layers_number]);
        for (int j = 0; j < m_length(output); j++) {
            if (e == j)
                output[j] = 5;
            else
                output[j] = 0;
        }
        DataPoint dp = To_dataPoint(input , output, NN.layers_sizes[0], NN.layers_sizes[NN.layers_number]);
        d.data[i] = dp;
    }


    for (size_t i = 0; i < nb_training; i++)
    {
        Learn(&NN, d, 0.05);
    }

    for (int i = 0; i < d.size; i++) 
    {
        Launch(NN, d.data[i].input, 1);
        size_t ind = array_max_ind(d.data[i].expect);
        printf(" true result : %zu\n", ind); // print result
    }

    data_Destroy(d);
    mnist_dataset_destroy(mnist);

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
    size_t layers_sizes[] = {784,25,10};

    size_t *layers_sizes_ = malloc(sizeof(size_t) * (layers_number + 1));
    for (size_t i = 0; i < layers_number + 1; i++)
        layers_sizes_[i] = layers_sizes[i];

    //init neural network
    neural_network NN = ia_init(layers_number, layers_sizes_);

    input_user(NN);

    // free the memory
    ia_memory_free(&NN);
    
    return 0;
}
