#include <iostream>

#include "vector.h"

int main()
{
    VectorDouble vectorDouble;
    ReadVectorFromInput(std::cin, vectorDouble);

    FindAverageAndAddToVectorElements(vectorDouble);

    WriteVectorToOutput(std::cout, vectorDouble);

    return 0;
}

