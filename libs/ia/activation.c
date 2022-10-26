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

double single_relu_Derivative(double inputs[], int index)
{
    return (inputs[index] > 0) ? 1 : 0;
}

void Softmax_Activate(Matrix(double) inputs)
{
    double expSum = 0;
    for (size_t i = 0; i < m_length(inputs); i++)
        expSum += exp(inputs[i]);
    

    for (size_t i = 0; i < m_length(inputs); i++)
        inputs[i] = exp(inputs[i]) / expSum;
    
}

