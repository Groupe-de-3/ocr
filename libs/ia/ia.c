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
Matrix(double) CalculateOutputs_NN(Matrix(double) input, neural_network NN)
{
    Matrix(double) new_m = NULL;

    for (size_t i = 0; i < NN.layers_number; i++)
    {
        new_m = m_new(double, m_width(NN.layers_[i].m_weight), m_height(input));
        m_mul(input, NN.layers_[i].m_weight, new_m);

        m_add(NN.layers_[i].m_bias, new_m);

        Relu_Activate(new_m);

        m_destroy(input);
        input = m_new(double, m_width(new_m), m_height(new_m));
        for (size_t i = 0; i < m_length(new_m); i++)
        {
            input[i] = new_m[i];
        }
        m_destroy(new_m);
    }
    
    return input;
}



void Print_array(Matrix(double) m)
{
    printf("-------------\n");
    for (size_t i = 0; i < m_length(m); i++)
        printf("array %zu :  %f\n", i ,m[i]);
    printf("-------------\n\n");
}

// Run the inputs through the network and return the index of the highest input
size_t Classify(neural_network NN, double input[])
{
    
    Matrix(double) input_ = m_new(double, NN.layers_sizes[0], 1);
    for (size_t i = 0; i < NN.layers_sizes[0]; i++)
    {
        input_[i] = input[i];
    }

    printf("inputs :\n");
    Print_array(input_);
    

    Matrix(double) output = CalculateOutputs_NN(input_, NN);
    size_t ind = 0;
    
    double max = output[0];
    
    for (size_t i = 1; i < m_length(output); i++)
    {
        
        if (max < output[i])
         {
            max = output[i];
            ind = i;
         }
    }

    printf("result :\n");
    Print_array(output);
    m_destroy(output);

    return ind;
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


