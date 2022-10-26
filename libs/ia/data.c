#include "data.h"
#include "matrices.h"

DataPoint To_dataPoint(double * input, double * expect, size_t size)
{
    Matrix(double) m_input = m_new(double, size, 0);
    Matrix(double) m_expect = m_new(double, size, 0); 
    for (size_t i = 0; i < size; i++)
    {
        m_input[i] = input[i];
        m_expect[i] = expect[i];
    }

    DataPoint d;
    d.input = m_input;
    d.expect = m_expect;
    
    return d;
}

void datapoint_Destroy(DataPoint d)
{
    free(d.expect);
    free(d.input);
}

