#pragma once
#include "struct.h"
#include "matrices.h"

DataPoint To_dataPoint(double * input, double * expect, size_t size);

void datapoint_Destroy(DataPoint d);
