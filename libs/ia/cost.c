#include "cost.h"

double CostFunction(Matrix(double) predictedOutputs, Matrix(double) expectedOutputs)
{
    double cost = 0;
    for (size_t i = 0; i < m_length(predictedOutputs); i++)
    {
        double error = predictedOutputs[i] - expectedOutputs[i];
        cost += error * error;
    }
    return 0.5 * cost;
}

double CostDerivative(double predictedOutput, double expectedOutput)
{
    return predictedOutput - expectedOutput;
}
