#include "struct.h"
#include "activation.h"
#include "cost.h"
#include "layer.h"

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
