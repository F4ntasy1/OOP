#include <iostream>

#include "vector.h"

int main()
{
    VectorFloat vectorFloat;
    ReadVectorFromInput(std::cin, vectorFloat);

    VectorProcessing(vectorFloat);

    WriteVectorInOutput(std::cout, vectorFloat);

    return 0;
}

