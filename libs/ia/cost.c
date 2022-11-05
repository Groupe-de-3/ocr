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



//---------------
// Cost
//---------------

double NodeCost_derivate(double output, double expected)
{
    return 2 * (output - expected);
}

double NodeCost(double output, double expected)
{
    double error = output - expected;
    return error * error;
}

double Cost(Matrix(double) outputs, Matrix(double) expects)
{
    double cost = 0;
    
    for (size_t i = 0; i < m_length(outputs); i++)
    {
        cost += NodeCost(outputs[i], expects[i]);
    }

    m_destroy(outputs);
    
    return cost;
}


