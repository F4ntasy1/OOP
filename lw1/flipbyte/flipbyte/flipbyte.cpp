#include <iostream>
#include <string>
#include <bitset>

void ValidateByte(const int byte)
{
    if (byte < 0 || byte > std::numeric_limits<unsigned char>::max())
    {
        throw std::runtime_error("Argument <byte> must "
            "be between 0 and 255\n");
    }
}

int ParseParameters(int argc, char* argv[])
{
    if (argc != 2)
    {
        throw std::invalid_argument("Invalid arguments count\n"
            "Usage: flipbyte.exe <byte>\n");
    }

    try
    {
        int byte = std::stoi(argv[1]);

        return byte;
    }
    catch (const std::exception&)
    {
        throw std::runtime_error("Argument <byte> must be numeric\n");
    }
}

int RevertBitsInByte(int byte)
{
    const int FOUR_SHIFT = 4;
    const int TWO_SHIFT = 2;
    const int ONE_SHIFT = 1;

    byte = ((byte & 0b1111) << FOUR_SHIFT) | 
        ((byte & 0b00001111) >> FOUR_SHIFT);
    byte = ((byte & 0b00110011) << TWO_SHIFT) | 
        ((byte & 0b11001100) >> TWO_SHIFT);
    byte = ((byte & 0b01010101) << ONE_SHIFT) | 
        ((byte & 0b10101010) >> ONE_SHIFT);

    return byte;
}

int main(int argc, char* argv[])
{
    int byte;

    try
    {
        byte = ParseParameters(argc, argv);

        ValidateByte(byte);
    }
    catch (const std::exception& e)
    {
        std::cout << e.what();
        return 1;
    }

    std::cout << RevertBitsInByte(byte) << std::endl;
}
