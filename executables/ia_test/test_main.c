#include <stddef.h>
#include <stdio.h>

#include "data.h"
#include "ia.h"
#include "matrices.h"
#include "save.h"
#include "mnist.h"
#include "struct.h"


void launch(neural_network NN, MnistDataSet mnist)
{
    
    Data d = data_init(100); // a modif

    for (int i = 0; i < d.size; i++)
    {

        Matrix(double) input = arr_to_mat(imgv_default(&mnist.images[i]));
        uint8_t e = mnist.labels[i];
        Matrix(double) output = m_new(double, 1, NN.layers_sizes[NN.layers_number]);
        for (size_t j = 0; j < m_length(output); j++) {
            if (e == j)
                output[j] = 5;
            else
                output[j] = 0;
        }
        DataPoint dp = To_dataPoint(input , output);
        d.data[i] = dp;
    }

    int nb_result = 0;
    for (int i = 0; i < d.size; i++) 
    {
        size_t ind = array_max_ind(d.data[i].expect);
        nb_result += Launch(NN, d.data[i].input, ind, 1);
    }
    printf("\nSuccess rate : %f %%\n", ((float) nb_result) / ((float) d.size) *100.f);
    data_Destroy(d);
    
}

void train(neural_network NN, size_t nb_training, size_t nb_sample, MnistDataSet mnist)
{
    //data
    
    Data d = data_init(nb_sample); // a modif

    for (int i = 0; i < d.size; i++)
    {
        Matrix(double) input = arr_to_mat(imgv_default(&mnist.images[i]));
        uint8_t e = mnist.labels[i];
        Matrix(double) output = m_new(double, 1, NN.layers_sizes[NN.layers_number]);
        for (size_t j = 0; j < m_length(output); j++) {
            if (e == j)
                output[j] = 5;
            else
                output[j] = 0;
        }
        DataPoint dp = To_dataPoint(input , output);
        d.data[i] = dp;
    }


    for (size_t i = 0; i < nb_training; i++)
    {
        Learn(&NN, d, 0.01);
    }

    int nb_result = 0;
    for (int i = 0; i < d.size; i++) 
    {
        size_t ind = array_max_ind(d.data[i].expect);
        nb_result += Launch(NN, d.data[i].input, ind, 1);
    }
    printf("\nSuccess rate : %f %%\n", ((float) nb_result) / ((float) d.size) *100.f);
    data_Destroy(d);

}

void input_user(neural_network NN, MnistDataSet mnist)
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
                printf("size of sample? ");
                size_t nb_sample = 0;
                scanf("%zu", &nb_sample);
                train(NN, nb_training, nb_sample, mnist);
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
                launch(NN, mnist);
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
    MnistDataSet mnist = mnist_dataset_read("train-images-idx3-ubyte", "train-labels-idx1-ubyte");

    input_user(NN, mnist);

    // free the memory
    ia_memory_free(&NN);
    mnist_dataset_destroy(mnist);
    return 0;
}
