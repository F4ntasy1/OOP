#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>

using VectorFloat = std::vector<float>;

const int DIVIDER = 2;
const int ROUND_FACTOR = 1000;

void ReadVectorFromInput(std::istream& input, VectorFloat& vectorFloat)
{
    float number;

    while (input >> number)
    {
        vectorFloat.push_back(number);
    }
}

float GetVectorAverage(const VectorFloat& vectorFloat)
{
    float average = 0;

    std::for_each(vectorFloat.begin(), vectorFloat.end(), 
        [&average](const float element) {
        if (element > 0)
        {
            average += round(element / DIVIDER * ROUND_FACTOR) / ROUND_FACTOR;
        }
    });

    return average;
}

void AddAverageToVectorElements(VectorFloat& vectorFloat, const float& average)
{
    std::for_each(vectorFloat.begin(), vectorFloat.end(), [average](float& element) {
        element += average;
    });
}

void VectorProcessing(VectorFloat& vectorFloat)
{
    const float average = GetVectorAverage(vectorFloat);

    AddAverageToVectorElements(vectorFloat, average);
}

void WriteVectorInOutput(std::ostream& output, VectorFloat vectorFloat)
{
    std::sort(vectorFloat.begin(), vectorFloat.end());
    std::copy(vectorFloat.begin(), vectorFloat.end(), 
        std::ostream_iterator<float>(output, " "));
}

int main()
{
    VectorFloat vectorFloat;

    ReadVectorFromInput(std::cin, vectorFloat);

    VectorProcessing(vectorFloat);

    WriteVectorInOutput(std::cout, vectorFloat);

    return 0;
}

