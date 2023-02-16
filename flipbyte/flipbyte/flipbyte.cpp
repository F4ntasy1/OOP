#include <iostream>
#include <string>
#include <bitset>

void validateParameters(int argc, char* argv[], int& byte)
{
    if (argc != 2)
    {
        throw std::invalid_argument("Invalid arguments count\n"
            "Usage: flipbyte.exe <byte>\n");
    }

    try
    {
        byte = std::stoi(argv[1]);
    }
    catch (const std::exception&)
    {
        throw std::invalid_argument("Argument <byte> must be numeric\n");
    }

    if (byte < 0 || byte > std::numeric_limits<unsigned char>::max())
    {
        throw std::invalid_argument("Argument <byte> must "
            "be between 0 and 255\n");
    }
}

int revertBitsInByte(int byte)
{
    return ((byte << 7) & 128) |
        ((byte << 5) & 64) |
        ((byte << 3) & 32) |
        ((byte << 1) & 16) |
        ((byte >> 1) & 8) |
        ((byte >> 3) & 4) |
        ((byte >> 5) & 2) |
        ((byte >> 7) & 1);
}

int main(int argc, char* argv[])
{
    int byte;

    try
    {
        validateParameters(argc, argv, byte);
    }
    catch (const std::exception& e)
    {
        std::cout << e.what();
        return 1;
    }

    std::cout << revertBitsInByte(byte) << std::endl;
}
