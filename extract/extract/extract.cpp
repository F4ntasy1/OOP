//50
#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char* argv[])
{
    if (argc != 5)
    {
        std::cout << "Invalid arguments count\n"
            << "Usage: extract.exe <input file> <output file> "
            << "<start position> <fragment size>\n";
        return 1;
    }

    std::ifstream input(argv[1]);
    if (!input.is_open())
    {
        std::cout << "Failed to open " << argv[1] << " for reading\n";
        return 1;
    }

    std::ofstream output(argv[2]);
    if (!output.is_open())
    {
        std::cout << "Failed to open " << argv[2] << " for writing\n";
        return 1;
    }

    int startPos;
    int size;

    try
    {
        startPos = std::stoi(argv[3]);
        size = std::stoi(argv[4]);
    }
    catch (const std::exception & ex)
    {
        std::cout << "Arguments <start position> and <fragment size> "
            << "must be numeric\n";
        return 1;
    }

    if (startPos < 0 || size < 0)
    {
        std::cout << "Arguments <start position> and <fragment size> "
            << "must be greater than or equal to 0\n";
        return 1;
    }

    int count = 0;
    char ch;

    while (count < startPos && input.get(ch))
    {
        count++;
    }

    if (count < startPos)
    {
        std::cout << "The value of the <start position> argument must not "
            << "exceed the length of the file content\n";
        return 1;
    }

    while (size > 0 && input.get(ch))
    {
        if (!output.put(ch))
        {
            std::cout << "Failed to save data on disk\n";
            return 1;
        }

        size--;
    }

    if (size > 0)
    {
        std::cout << "The value of the <fragment size> argument must not "
            << "exceed the length of the file content";
        return 1;
    }

    if (!output.flush())
    {
        std::cout << "Failed to save data on disk\n";
        return 1;
    }

    return 0;
}