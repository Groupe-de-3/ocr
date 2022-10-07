#include "activation.h"

double Activate(double inputs[], int index)
{
    if (0 > inputs[index])
        return 0;
    return inputs[index];
}

double Derivative(double inputs[], int index)
{
    return (inputs[index] > 0) ? 1 : 0;
}

