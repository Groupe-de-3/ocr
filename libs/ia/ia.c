#include "struct.h"
#include "ia.h"
#include "activation.h"

#include <err.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


//---------------
// Neural Network
//---------------
Matrix(double)* CalculateOutputs_NN(Matrix(double) input, neural_network NN)
{
    Matrix(double) new_m = NULL;

    for (size_t i = 0; i < NN.layers_number; i++)
    {
        new_m = m_new(double, m_height(input), m_width(NN.layers_[i].m_weight));
        m_mul(NN.layers_[i].m_weight, input, new_m);

        m_add(NN.layers_[i].m_bias, new_m);

        Relu_Activate(&new_m);

        m_destroy(input);
        m_copy(&input, new_m);
        m_destroy(new_m);
    }
    
    return &new_m;
}


// Run the inputs through the network and return the index of the highest input
size_t Classify(neural_network NN, double inputs[])
{
    /*
    double * output;
    double max = output[0];


    size_t ind = 0;
    for (size_t i = 1; i < NN.layers_sizes[NN.layers_number-1]; i++)
    {
        if (max < output[i])
         {
            max = output[i];
            ind = i;
         }
    }

    return ind;*/
}


//---------------
// Cost
//---------------

double NodeCost(double output, double expected)
{
    double error = output - expected;
    return error * error;
}

double Cost(double* outputs, double* expects, int size)
{
    double cost = 0;
    
    for (int i = 0; i < size; i++)
    {
        cost += NodeCost(outputs[i], expects[i]);
    }
    
    
    return cost;
}

double Average_Cost(double** outputs, double** expects, int size)
{
    double cost = 0;
    
    for (int i = 0; i < size; i++)
    {
        cost += Cost(outputs[i], expects[i], size);
    }
    
    
    return cost / size;
} 


