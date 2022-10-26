#include "struct.h"
#include "ia.h"
#include "activation.h"
#include "cost.h"

#include <err.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


//---------------
// Neural Network Forward
//---------------
Matrix(double) CalculateOutputs_NN(Matrix(double) input, neural_network NN)
{
    Matrix(double) new_m = NULL;

    for (size_t i = 0; i < NN.layers_number; i++)
    {
        new_m = m_new(double, m_width(NN.layers_[i].m_weight), m_height(input));
        m_mul(input, NN.layers_[i].m_weight, new_m);

        m_add(new_m, NN.layers_[i].m_bias);

        if (i < NN.layers_number-1)
            Relu_Activate(new_m);
        else
            Softmax_Activate(new_m);

        m_destroy(input);
        input = m_new(double, m_width(new_m), m_height(new_m));
        for (size_t j = 0; j < m_length(new_m); j++)
        {
            input[j] = new_m[j];
        }
        m_destroy(new_m);
    }
    
    return input;
}



void Print_array(Matrix(double) m) // print matrix
{
    printf("-------------\n");
    for (size_t i = 0; i < m_length(m); i++)
        printf("array %zu :  %f\n", i ,m[i]);
    printf("-------------\n\n");
}

size_t array_max(Matrix(double) m) // search index of max element in Matrix
{
    size_t ind = 0;
    double max = 0;
    for (size_t i = 1; i < m_length(m); i++)
    {
        if (max < m[i])
         {
            max = m[i];
            ind = i;
         }
    }

    return ind;
}

// Run the inputs through the network and return the output
Matrix(double) Classify(neural_network NN, double input[])
{
    
    Matrix(double) input_ = m_new(double, NN.layers_sizes[0], 1); // to Matrix
    for (size_t i = 0; i < NN.layers_sizes[0]; i++)
        input_[i] = input[i];
    

    printf("inputs :\n"); //print input
    Print_array(input_);
    

    return CalculateOutputs_NN(input_, NN); // launch forward
}

size_t Get_result(Matrix(double) output)
{
    size_t ind = array_max(output); 

    printf("result : %zu\n", ind); // print result
    Print_array(output);

    m_destroy(output);

    return ind;
}

void Launch(neural_network NN, double input[])
{
    Matrix(double) output = Classify(NN, input);

    Get_result(output);
}


//---------------
// Gradient
//---------------

void ApplyGradients(Layer layer, Matrix(double) costB, Matrix(double) costW, double learnRate)
{
    for (size_t i = 0; i < m_height(costW); i++)
    {
        m_get2(layer.m_bias, i, 0) -= m_get2(costB, i, 0) * learnRate;
        for (size_t j = 0; j < m_width(costW); j++)
        {
            m_get2(layer.m_weight, i, j) -= m_get2(costW, i, j) * learnRate;;
        }     
    }
    
}
/*
void Learn(neural_network *NN, DataPoint datapoint, double learnRate)
{
    double h = 0.01;
    //double OriginalCost = Cost();

    for (size_t layer_ind = 0; layer_ind < NN->layers_number; layer_ind++)
    {
        Matrix(double) costW = m_new(double, NN->layers_[layer_ind].layer_size, NN->layers_sizes[layer_ind]);

        for (size_t i = 0; i < m_height(costW); i++)
        {
            for (size_t j = 0; j < m_width(costW); j++)
            {
                m_get2(NN->layers_[layer_ind].m_weight, i, j) += h;
                double deltaCost = Aply_cost(NN, input_, expected);
                m_get2(NN->layers_[layer_ind].m_weight, i, j) -= h;
                //m_get2(costW, i, j) = deltaCost / h;
            }     
        }

        Matrix(double) costB = m_new(double, NN->layers_[layer_ind].layer_size, 0);

        for (size_t i = 0; i < m_height(costW); i++)
        {
            m_get2(NN->layers_[layer_ind].m_weight, i, 0) += h;
            double deltaCost = Aply_cost()
            m_get2(NN->layers_[layer_ind].m_weight, i, 0) -= h;
            //m_get2(costW, i, 0) = deltaCost / h;
              
        }

        ApplyGradients(NN->layers_[layer_ind], costB, costW, 0.1);

        m_destroy(costW);
        m_destroy(costB);   
    }

}

double Aply_cost(neural_network NN, Matrix(double) inputs, Matrix(double) expects)
{
    Matrix(double) outputs = CalculateOutputs_NN(inputs, NN);
    return Cost(outputs, expects);
}
*/


//---------------
// Cost
//---------------

double NodeCost_derivate(double output, double expected)
{
    return 2 * (output - expected);
}

double NodeCost(double output, double expected)
{
    double error = output - expected;
    return error * error;
}

double Cost(Matrix(double) outputs, Matrix(double) expects)
{
    double cost = 0;
    
    for (size_t i = 0; i < m_length(outputs); i++)
    {
        cost += NodeCost(outputs[i], expects[i]);
    }
    
    return cost;
}


