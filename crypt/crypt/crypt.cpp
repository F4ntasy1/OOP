#include <iostream>
#include <fstream>
#include <string>

const std::string ACTION_CRYPT = "crypt";
const std::string ACTION_DECRYPT = "decrypt";

const int SHIFT_LEFT_BY_TWO = 2;
const int SHIFT_RIGHT_BY_TWO = 2;
const int SHIFT_RIGHT_BY_FIVE = 5;
const int SHIFT_LEFT_BY_THREE = 3;

struct Params {
    std::string action;
    std::string inputPath;
    std::string outputPath;
    int key;
};

void Crypt(std::istream& input, std::ostream& output, const int key)
{
    char ch;
    while (input.read((char*)&ch, sizeof(ch)))
    {
        int byte = ch ^ key;

        int byteCrypt = 
            ((byte << SHIFT_LEFT_BY_TWO) & 0b00011100) |
            ((byte >> SHIFT_RIGHT_BY_FIVE) & 0b00000011) |
            ((byte << SHIFT_LEFT_BY_THREE) & 0b11000000) |
            ((byte >> SHIFT_RIGHT_BY_TWO) & 0b00100000);

        output << byteCrypt << ' ';
    }
}

void Decrypt(std::istream& input, std::ostream& output, const int key)
{
    int byte;
    while (input >> byte)
    {
        int byteDecrypt =
            ((byte & 0b00011100) >> SHIFT_LEFT_BY_TWO) |
            ((byte & 0b00000011) << SHIFT_RIGHT_BY_FIVE) |
            ((byte & 0b11000000) >> SHIFT_LEFT_BY_THREE) |
            ((byte & 0b00100000) << SHIFT_RIGHT_BY_TWO);

        char ch = byteDecrypt ^ key;

        output << ch;
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
        throw std::logic_error("Failed to open " + inFileName +
            " for reading\n");
    }

    if (!outFile.is_open())
    {
        throw std::logic_error("Failed to open " + outFileName +
            " for writing\n");
    }
}

void ValidateParameters(Params& params)
{
    if (params.action != ACTION_CRYPT && params.action != ACTION_DECRYPT)
    {
        throw std::runtime_error("Argument <action> is not defined\n"
            "Usage: crypt.exe <action>(crypt|decrypt) ...\n");
    }

    if (params.key < 0 || params.key > std::numeric_limits<unsigned char>::max())
    {
        throw std::runtime_error("Argument <key> must be in "
            "the range 0 - 255\n");
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
    }
    catch (const std::exception& e)
    {
        std::cout << e.what();
        return 1;
    }

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
        std::cout << "Failed to save data on disk\n";
        return 1;
    }

    return 0;
}