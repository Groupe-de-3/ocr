#include "data.h"
#include "matrices.h"
#include <stdio.h>

Data data_init(size_t size)
{
    Data d;
    d.data = malloc(size * sizeof(DataPoint));
    d.size = size;
    return d;
}


DataPoint To_dataPoint(double *input, double *expect, size_t size)
{

    Matrix(double) m_input = m_new(double, size, 1);
    Matrix(double) m_expect = m_new(double, size, 1);
    for (size_t i = 0; i < size; i++)
    {
        m_input[i] = input[i];
        m_expect[i] = expect[i];
    }

    DataPoint datapoint;
    datapoint.input = m_input;
    datapoint.expect = m_expect;

    return datapoint;
    
}

void To_dataPoints(Data d, double **input, double **expect, size_t size)
{
    for (size_t i = 0; i < d.size; i++)
    {
        d.data[i] = To_dataPoint(input[i], expect[i], size);
    }
}

void datapoint_Destroy(DataPoint d)
{
    m_destroy(d.expect);
    //m_destroy(d.input);
}

void data_Destroy(Data d)
{
    for (size_t i = 0; i < d.size; i++)
        datapoint_Destroy(d.data[i]);
    free(d.data);
}
