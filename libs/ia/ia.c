#include "struct.h"
#include "ia.h"
#include "activation.h"

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

// Run the inputs through the network and return the index of the highest input
size_t Classify(neural_network NN, double input[])
{
    
    Matrix(double) input_ = m_new(double, NN.layers_sizes[0], 1); // to Matrix
    for (size_t i = 0; i < NN.layers_sizes[0]; i++)
        input_[i] = input[i];
    
        
    

    printf("inputs :\n"); //print input
    Print_array(input_);
    

    Matrix(double) output = CalculateOutputs_NN(input_, NN); // launch forward
    
    size_t ind = array_max(output); 

    printf("result :\n"); // print result
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


