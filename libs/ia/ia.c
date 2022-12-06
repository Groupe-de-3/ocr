#include "matrices.h"
#include "struct.h"
#include "ia.h"
#include "activation.h"
#include "cost.h"
#include "layer.h"
#include "image_view.h"

#include <err.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


Matrix(double) arr_to_mat(ImageView img)
{
    Matrix(double) res = m_new(double, 1, 784);
    for(int i = 0; i < 28; i++)
    {
        for(int j = 0; j < 28; j++)
            res[j * 28 + i] = (double) imgv_get_pixel_grayscale(&img, i, j);
    }

    return res;
}

//---------------
// Neural Network Forward
//---------------


Matrix(double) CalculateOutputs_NN(Matrix(double) input, neural_network NN) 
{
    for (size_t i = 0; i < NN.layers_number; i++)
        input = CalculateOutputs_Layer(NN.layers_[i], input, (i == NN.layers_number-1));
    
    return input;
}


void Print_array(Matrix(double) m) // print matrix
{
    printf("-------------\n");
    for (size_t i = 0; i < m_length(m); i++)
        printf("array %zu :  %f\n", i ,m[i]);
    printf("-------------\n\n");
}

size_t array_max_ind(Matrix(double) m) // search index of max element in Matrix
{
    size_t ind = 0;
    double max = 0;
    for (size_t i = 0; i < m_length(m); i++)
    {
        if (max < m[i])
         {
            max = m[i];
            ind = i;
         }
    }

    return ind;
}

double array_max_val(Matrix(double) m) // search index of max element in Matrix
{
    double max = 0;
    for (size_t i = 0; i < m_length(m); i++)
    {
        if (max < m[i])
            max = m[i];
    }
    return max;
}

int Get_result(Matrix(double) output, size_t expected, int show)
{
    size_t ind = array_max_ind(output);
    double val = array_max_val(output);
    if (show == 1)
    {        
        if (ind == expected)
            printf("Success : %zu with %f %% \n", ind, val*100); // print result
        else
            printf("Failed : %zu != %zu  ( %f %% )\n", ind, expected, val*100); // print result
    }
    return ind == expected;
}

// Run the inputs through the network and return the output
Matrix(double) Classify(neural_network NN, Matrix(double) input)
{
    Matrix(double) input_ = m_new(double, 1, m_height(input));
    m_copy(input_, input);

    return CalculateOutputs_NN(input_, NN); // launch forward
}

int Launch(neural_network NN, Matrix(double) input, size_t expected, int show) // Start the IA
{
    Matrix(double) output = Classify(NN, input);
    int res = Get_result(output, expected, show);
    
    size_t ind = array_max_ind(output);
    m_destroy(output);

    if (show == 2) // for real pass
        return ind;
    return res;
}


void ApplyGradients_all_layer(neural_network NN, Matrix(double) *grad_W_train, Matrix(double) *grad_B_train, double learnRate, int size_data)
{
    for (size_t ind_layer = 0; ind_layer < NN.layers_number; ind_layer++) 
    {
        Layer layer = NN.layers_[ind_layer];

        for (size_t i = 0; i < m_width(layer.m_weight); i++)
        {
            for (size_t j = 0; j < m_height(layer.m_weight); j++)
                m_get2(layer.m_weight, i, j) -= m_get2(grad_W_train[ind_layer], i, j) * learnRate /size_data;
        }

        for (size_t i = 0; i < m_height(layer.m_bias); i++)
            m_get2(layer.m_bias, 0, i) -= m_get2(grad_B_train[ind_layer], 0, i) * learnRate /size_data;
    }

}


void ApplyGradients_layer_(Layer layer, double learnRate)
{
    for (size_t i = 0; i < m_width(layer.m_weight); i++)
    {
        for (size_t j = 0; j < m_height(layer.m_weight); j++)
            m_get2(layer.m_weight, i, j) -= m_get2(layer.m_gradW, i, j) * learnRate;
    }

    for (size_t i = 0; i < m_height(layer.m_bias); i++)
        m_get2(layer.m_bias, 0, i) -= m_get2(layer.m_gradB , 0, i) * learnRate;
}

void Calcul_gradient_layer(neural_network NN, DataPoint datapoint)
{

    //last layer
    get_loss_last_layer(NN.layers_[NN.layers_number-1], datapoint.expect);
    get_layer_gradient(NN.layers_[NN.layers_number-1], NN.layers_[NN.layers_number-2].last_output_activated);
    
    for (int layer_ind = (int) NN.layers_number - 2; layer_ind > 0; layer_ind--)
    {
        // middle layers
        get_loss_hidden_layer(NN.layers_[layer_ind], NN.layers_[layer_ind+1]);
        get_layer_gradient(NN.layers_[layer_ind], NN.layers_[layer_ind-1].last_output_activated);
    }
    
    // first layer
    get_loss_hidden_layer(NN.layers_[0], NN.layers_[1]);
    get_layer_gradient(NN.layers_[0], datapoint.input);

}

void Learn(neural_network *NN, Data data, double learnRate) // Start the learning
{
    Matrix(double) *grad_W_train = malloc(sizeof(Matrix(double)) * NN->layers_number); //list of the list of grad
    Matrix(double) *grad_B_train = malloc(sizeof(Matrix(double)) * NN->layers_number);
    for (size_t i = 0; i < NN->layers_number; i++)
    {
        grad_W_train[i] = m_new(double, NN->layers_sizes[i], NN->layers_sizes[i+1]);
        grad_B_train[i] = m_new(double, 1, NN->layers_sizes[i+1]);
        memset(grad_W_train[i], 0, sizeof(double) * m_length(grad_W_train[i]));
        memset(grad_B_train[i], 0, sizeof(double) * m_length(grad_B_train[i]));
    }

    for (int data_ind = 0; data_ind < data.size; data_ind++)
    {
        DataPoint datapoint = data.data[data_ind];
        Launch(*NN, datapoint.input, 0, 0);
        Calcul_gradient_layer(*NN, datapoint);
        
        for (size_t i = 0; i < NN->layers_number; i++)
        {
            //ApplyGradients_layer_(NN->layers_[i], learnRate);
        }
        
        
        for (size_t i = 0; i < NN->layers_number; i++)
        {
            m_add(grad_B_train[i], NN->layers_[i].m_gradB);
            m_add(grad_W_train[i], NN->layers_[i].m_gradW);
        }
    }

    ApplyGradients_all_layer(*NN, grad_W_train, grad_B_train, learnRate, data.size); // aply gradient to all layer after all data

    for (size_t i = 0; i < NN->layers_number; i++)
    {
        m_destroy(grad_W_train[i]);
        m_destroy(grad_B_train[i]);
    }

    free(grad_B_train);
    free(grad_W_train);
}

