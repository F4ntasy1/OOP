#include <iostream>
#include <fstream>
#include <string>

const std::string ACTION_CRYPT = "crypt";
const std::string ACTION_DECRYPT = "decrypt";

void crypt(std::istream& input, std::ostream& output, const int key)
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

void decrypt(std::istream& input, std::ostream& output, const int key)
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

void validateParameters(
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
        input.open(argv[2], std::ios::binary);
    }
    catch (const std::exception)
    {
        std::cout << "Failed to open " << argv[2] << " for reading\n";
        throw std::invalid_argument("");
    }

    try
    {
        output.open(argv[3], std::ios::binary);
    }
    catch (const std::exception)
    {
        std::cout << "Failed to open " << argv[3] << " for writing\n";
        throw std::invalid_argument("");
    }

    if (!input.is_open())
    {
        std::cout << "Failed to open " << argv[2] << " for reading\n";
        throw std::invalid_argument("");
    }

    if (!output.is_open())
    {
        std::cout << "Failed to open " << argv[3] << " for writing\n";
        throw std::invalid_argument("");
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
        validateParameters(argc, argv, key, action, input, output);
    }
    catch (const std::invalid_argument& e)
    {
        std::cout << e.what();
        return 1;
    }

    if (action == ACTION_CRYPT)
    {
        crypt(input, output, key);
    }
    else
    {
        decrypt(input, output, key);
    }

    if (!output.flush())
    {
        std::cout << "Failed to save data on disk\n";
        return 1;
    }

    return 0;
}