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

void ia_launch(neural_network NN, Matrix(bool) images_mask, Matrix(ImageView) images, char * sudo)
{

    for (size_t i = 0; i < 81; i++)
    {
        if (!images_mask[i])
            continue;
        sudo[i] = (char) ia_launch_single(NN, images[i]);
    }

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


    for (int i = 0; i < d.size; i++)
    {
        unsigned long ind_pic = ((unsigned long)rand()%mnist.size);
        Matrix(double) input = arr_to_mat(imgv_default(&mnist.images[ind_pic]));
        uint8_t e = mnist.labels[ind_pic];
        Matrix(double) output = m_new(double, 1, NN.layers_sizes[NN.layers_number]);
        for (size_t j = 0; j < m_length(output); j++) {
            if (e == j)
                output[j] = 8;
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


void train2(neural_network NN, size_t nb_training, coople ret)
{
    
    //data
    size_t c = 0;
    for (int i = 0; i < 81; i++)
        c += ret.sudoku_mask[i];

    Data d = data_init(c); // a modif


    Matrix(size_t) labels = m_new(size_t, 1, c);
    //size_t labels_[] = {5,3,7,6,1,9,5,9,8,6,8,6,3,4,8,3,1,7,2,6,6,2,8,4,1,9,5,8,7,9};
    size_t labels_[] = {4,5,8,7,2,1,3,4,3,2,1,6,7,4,7,2,6,3,4,9,1,3,6,1,5,8,6,6,9,5};

    for (size_t i = 0; i < c; i++)
        labels[i] = labels_[i];


    int n = 0;
    
    for (int i = 0; i < 81; i++)
    {
        if (ret.sudoku_mask[i])
        {
            Matrix(double) input = arr_to_mat(ret.sudoku_imgs[i]);
            size_t e = labels[n];
            Matrix(double) output = m_new(double, 1, NN.layers_sizes[NN.layers_number]);
            for (size_t j = 0; j < m_length(output); j++) {
                if (e == j)
                    output[j] = 5;
                else
                    output[j] = 0;
            }
            DataPoint dp = To_dataPoint(input , output);
            d.data[n] = dp;
            n++;
        }   
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
