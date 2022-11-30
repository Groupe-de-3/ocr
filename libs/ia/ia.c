#include "matrices.h"
#include "struct.h"
#include "ia.h"
#include "activation.h"
#include "cost.h"
#include "layer.h"

#include <err.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


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

size_t Get_result(Matrix(double) output)
{
    size_t ind = array_max_ind(output);
    double val = array_max_val(output);
    Print_array(output);
    printf("result : %zu with %f %% \n", ind, val*100); // print result
    //m_destroy(output);

    return ind;
}

// Run the inputs through the network and return the output
Matrix(double) Classify(neural_network NN, Matrix(double) input)
{
    //printf("inputs :\n"); //print input
    //Print_array(input);
    Matrix(double) input_ = m_new(double, 1, m_height(input));

    return CalculateOutputs_NN(input_, NN); // launch forward
}

void Launch(neural_network NN, Data d) // Start the IA
{
    for (size_t i = 0; i < d.size; i++)
    {
        Matrix(double) output = Classify(NN, d.data[i].input);
        Get_result(output);
        m_destroy(output);
        printf("\n\n");
    }
}


double Aply_cost(neural_network *NN, DataPoint d)
{
    Matrix(double) inputs2 = m_new(double, m_width(d.input), m_height(d.input)); 
    for (size_t i = 0; i < m_length(d.input); i++)
        inputs2[i] = d.input[i];
    
    
    Matrix(double) outputs = CalculateOutputs_NN(inputs2, *NN);
    return Cost(outputs, d.expect);
}

void Calcul_gradient_layer(neural_network NN, DataPoint datapoint)
{
    get_last_layer_gradientW(NN.layers_[NN.layers_number-1], datapoint.expect, NN.layers_[NN.layers_number-1].last_output_activated);
    //get_last_layer_gradientB(NN.layers_[NN.layers_number-1], datapoint.expect);
    
    for (int layer_ind = (int) NN.layers_number - 2; layer_ind >= 0; layer_ind--)
    {
        get_hidden_layer_gradientW(NN.layers_[layer_ind], NN.layers_[layer_ind+1].m_gradW, NN.layers_[layer_ind +1].m_weight);
        //get_hidden_layer_gradientB(NN.layers_[layer_ind], NN.layers_[layer_ind+1].m_gradB, NN.layers_[layer_ind +1].m_bias);
    }
}

void ApplyGradients_all_layer(neural_network NN, Matrix(double) *grad_W_train, Matrix(double) *grad_B_train, double learnRate)
{
    for (size_t ind_layer = 0; ind_layer < NN.layers_number; ind_layer++) 
    {
        Layer layer = NN.layers_[ind_layer];

        for (size_t i = 0; i < m_width(layer.m_weight); i++)
        {
            for (size_t j = 0; j < m_height(layer.m_weight); j++)
            {
                //Print_array(grad_W_train[ind_layer]);
                m_get2(layer.m_weight, i, j) -= m_get2(grad_W_train[ind_layer], 0, j) * learnRate;
            }
        }

        for (size_t i = 0; i < m_height(layer.m_bias); i++)
        {
            //Print_array(grad_B_train[ind_layer]);
            m_get2(layer.m_bias, 0, i) -= m_get2(grad_B_train[ind_layer], 0, i) * learnRate;
        }
    }

}


void Learn(neural_network *NN, Data data, double learnRate) // Start the learning
{
    Matrix(double) *grad_W_train = malloc(sizeof(Matrix(double)) * data.size); //list of the list of grad
    Matrix(double) *grad_B_train = malloc(sizeof(Matrix(double)) * data.size);
    for (size_t i = 0; i < NN->layers_number; i++)
    {
        grad_W_train[i] = m_new(double, 1, NN->layers_sizes[i+1]);
        grad_B_train[i] = m_new(double, 1, NN->layers_sizes[i+1]);
        memset(grad_W_train[i], 0, sizeof(double) * m_length(grad_W_train[i]));
        memset(grad_B_train[i], 0, sizeof(double) * m_length(grad_B_train[i]));
    }


    for (size_t data_ind = 0; data_ind < data.size; data_ind++)
    {
        DataPoint datapoint = data.data[data_ind];
        Calcul_gradient_layer(*NN, datapoint);
        
        for (size_t i = 0; i < NN->layers_number; i++)
        {
            m_add(grad_B_train[i], NN->layers_[i].m_gradB);
            m_add(grad_W_train[i], NN->layers_[i].m_gradW);
        }
    }

    ApplyGradients_all_layer(*NN, grad_W_train, grad_B_train, learnRate);


    for (size_t i = 0; i < NN->layers_number; i++)
    {
        m_destroy(grad_W_train[i]);
        m_destroy(grad_B_train[i]);
    }

    free(grad_B_train);
    free(grad_W_train);
}

