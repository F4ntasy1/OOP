#include <iostream>
#include <fstream>
#include <string>

const std::string ACTION_CRYPT = "crypt";
const std::string ACTION_DECRYPT = "decrypt";

void Crypt(std::istream& input, std::ostream& output, const int key)
{
    char ch;
    while (input.read((char*)&ch, sizeof(ch)))
    {
        int byte = ch ^ key;

        int byteCrypt = 
            ((byte << 2) & 28) |    //28 = 00011100
            ((byte >> 5) & 3) |     //3 = 00000011
            ((byte << 3) & 192) |   //192 = 11000000
            ((byte >> 2) & 32);     //32 = 00100000

        output << byteCrypt << ' ';
    }
}

void Decrypt(std::istream& input, std::ostream& output, const int key)
{
    int byte;
    while (input >> byte)
    {
        int byteDecrypt =
            ((byte & 28) >> 2) |
            ((byte & 3) << 5) |
            ((byte & 192) >> 3) |
            ((byte & 32) << 2);

        char ch = byteDecrypt ^ key;

        output << ch;
    }
}

void CheckFilesToOpen(
    std::ifstream& inFile, 
    std::ofstream& outFile, 
    const std::string& inFileName,
    const std::string& outFileName)
{
    std::string failToOpenForReadingMsg = "Failed to open " + inFileName + 
        " for reading\n";

    std::string failToOpenForWritingMsg = "Failed to open " + outFileName +
        " for writing\n";

    try
    {
        inFile.open(inFileName, std::ios::binary);
    }
    catch (const std::exception)
    {
        throw std::invalid_argument(failToOpenForReadingMsg);
    }

    try
    {
        outFile.open(outFileName, std::ios::binary);
    }
    catch (const std::exception)
    {
        throw std::invalid_argument(failToOpenForWritingMsg);
    }

    if (!inFile.is_open())
    {
        throw std::invalid_argument(failToOpenForReadingMsg);
    }

    if (!outFile.is_open())
    {
        throw std::invalid_argument(failToOpenForWritingMsg);
    }
}

void ValidateParameters(
    int argc, 
    char* argv[], 
    int& key, 
    std::string& action,
    std::ifstream& input,
    std::ofstream& output)
{
    if (argc != 5)
    {
        throw std::invalid_argument("Invalid arguments count\n"
            "Usage: crypt.exe <action>(crypt|decrypt) "
            "<input file> <output file> <key>\n");
    }

    action = argv[1];
    if (action != ACTION_CRYPT && action != ACTION_DECRYPT)
    {
        throw std::invalid_argument("Argument <action> is not defined\n"
            "Usage: crypt.exe <action>(crypt|decrypt) ...\n");
    }

    try
    {
        CheckFilesToOpen(input, output, argv[2], argv[3]);
    }
    catch (const std::invalid_argument& e)
    {
        throw e;
    }

    try
    {
        key = std::stoi(argv[4]);
    }
    catch (const std::exception)
    {
        throw std::invalid_argument("Argument <key> must be numeric\n");
    }

    if (key < 0 || key > std::numeric_limits<unsigned char>::max())
    {
        throw std::invalid_argument("Argument <key> must be in "
            "the range 0 - 255\n");
    }
}

int main(int argc, char* argv[])
{
    std::string action;
    std::ifstream input;
    std::ofstream output;
    int key;

    try
    {
        ValidateParameters(argc, argv, key, action, input, output);
    }
    catch (const std::invalid_argument& e)
    {
        std::cout << e.what();
        return 1;
    }

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
        std::cout << "Failed to save data on disk\n";
        return 1;
    }

    return 0;
}