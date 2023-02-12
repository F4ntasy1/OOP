#include <iostream>
#include <fstream>
#include <string>

void crypt(std::ifstream& input, std::ofstream& output, const int key)
{
    char ch;
    while (input.get(ch))
    {
        int byte = ch ^ key;

        int byteCrypt = 
            ((byte << 2) & 0x1C) |
            ((byte >> 5) & 0x03) |
            ((byte << 3) & 0xC0) |
            ((byte >> 2) & 0x20);

        output << byteCrypt << ' ';
    }
}

void decrypt(std::ifstream& input, std::ofstream& output, const int key)
{
    int byte;
    while (input >> byte)
    {
        int byteDecrypt =
            ((byte & 0x1C) >> 2) |
            ((byte & 0x03) << 5) |
            ((byte & 0xC0) >> 3) |
            ((byte & 0x20) << 2);

        char ch = byteDecrypt ^ key;

        output << ch;
    }
}

int main(int argc, char* argv[])
{
	if (argc != 5)
	{
		std::cout << "Invalid arguments count\n"
			<< "Usage: crypt.exe <action>(crypt|decrypt) "
            << "<input file> <output file> <key>\n";
		return 1;
	}

    std::ifstream input(argv[2]);
    if (!input.is_open())
    {
        std::cout << "Failed to open " << argv[2] << " for reading\n";
        return 1;
    }

    std::ofstream output(argv[3]);
    if (!output.is_open())
    {
        std::cout << "Failed to open " << argv[3] << " for writing\n";
        return 1;
    }
    
    int key;
    try 
    {
        key = std::stoi(argv[4]);
    }
    catch (const std::exception& ex)
    {
        std::cout << "Argument <key> must be numeric\n";
        return 1;
    }

    if (key < 0 || key > 255)
    {
        std::cout << "Argument <key> must be in the range 0-255\n";
        return 1;
    }

    std::string action = argv[1];

    if (action == "crypt")
    {
        crypt(input, output, key);
    }
    else if (action == "decrypt")
    {
        decrypt(input, output, key);
    }
    else
    {
        std::cout << "Argument <action> is not defined\n"
            << "Usage: crypt.exe <action>(crypt|decrypt) ...";
        return 1;
    }

    if (!output.flush())
    {
        std::cout << "Failed to save data on disk\n";
        return 1;
    }

    return 0;
}