#pragma once

#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>

using VectorFloat = std::vector<double>;

const int DIVIDER = 2;
const int ROUND_FACTOR = 1000;

void ReadVectorFromInput(std::istream& input, VectorFloat& vectorFloat)
{
    double number;

    while (!input.eof() && input >> number)
    {
        vectorFloat.push_back(number);
    }
}

double GetVectorAverage(const VectorFloat& vectorFloat)
{
    double average = 0;

    std::for_each(vectorFloat.begin(), vectorFloat.end(),
        [&average](const double element) {
            if (element > 0)
            {
                average += round(element / DIVIDER * ROUND_FACTOR) / ROUND_FACTOR;
            }
        });

    return average;
}

void AddAverageToVectorElements(VectorFloat& vectorFloat, const double average)
{
    if (average < 0)
    {
        return;
    }

    std::for_each(vectorFloat.begin(), vectorFloat.end(), [average](double& element) {
        element += average;
    });
}

void VectorProcessing(VectorFloat& vectorFloat)
{
    const double average = GetVectorAverage(vectorFloat);

    AddAverageToVectorElements(vectorFloat, average);
}

void WriteVectorInOutput(std::ostream& output, VectorFloat vectorFloat)
{
    if (vectorFloat.empty())
    {
        return;
    }

    std::sort(vectorFloat.begin(), vectorFloat.end());

    std::copy(vectorFloat.begin(), vectorFloat.end(),
        std::ostream_iterator<double>(output, " "));
    output << std::endl;
}