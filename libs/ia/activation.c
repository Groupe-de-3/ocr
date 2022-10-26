#include "activation.h"
#include "matrices.h"
#include <math.h>

double single_relu_Activate(double val)
{
    if (0 > val)
        return 0;
    return val;
}

void Relu_Activate(Matrix(double) m)
{
    for (size_t i = 0; i < m_length(m); i++)
    {
        m[i] = single_relu_Activate(m[i]);
    }
}

double single_relu_Derivative(double val)
{
    return (val > 0) ? 1 : 0;
}

void Relu_Derivative(Matrix(double) m)
{
    for (size_t i = 0; i < m_length(m); i++)
    {
        m[i] = single_relu_Derivative(m[i]);
    }
}

void Softmax_Activate(Matrix(double) inputs)
{
    double expSum = 0;
    for (size_t i = 0; i < m_length(inputs); i++)
        expSum += exp(inputs[i]);
    

    for (size_t i = 0; i < m_length(inputs); i++)
        inputs[i] = exp(inputs[i]) / expSum;
    
}

void Softmax_Derivative(Matrix(double) inputs)
{
    double expSum = 0;
    for (size_t i = 0; i < m_length(inputs); i++)
        expSum += exp(inputs[i]);
    

    for (size_t i = 0; i < m_length(inputs); i++)
    {
        double ex = exp(inputs[i]);
        inputs[i] = (ex * expSum - ex * ex) / (expSum * expSum);
    }
}


