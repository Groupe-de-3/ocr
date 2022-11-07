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

void ApplyGradients_layer(Layer layer, Matrix(double) costB, Matrix(double) costW, double learnRate)
{

    for (size_t i = 0; i < m_width(costW); i++)
    {
        for (size_t j = 0; j < m_height(costW); j++)
        {
            m_get2(layer.m_weight, i, j) -= m_get2(costW, i, j) * learnRate;;
        }
    }

    for (size_t i = 0; i < m_width(costB); i++)
    {
        double a = m_get2(costB, i, 0) * learnRate;
        m_get2(layer.m_bias, i, 0) -= a;
    }
}

Matrix(double) get_last_layer_gradient(Layer layer, Matrix(double) expected, Matrix(double) inputs)
{
    //Matrix(double) transpose_inputs = m_new(double, m_height(inputs), m_width(inputs));
    //m_transpose(inputs, transpose_inputs); //3t
    /*
    Sigmoid_Derivative(layer.last_output); //2

    Matrix(double) tmp_mat = m_new(double, m_height(layer.last_output), m_width(transpose_inputs));
    
    m_mul(layer.last_output, transpose_inputs, tmp_mat); //mult 2 * 3
    
    Matrix(double) cost_mnt = CostFunction_derivative(layer.last_output_activated ,expected); // 1
    // cost_mnt = dJ / dO
    // cost_mnt * dO / dOw = cost_mnt * actDeriv(Ow)
    Matrix(double) act_deriv = Sigmoid_Derivative(layer.last_output);

    Matrix(double) weight_for_cost_deriv = m_new(double, m_height(cost_mnt), m_width(act_deriv));
    m_mul(cost_mnt, act_deriv, weight_for_cost_deriv);*/
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

    Matrix(double) tmp_mat = m_new(double, m_height(layer.last_output), m_width(loss_next_layer));
    
    m_mul(transpose_weight, loss_next_layer, tmp_mat); // mult ( W(l+1)T ⋅ ∇z(l+1)J )

    m_hadamard_product(tmp_mat, layer.last_output); //  ( W(l+1)T ⋅ ∇z(l+1)J ) ⊙ g′(z(l))

    m_destroy(transpose_weight);
    m_destroy(tmp_mat);

    return layer.last_output;
}



