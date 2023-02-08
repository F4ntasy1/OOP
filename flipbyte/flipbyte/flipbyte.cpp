﻿// flipbyte #50
#include <iostream>
#include <string>
#include <bitset>

std::string convertByteToBinary(int byte)
{
    const int MAX_LENGTH = 8;

    std::string buffer;

    do
    {
        buffer += char('0' + byte % 2);
        byte = byte / 2;
    } while (byte > 0);

    std::string result = std::string(buffer.crbegin(), buffer.crend());
    result.insert(0, MAX_LENGTH - buffer.length(), '0');

    return result;
}

int convertBinaryToByte(std::string binary)
{
    return std::bitset<32>(binary).to_ulong();
}

std::string swapString(std::string str)
{
    std::string swapStr = std::string();

    for (int i = str.length() - 1; i >= 0; i--)
    {
        swapStr += str[i];
    }

    return swapStr;
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cout << "Invalid arguments count. "
            << "Usage: flipbyte.exe <byte>\n";
        return 1;
    }

    int byte;

    try
    {
        byte = std::stoi(argv[1]);
    }
    catch (const std::exception& ex)
    {
        std::cout << "Argument <byte> must be numeric\n";
        return 1;
    }

    if (byte < 0 || byte > 255)
    {
        std::cout << "Argument <byte> must be between 0 and 255\n";
        return 1;
    }

    std::string binary = convertByteToBinary(byte);

    std::string binarySwap = swapString(binary);

    std::cout << convertBinaryToByte(binarySwap);
}
