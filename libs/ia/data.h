#pragma once
#include "struct.h"
#include "matrices.h"

Data data_init(size_t size);

DataPoint To_dataPoint(Matrix(double) input, Matrix(double) expect);

void data_Destroy(Data data);
