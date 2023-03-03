#include <iostream>
#include <fstream>
#include <string>

const auto ACTION_CRYPT = "crypt";
const auto ACTION_DECRYPT = "decrypt";

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

void Crypt(std::istream& input, std::ostream& output, const int key)
{
    const int CHAR_SIZE = sizeof(uint8_t);

    char byte;
    while (input.read(&byte, CHAR_SIZE))
    {
        int byteCryptByKey = byte ^ key;

        output << (
            ((byteCryptByKey << 2) & PLOT_OF_BITS_TO_SHIFT_LEFT_BY_TWO) |
            ((byteCryptByKey >> 5) & PLOT_OF_BITS_TO_SHIFT_RIGHT_BY_FIVE) |
            ((byteCryptByKey << 3) & PLOT_OF_BITS_TO_SHIFT_LEFT_BY_THREE) |
            ((byteCryptByKey >> 2) & PLOT_OF_BITS_TO_SHIFT_RIGHT_BY_TWO)
        ) << ' ';
    }
}

void Decrypt(std::istream& input, std::ostream& output, const int key)
{
    int byte;
    while (input >> byte)
    {
        int byteDecrypt =
            ((byte & PLOT_OF_BITS_TO_SHIFT_LEFT_BY_TWO) >> 2) |
            ((byte & PLOT_OF_BITS_TO_SHIFT_RIGHT_BY_FIVE) << 5) |
            ((byte & PLOT_OF_BITS_TO_SHIFT_LEFT_BY_THREE) >> 3) |
            ((byte & PLOT_OF_BITS_TO_SHIFT_RIGHT_BY_TWO) << 2);

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

    return params;
}

void PerformActionOnInputFileToOutputFile(const Params& params)
{
    std::ifstream input;
    std::ofstream output;

    OpenFiles(input, output, params.inputPath, params.outputPath);

    if (params.action == ACTION_CRYPT)
    {
        Crypt(input, output, params.key);
    }
    else
    {
        Decrypt(input, output, params.key);
    }

    if (!output.flush())
    {
        throw std::runtime_error("Failed to save data on disk\n");
    }
}

int main(int argc, char* argv[])
{
    try
    {
        Params params = ParseParameters(argc, argv);

        PerformActionOnInputFileToOutputFile(params);
    }
    catch (const std::exception& e)
    {
        std::cout << e.what();
        return 1;
    }
    //файл зашифрованный и изнач. должны совпадать по длине, исп. символы

    return 0;
}