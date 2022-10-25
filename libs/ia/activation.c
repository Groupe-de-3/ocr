#include "activation.h"
#include "matrices.h"

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

double Derivative(double inputs[], int index)
{
    return (inputs[index] > 0) ? 1 : 0;
}

