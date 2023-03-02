#include <iostream>
#include <vector>

#include "vector.h"

int main()
{
    try
    {
        VectorFloat vectorFloat;
        ReadVectorFromInput(std::cin, vectorFloat);

        VectorProcessing(vectorFloat);

        WriteVectorInOutput(std::cout, vectorFloat);
    }
    catch (const std::exception& e)
    {
        std::cout << e.what();
        return 1;
    }

    return 0;
}

