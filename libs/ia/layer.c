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

void get_last_layer_gradientW(Layer layer, Matrix(double) expected, Matrix(double) previous_activated)
{
    // ∇z(L)J = C ⊙ g′(z(l))
    Sigmoid_Derivative(layer.last_output); // g′(z(l))
    Matrix(double) cost_derivate = CostFunction_derivative(layer.last_output_activated, expected); // C
    m_hadamard_product(layer.last_output, cost_derivate);

    printf("%zu,  %zu\n", m_width(cost_derivate), m_height(cost_derivate));
    m_copy(layer.loss, cost_derivate);
    m_mul(cost_derivate, previous_activated, layer.m_gradW);
    //m_copy(layer.m_gradW, cost_derivate);
    //m_copy(layer.m_gradB, cost_derivate);
    m_destroy(cost_derivate);
    
    //printf("test last layer\n");
    //printf("%zu     %zu\n", m_width(layer.m_gradW), m_height(layer.m_gradW));
    
}

void get_hidden_layer_gradientW(Layer layer, Matrix(double) loss_next_layer, Matrix(double) next_weight)
{
    // loss_next_layer : (l+1)J
    // next_weight : W(l+1)
    // layer.last_output : z(l)

    //∇z(l)J = ( W(l+1)T ⋅ ∇z(l+1)J ) ⊙ g′(z(l))

    Matrix(double) transpose_weight = m_new(double, m_height(next_weight), m_width(next_weight));
    m_transpose(next_weight, transpose_weight); // W(l+1)T

    Matrix(double) m_last_output = m_new(double, m_height(layer.last_output), m_width(layer.last_output));
    m_copy(m_last_output, layer.last_output);
    Sigmoid_Derivative(m_last_output); // g′(z(l))

    //Matrix(double) tmp_mat = m_new(double, m_height(layer.last_output), m_width(loss_next_layer));
    
    m_mul(transpose_weight, loss_next_layer, layer.m_gradW); // mult ( W(l+1)T ⋅ ∇z(l+1)J )

    m_hadamard_product(m_last_output, layer.m_gradW); //  ( W(l+1)T ⋅ ∇z(l+1)J ) ⊙ g′(z(l))
    m_hadamard_product(m_last_output, layer.m_gradB); //  ( W(l+1)T ⋅ ∇z(l+1)J ) ⊙ g′(z(l))

    m_destroy(transpose_weight);
    m_destroy(m_last_output);


    //printf("test hidden layer\n");
}
