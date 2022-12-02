#pragma once
#include "struct.h"
#include "matrices.h"

Data data_init(int size);

DataPoint To_dataPoint(double *input, double *expect, size_t size);

void data_Destroy(Data data);
