#pragma once

#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>

using VectorDouble = std::vector<double>;

const int ROUND_FACTOR = 1000;

void ReadVectorFromInput(std::istream& input, VectorDouble& vectorDouble)
{
    double number;

    while (!input.eof() && input >> number)
    {
        vectorDouble.push_back(number);
    }
}

double GetVectorAverage(const VectorDouble& vectorDouble)
{
    double average = 0;

    std::for_each(vectorDouble.begin(), vectorDouble.end(),
        [&average](const double element) {
            if (element > 0)
            {
                average += round(element / 2 * ROUND_FACTOR) / ROUND_FACTOR;
            }
        });

    return average;
}

void AddAverageToVectorElements(VectorDouble& vectorDouble, const double average)
{
    if (average >= 0)
    {
        std::for_each(vectorDouble.begin(), vectorDouble.end(),
            [average](double& element) {
                element += average;
            });
    }
}

void FindAverageAndAddToVectorElements(VectorDouble& vectorDouble)
{
    const double average = GetVectorAverage(vectorDouble);

    AddAverageToVectorElements(vectorDouble, average);
}

void WriteVectorToOutput(std::ostream& output, VectorDouble vectorDouble)
{
    if (!vectorDouble.empty())
    {
        std::sort(vectorDouble.begin(), vectorDouble.end());

        std::copy(vectorDouble.begin(), vectorDouble.end(),
            std::ostream_iterator<double>(output, " "));

        output << std::endl;
    }
}