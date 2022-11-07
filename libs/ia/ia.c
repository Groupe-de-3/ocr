#include "struct.h"
#include "ia.h"
#include "activation.h"
#include "cost.h"
#include "layer.h"

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

size_t array_max(Matrix(double) m) // search index of max element in Matrix
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

size_t Get_result(Matrix(double) output)
{
    size_t ind = array_max(output);

    printf("result : %zu\n", ind); // print result
    Print_array(output);

    m_destroy(output);

    return ind;
}

// Run the inputs through the network and return the output
Matrix(double) Classify(neural_network NN, DataPoint datapoint)
{
    printf("inputs :\n"); //print input
    Print_array(datapoint.input);

    return CalculateOutputs_NN(datapoint.input, NN); // launch forward
}

void Launch(neural_network NN, Data d)
{
    for (size_t i = 0; i < d.size; i++)
    {
        Matrix(double) output = Classify(NN, d.data[i]);
        Get_result(output);
        printf("\n\n");
    }
}


//void CalculOutputLayerNodeValue()


double Aply_cost(neural_network *NN, DataPoint d)
{
    Matrix(double) inputs2 = m_new(double, m_width(d.input), m_height(d.input)); 
    for (size_t i = 0; i < m_length(d.input); i++)
        inputs2[i] = d.input[i];
    
    
    Matrix(double) outputs = CalculateOutputs_NN(inputs2, *NN);
    return Cost(outputs, d.expect);
}

Matrix(double) Learn_layer(neural_network NN, DataPoint datapoint)
{

    Matrix(double) output = get_last_layer_gradient(NN.layers_[NN.layers_number-1], datapoint.expect, datapoint.input);

    for (size_t layer_ind = NN.layers_number - 2; layer_ind > 0; layer_ind++)
    {
        get_hidden_layer_gradient(NN.layers_[layer_ind], datapoint.expect, output);
    }
    

}


void Learn(neural_network *NN, Data data, double learnRate)
{
    for (size_t data_ind = 0; data_ind < data.size; data_ind++)
    {
        DataPoint datapoint = data.data[data_ind];
        double OriginalCost = Aply_cost(NN, datapoint);

        Learn_layer(*NN, datapoint);
    }
}

