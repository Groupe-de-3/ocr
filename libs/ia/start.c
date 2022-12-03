#include "matrices.h"
#include "struct.h"
#include "ia.h"
#include "activation.h"
#include "cost.h"
#include "layer.h"
#include "image_view.h"
#include "data.h"
#include "mnist.h"

#include <err.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>



void launch(neural_network NN, size_t nb_sample, MnistDataSet mnist)
{
    
    Data d = data_init(nb_sample); // a modif
    //srand(126);
    for (int i = 0; i < d.size; i++)
    {
        unsigned long ind_pic = ((unsigned long)rand()%mnist.size);
        Matrix(double) input = arr_to_mat(imgv_default(&mnist.images[ind_pic]));
        uint8_t e = mnist.labels[ind_pic];
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
    printf("\nSuccess rate : %f %%\n\n", ((float) nb_result) / ((float) d.size) *100.f);
    data_Destroy(d);
    
}

void train(neural_network NN, size_t nb_training, size_t nb_sample, MnistDataSet mnist)
{
    //data
    
    Data d = data_init(nb_sample); // a modif

    for (int i = 0; i < d.size; i++)
    {
        unsigned long ind_pic = ((unsigned long)rand()%mnist.size);
        Matrix(double) input = arr_to_mat(imgv_default(&mnist.images[ind_pic]));
        uint8_t e = mnist.labels[ind_pic];
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
    printf("\nSuccess rate : %f %%\n\n", ((float) nb_result) / ((float) d.size) *100.f);
    data_Destroy(d);

}
