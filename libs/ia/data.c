#include "data.h"
#include "matrices.h"
#include <stdio.h>

Data data_init(size_t size) //ini of the data
{
    Data d;
    d.data = malloc(size * sizeof(DataPoint));
    d.size = size;
    return d;
}


DataPoint To_dataPoint(double *input, double *expect, size_t size) // create a datapoint
{

    Matrix(double) m_input = m_new(double, 1, size);
    Matrix(double) m_expect = m_new(double, 1, size);
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


void datapoint_Destroy(DataPoint d) // free datapoint
{
    m_destroy(d.expect);
    m_destroy(d.input);
}

void data_Destroy(Data d) // destroy data
{
    //printf("test %zu", d.size);
    for (size_t i = 0; i < d.size; i++)
        datapoint_Destroy(d.data[i]);
    //free(d.data);
    
}
