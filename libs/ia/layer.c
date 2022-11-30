#include "struct.h"
#include "activation.h"
#include "cost.h"
#include "layer.h"
#include "matrices.h"

#include <err.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


//---------------
// Layer
//---------------

Matrix(double) CalculateOutputs_Layer(Layer layer, Matrix(double) input, int last)
{
    Matrix(double) new_m = m_new(double, 1, m_height(layer.m_weight));

    m_mul(layer.m_weight, input, new_m);

    m_add(new_m, layer.m_bias);

    m_copy(layer.last_output, new_m);

    if (last)
        Softmax_Activate(new_m); // last layer
    else
        Sigmoid_Activate(new_m); // orther layer

    m_copy(layer.last_output_activated, new_m);

    m_destroy(input);
    
    return new_m;
} 



//---------------
// Gradient
//---------------

/*
void ApplyGradients_layer(Layer layer, double learnRate)
{
    
    for (size_t i = 0; i < m_width(layer.m_gradW); i++)
    {
        for (size_t j = 0; j < m_height(layer.m_gradW); j++)
        {
            m_get2(layer.m_weight, i, j) -= m_get2(layer.m_gradW, i, j) * learnRate;
        }
    }

    for (size_t i = 0; i < m_height(layer.m_gradB); i++)
    {
        m_get2(layer.m_bias, 0, i) -= m_get2(layer.m_gradB, 0, i) * learnRate;
    }
}*/
/*
Matrix(double) get_last_layer_gradient(Layer layer, Matrix(double) expected, Matrix(double) inputs)
{
    Sigmoid_Derivative(inputs);
    CostDerivative(layer.last_output_activated, expected);

    for (size_t i = 0; i < m_length(expected); i++)
    {
        layer.m_gradW[i] = layer.m_weight * expected[i];

    }
} */



void get_hidden_layer_gradient(Layer layer, Matrix(double) inputs, Matrix(double) loss)
{
    // L = loss
    // a[l-1] = input

    // G_W = L * a[l-1]T
    // G_B = L

    Matrix(double) transpose_inputs = m_new(double, m_height(inputs), m_width(inputs));
    m_transpose(inputs, transpose_inputs); // W(l+1)T
    
    m_mul(loss, transpose_inputs, layer.m_gradW);

    m_copy(layer.m_gradB, loss);
    
    m_destroy(transpose_inputs);

    //printf("test hidden layer\n");
}



Matrix(double) get_loss_last_layer(Layer layer, Matrix(double) expects)
{
    Sigmoid_Derivative(layer.last_output_activated); // g′(z(l))
    Matrix(double) m_cost = CostFunction_derivative(layer.last_output, expects);
    //printf("I : %zu     %zu\n", m_width(layer.last_output_activated), m_height(layer.last_output_activated));
    //printf("C : %zu     %zu\n", m_width(m_cost), m_height(m_cost));
    //m_mul(layer.last_output_activated, m_cost, layer.loss);

    m_copy(layer.last_output, m_cost);
    m_destroy(m_cost);
    return layer.last_output;
}


Matrix(double) get_loss(Layer layer, Layer next_layer)
{
    // loss_next_layer : (l+1)J
    // next_weight : W(l+1)
    // layer.last_output : z(l)

    //∇z(l)J = ( W(l+1)T ⋅ ∇z(l+1)J ) ⊙ g′(z(l))
    Matrix(double) transpose_weight = m_new(double, m_height(next_layer.m_weight), m_width(next_layer.m_weight));
    m_transpose(next_layer.m_weight, transpose_weight); // W(l+1)T

    Matrix(double) m_res = m_new(double, m_width(next_layer.last_output), m_height(transpose_weight));
/////////////////////////////////////////*////////////////////*
    m_mul(transpose_weight, next_layer.last_output, m_res); // ( W(l+1)T ⋅ ∇z(l+1)J )
    
    //printf("%zu     %zu\n", m_width(m_res), m_height(m_res));
    Sigmoid_Derivative(layer.last_output); // g′(z(l))
    m_hadamard_product(m_res, layer.last_output); //  ( W(l+1)T ⋅ ∇z(l+1)J ) ⊙ g′(z(l))

    m_destroy(m_res);
    m_destroy(transpose_weight);

    return layer.last_output;
}
