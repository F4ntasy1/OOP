#include <iostream>
#include <windows.h>

#include "miniGlossary.h"

int main(int argc, char* argv[])
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    try
    {
        std::string glossaryFileName = 
            Util::GetGlossaryFileNameFromParams(argc, argv);

        WorkWithGlossary::WorkWithGlossaryInDialogMode(std::cin, glossaryFileName);
    }
    catch (const std::exception& e)
    {
        std::cout << e.what();
        return 1;
    }

    return 0;
}