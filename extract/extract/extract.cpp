#include <iostream>
#include <fstream>
#include <string>

struct Params {
    std::string inputPath;
    std::string outputPath;
    int startPosition;
    int fragmentSize;
};

void CopyFileFragment(
    std::istream& input, 
    std::ostream& output, 
    const int startPosition, 
    int fragmentSize)
{
    int count = 0;
    char ch;
    for (count; count < startPosition && input.get(ch); count++) {}

    if (count < startPosition)
    {
        throw std::runtime_error("The value of the <start position> "
            "argument must not exceed the length of the file content\n");
    }

    for (fragmentSize; fragmentSize > 0 && 
        input.read((char*)&ch, sizeof(ch)); fragmentSize--)
    {
        output << ch;
    }

    if (fragmentSize > 0)
    {
        throw std::runtime_error("The value of the <fragment size> argument "
            "must not exceed the length of the file content\n");
    }

    if (!output.flush())
    {
        throw std::runtime_error("Failed to save data on disk\n");
    }
}

void OpenFiles(
    std::ifstream& input,
    std::ofstream& output,
    const std::string& inputPath, 
    const std::string& outputPath)
{
    input.open(inputPath, std::ios::binary);
    output.open(outputPath, std::ios::binary);

    if (!input.is_open())
    {
        throw std::runtime_error("Failed to open " +
            inputPath + " for reading\n");
    }

    if (!output.is_open())
    {
        throw std::runtime_error("Failed to open " +
            outputPath + " for writing\n");
    }
}

Params ParseParameters(int argc, char* argv[])
{
    if (argc != 5)
    {
        throw std::runtime_error("Invalid arguments count\n"
            "Usage: extract.exe <input file> <output file> "
            "<start position> <fragment size>\n");
    }

    Params params;

    params.inputPath = argv[1];
    params.outputPath = argv[2];

    try
    {
        params.startPosition = std::stoi(argv[3]);
        params.fragmentSize = std::stoi(argv[4]);
    }
    catch (const std::exception&)
    {
        throw std::runtime_error("Arguments <start position> and "
            "fragment size> must be numeric\n");
    }

    if (params.startPosition < 0 || params.fragmentSize < 0)
    {
        throw std::runtime_error("Arguments <start position> and "
            "fragment size> must be greater than or equal to 0\n");
    }

    return params;
}

int main(int argc, char* argv[])
{
    std::ifstream input;
    std::ofstream output;

    Params params;

    try
    {
        params = ParseParameters(argc, argv);

        OpenFiles(input, output, params.inputPath, params.outputPath);

        CopyFileFragment(
            input, output, params.startPosition, params.fragmentSize
        );
    }
    catch (const std::exception& e)
    {
        std::cout << e.what();
        return 1;
    }

    return 0;
}