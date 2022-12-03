#include "data.h"
#include "matrices.h"
#include <stdio.h>

Data data_init(size_t size) //ini of the data
{
    Data d;
    d.data = malloc(size * sizeof(DataPoint));
    d.size = (int)size;
    return d;
}


DataPoint To_dataPoint(Matrix(double) input, Matrix(double) expect) // create a datapoint
{
    DataPoint datapoint;
    datapoint.input = input;
    datapoint.expect = expect;

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
    for (int i = 0; i < d.size; i++)
        datapoint_Destroy(d.data[i]);
    free(d.data);
    
}
