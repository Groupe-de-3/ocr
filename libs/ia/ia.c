#include "struct.h"
#include "ia.h"
#include "activation.h"

#include <err.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
//---------------
//Layer
//---------------
double* CalculateOutputs_layer(double inputs[], Layer layer)
{
    double output[layer.layer_size];
    
    for (int i = 0; i < layer.layer_size; i++)
    {
        double weightIntput = layer.neural_list[i].bias;

        for (int j = 0; j < layer.layer_size; j++)
        {
            weightIntput += layer.neural_list[i].weights[j] * inputs[i];
        }

        output[i] = weightIntput;
    }

    double activations[layer.layer_size];
    for (int i = 0; i < layer.layer_size; i++)
    {
        activations[i] = Activate(output, i);
    }
    

    return activations;
}

//---------------
// Neural Network
//---------------
double* CalculateOutputs_NN(double inputs[], neural_network NN)
{
    for (int i = 0; i < NN.layers_number; i++)
    {
        inputs = CalculateOutputs_layer(inputs, NN.layers_[i]);
    }
    

    return inputs;
}


// Run the inputs through the network and return the index of the highest input
int Classify(neural_network NN, double inputs[])
{
    double * output = CalculateOutputs_NN(inputs, NN);
    double max = output[0];


    int ind = 0;
    for (int i = 1; i < NN.layers_sizes[NN.layers_number-1]; i++)
    {
        if (max < output[i])
         {
            max = output[i];
            ind = i;
         }
    }

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

*/
