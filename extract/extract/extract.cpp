#include <iostream>
#include <fstream>
#include <string>

void copyFileFragment(std::istream& input, std::ostream& output, int size)
{
    char ch;

    for (size; size > 0 && input.get(ch); size--)
    {
        if (!output.put(ch))
        {
            throw std::invalid_argument("Failed to save data on file\n");
        }
    }

    if (size > 0)
    {
        throw std::invalid_argument("The value of the <fragment size> argument "
            "must not exceed the length of the file content\n");
    }

    if (!output.flush())
    {
        throw std::invalid_argument("Failed to save data on disk\n");
    }
}

void validateParameters(
    int argc, 
    char* argv[], 
    int& startPosition, 
    int& fragmentSize, 
    std::ifstream& input, 
    std::ofstream& output)
{
    if (argc != 5)
    {
        throw std::invalid_argument("Invalid arguments count\n"
            "Usage: extract.exe <input file> <output file> "
            "<start position> <fragment size>\n");
    }

    if (!input.is_open())
    {
        std::cout << "Failed to open " << argv[1] << " for reading\n";
        throw std::invalid_argument("");
    }

    if (!output.is_open())
    {
        std::cout << "Failed to open " << argv[2] << " for writing\n";
        throw std::invalid_argument("");
    }

    try
    {
        startPosition = std::stoi(argv[3]);
        fragmentSize = std::stoi(argv[4]);
    }
    catch (const std::exception)
    {
        throw std::invalid_argument("Arguments <start position> and "
            "fragment size> must be numeric\n");
    }

    if (startPosition < 0 || fragmentSize < 0)
    {
        throw std::invalid_argument("Arguments <start position> and "
            "fragment size> must be greater than or equal to 0\n");
    }

    int count = 0;
    char ch;
    while (count < startPosition && input.get(ch))
    {
        count++;
    }

    if (count < startPosition)
    {
        throw std::invalid_argument("The value of the <start position> "
            "argument must not exceed the length of the file content\n");
    }
}

int main(int argc, char* argv[])
{
    std::ifstream input(argv[1]);
    std::ofstream output(argv[2]);
    int startPosition;
    int fragmentSize;

    try
    {
        validateParameters(argc, argv, startPosition, fragmentSize, input, output);
    }
    catch (const std::invalid_argument& e)
    {
        std::cout << e.what();
        return 1;
    }

    try
    {
        copyFileFragment(input, output, fragmentSize);
    }
    catch (const std::invalid_argument& e)
    {
        std::cout << e.what();
        return 1;
    }

    return 0;
}