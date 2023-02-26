#include <iostream>
#include <fstream>
#include <string>

const auto ACTION_CRYPT = "crypt";
const auto ACTION_DECRYPT = "decrypt";

const int SHIFT_LEFT_BY_TWO = 2;
const int SHIFT_LEFT_BY_THREE = 3;
const int SHIFT_RIGHT_BY_TWO = 2;
const int SHIFT_RIGHT_BY_FIVE = 5;

const int PLOT_OF_BITS_TO_SHIFT_LEFT_BY_TWO = 0b00011100;
const int PLOT_OF_BITS_TO_SHIFT_LEFT_BY_THREE = 0b11000000;
const int PLOT_OF_BITS_TO_SHIFT_RIGHT_BY_TWO = 0b00100000;
const int PLOT_OF_BITS_TO_SHIFT_RIGHT_BY_FIVE = 0b00000011;

struct Params {
    std::string action;
    std::string inputPath;
    std::string outputPath;
    int key;
};

void Crypt(std::istream& input, std::ostream& output, const int& key)
{
    const int CHAR_SIZE = sizeof(char);

    char byte;
    while (input.read(&byte, CHAR_SIZE))
    {
        int byteCryptByKey = byte ^ key;

        output << (
            ((byteCryptByKey << SHIFT_LEFT_BY_TWO) & 
                PLOT_OF_BITS_TO_SHIFT_LEFT_BY_TWO) |
            ((byteCryptByKey >> SHIFT_RIGHT_BY_FIVE) & 
                PLOT_OF_BITS_TO_SHIFT_RIGHT_BY_FIVE) |
            ((byteCryptByKey << SHIFT_LEFT_BY_THREE) & 
                PLOT_OF_BITS_TO_SHIFT_LEFT_BY_THREE) |
            ((byteCryptByKey >> SHIFT_RIGHT_BY_TWO) & 
                PLOT_OF_BITS_TO_SHIFT_RIGHT_BY_TWO)
        ) << ' ';
    }
}

void Decrypt(std::istream& input, std::ostream& output, const int& key)
{
    int byte;
    while (input >> byte)
    {
        int byteDecrypt =
            ((byte & PLOT_OF_BITS_TO_SHIFT_LEFT_BY_TWO) >> 
                SHIFT_LEFT_BY_TWO) |
            ((byte & PLOT_OF_BITS_TO_SHIFT_RIGHT_BY_FIVE) << 
                SHIFT_RIGHT_BY_FIVE) |
            ((byte & PLOT_OF_BITS_TO_SHIFT_LEFT_BY_THREE) >> 
                SHIFT_LEFT_BY_THREE) |
            ((byte & PLOT_OF_BITS_TO_SHIFT_RIGHT_BY_TWO) << 
                SHIFT_RIGHT_BY_TWO);

        output << char(byteDecrypt ^ key);
    }
}

void OpenFiles(
    std::ifstream& inFile, 
    std::ofstream& outFile, 
    const std::string& inFileName,
    const std::string& outFileName)
{
    inFile.open(inFileName, std::ios::binary);
    outFile.open(outFileName, std::ios::binary);

    if (!inFile.is_open())
    {
        throw std::runtime_error("Failed to open " + inFileName +
            " for reading\n");
    }

    if (!outFile.is_open())
    {
        throw std::runtime_error("Failed to open " + outFileName +
            " for writing\n");
    }
}

void ValidateParameters(const Params& params)
{
    if (params.action != ACTION_CRYPT && params.action != ACTION_DECRYPT)
    {
        throw std::runtime_error("Argument <action> is not defined\n"
            "Use crypt/decrypt as the <action> argument\n");
    }

    if (params.key < 0 || params.key > std::numeric_limits<unsigned char>::max())
    {
        throw std::runtime_error("Argument <key> must be in "
            "the range 0 and 255\n");
    }
}

Params ParseParameters(int argc, char* argv[])
{
    if (argc != 5)
    {
        throw std::runtime_error("Invalid arguments count\n"
            "Usage: crypt.exe <action>(crypt|decrypt) "
            "<input file> <output file> <key>\n");
    }

    Params params;

    params.action = argv[1];
    params.inputPath = argv[2];
    params.outputPath = argv[3];

    try
    {
        params.key = std::stoi(argv[4]);
    }
    catch (const std::exception&)
    {
        throw std::runtime_error("Argument <key> must be numeric\n");
    }

    return params;
}

void PerformActionOnInputFileToOutputFile(
    std::istream& input,
    std::ostream& output,
    const std::string& action, 
    const int& key)
{
    if (action == ACTION_CRYPT)
    {
        Crypt(input, output, key);
    }
    else
    {
        Decrypt(input, output, key);
    }

    if (!output.flush())
    {
        throw std::runtime_error("Failed to save data on disk\n");
    }
}

int main(int argc, char* argv[])
{
    std::ifstream input;
    std::ofstream output;

    Params params;

    try
    {
        params = ParseParameters(argc, argv);

        ValidateParameters(params);

        OpenFiles(input, output, params.inputPath, params.outputPath);

        PerformActionOnInputFileToOutputFile(
            input, output, params.action, params.key);
    }
    catch (const std::exception& e)
    {
        std::cout << e.what();
        return 1;
    }

    return 0;
}