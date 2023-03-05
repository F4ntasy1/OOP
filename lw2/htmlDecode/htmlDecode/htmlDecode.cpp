#include <iostream>

#include "htmlDecode.h"

int main()
{
    try
    {
        DecodeInputToOutput(std::cin, std::cout);
    }
    catch (const std::exception& e)
    {
        std::cout << e.what();
        return 1;
    }

    return 0;
}