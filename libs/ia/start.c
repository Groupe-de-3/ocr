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



int ia_launch_single(neural_network NN, ImageView im)
{
    Matrix(double) input = arr_to_mat(im);
    int res = Launch(NN, input, 0, 2);
    m_destroy(input);
    return res;
}

char* ia_launch(neural_network NN, Matrix(ImageView) images)
{
    size_t size = m_length(images);
    char *result = malloc(sizeof(int) * size);
    for (size_t i = 0; i < size; i++)
    {
        result[i] = (char) ia_launch_single(NN, images[i]);
    }
    
    return result;
}


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
                output[j] = 1;
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

    Matrix(double) input1 = get_blanck();
    Matrix(double) output1 = m_new(double, 1, NN.layers_sizes[NN.layers_number]);
    output1[0] = 3;
    for (size_t j = 1; j < m_length(output1); j++) {
         output1[j] = 0;
    }

    d.data[0] = To_dataPoint(input1 , output1);

    for (int i = 1; i < d.size; i++)
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

    /*
    int nb_result = 0;
    for (int i = 0; i < d.size; i++) 
    {
        size_t ind = array_max_ind(d.data[i].expect);
        nb_result += Launch(NN, d.data[i].input, ind, 1);
    }
    printf("\nSuccess rate : %f %%\n\n", ((float) nb_result) / ((float) d.size) *100.f);*/
    data_Destroy(d);

}


void train2(neural_network NN, size_t nb_training, size_t nb_sample, Matrix(ImageView) sudo)
{
    //data
    
    Data d = data_init(nb_sample); // a modif


    Matrix(int) labels = m_new(int, 1, m_length(sudo));
    size_t labels_[] = {
        5,3,0,0,7,0,0,0,0,
        6,0,0,1,9,5,0,0,0,
        0,9,8,0,0,0,0,6,0,
        8,0,0,0,6,0,0,0,3,
        4,0,0,8,0,3,0,0,1,
        7,0,0,0,2,0,0,0,6,
        0,6,0,0,0,0,2,8,0,
        0,0,0,4,1,9,0,0,5,
        0,0,0,0,8,0,0,7,9
        };

    for (size_t i = 0; i < 81; i++)
        labels[i] = labels_[i];
    
    for (int i = 0; i < d.size; i++)
    {
        Matrix(double) input = arr_to_mat(sudo[i]);
        uint8_t e = labels[i];
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

    /*
    int nb_result = 0;
    for (int i = 0; i < d.size; i++) 
    {
        size_t ind = array_max_ind(d.data[i].expect);
        nb_result += Launch(NN, d.data[i].input, ind, 1);
    }
    printf("\nSuccess rate : %f %%\n\n", ((float) nb_result) / ((float) d.size) *100.f);*/
    data_Destroy(d);

}
