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
    Matrix(double) new_m = NULL;

    new_m = m_new(double, m_width(layer.m_weight), 1);
    m_mul(input, layer.m_weight, new_m);

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

void ApplyGradients_layer(Layer layer, double learnRate)
{
    
    for (size_t i = 0; i < m_width(layer.m_gradW); i++)
    {
        for (size_t j = 0; j < m_height(layer.m_weight); j++)
        {
            m_get2(layer.m_weight, i, j) -= m_get2(layer.m_gradW, i, 0) * learnRate;
        }
    }

    for (size_t i = 0; i < m_width(layer.m_gradB); i++)
    {
        //double a = m_get2(layer.m_gradB, i, 0) * learnRate;
        //m_get2(layer.m_bias, i, 0) -= a;
    }
}
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

Matrix(double) get_last_layer_gradient(Layer layer, Matrix(double) expected)
{
    Sigmoid_Derivative(layer.last_output); // g′(z(l))
    Matrix(double) cost_derivate = CostFunction_derivative(layer.last_output_activated, expected);
    m_hadamard_product(cost_derivate, layer.last_output);

    m_copy(layer.m_gradW, layer.last_output);
    m_destroy(cost_derivate);
    
    return layer.m_gradW;
    
}

Matrix(double) get_hidden_layer_gradient(Layer layer, Matrix(double) loss_next_layer, Matrix(double) next_weight)
{
    // loss_next_layer : J
    // next_weight : W(l+1)
    // layer.last_output : z(l)

    //∇z(l)J = ( W(l+1)T ⋅ ∇z(l+1)J ) ⊙ g′(z(l))

    Matrix(double) transpose_weight = m_new(double, m_height(next_weight), m_width(next_weight));
    m_transpose(next_weight, transpose_weight); // W(l+1)T

    Sigmoid_Derivative(layer.last_output); // g′(z(l))

    //Matrix(double) tmp_mat = m_new(double, m_height(layer.last_output), m_width(loss_next_layer));
    
    m_mul(transpose_weight, loss_next_layer, layer.m_gradW); // mult ( W(l+1)T ⋅ ∇z(l+1)J )

    m_hadamard_product(layer.m_gradW, layer.last_output); //  ( W(l+1)T ⋅ ∇z(l+1)J ) ⊙ g′(z(l))

    m_destroy(transpose_weight);

    return layer.m_gradW;
}



