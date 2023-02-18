#include <iostream>
#include <fstream>
#include <string>

void CopyFileFragment(std::istream& input, std::ostream& output, int size)
{
    char ch;

    for (size; size > 0 && input.read((char*)&ch, sizeof(ch)); size--)
    {
        output << ch;
    }

    if (size > 0)
    {
        throw std::invalid_argument("The value of the <fragment size> argument "
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
    std::string inputPath, 
    std::string outputPath)
{
    std::string failedToOpenForReadingMsg = "Failed to open " +
        inputPath + " for reading\n";
    std::string failedToOpenForWritingMsg = "Failed to open " +
        outputPath + " for writing\n";

    try
    {
        input.open(inputPath, std::ios::binary);
    }
    catch (const std::exception&)
    {
        throw std::invalid_argument(failedToOpenForReadingMsg);
    }

    try
    {
        output.open(outputPath, std::ios::binary);
    }
    catch (const std::exception&)
    {
        throw std::invalid_argument(failedToOpenForWritingMsg);
    }

    if (!input.is_open())
    {
        throw std::invalid_argument(failedToOpenForReadingMsg);
    }

    if (!output.is_open())
    {
        throw std::invalid_argument(failedToOpenForWritingMsg);
    }
}

void ValidateParameters(
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

    std::string inputPath = argv[1];
    std::string outputPath = argv[2];

    try
    {
        OpenFiles(input, output, inputPath, outputPath);
    }
    catch (const std::invalid_argument&)
    {
        throw;
    }

    try
    {
        startPosition = std::stoi(argv[3]);
        fragmentSize = std::stoi(argv[4]);
    }
    catch (const std::exception&)
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
    std::ifstream input;
    std::ofstream output;
    int startPosition;
    int fragmentSize;

    try
    {
        ValidateParameters(argc, argv, startPosition, fragmentSize, input, output);
    }
    catch (const std::invalid_argument& e)
    {
        std::cout << e.what();
        return 1;
    }

    try
    {
        CopyFileFragment(input, output, fragmentSize);
    }
    catch (const std::exception& e)
    {
        std::cout << e.what();
        return 1;
    }

    return 0;
}