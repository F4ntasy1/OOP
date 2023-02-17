#include <iostream>
#include <fstream>
#include <vector>
#include <string>

// Добавить тесты
// Добавить обработку пустых строк, подгонять под самую длинную строку с фигурой

using field = std::vector<std::vector<char>>;

const int MAX_FIELD_SIZE = 100;

const char FILL_SYMBOL = '.';
const char FILL_START_SYMBOL = 'O';

void ValidateParameters(
    int argc, char* argv[], std::ifstream& input, std::ofstream& output)
{
    if (argc != 3)
    {
        throw std::invalid_argument("Invalid arguments count\n"
            "Usage: fill.exe <input file> <output file>\n");
    }

    std::string inputPath = argv[1];
    std::string outputPath = argv[2];

    try
    {
        input.open(inputPath);
    }
    catch (const std::exception)
    {
        throw std::invalid_argument("Failed to open " + inputPath + 
            " for reading\n");
    }

    try
    {
        output.open(outputPath);
    }
    catch (const std::exception)
    {
        throw std::invalid_argument("Failed to open " + outputPath + 
            " for writing\n");
    }

    if (!input.is_open())
    {
        throw std::invalid_argument("Failed to open " + inputPath +
            " for reading\n");
    }

    if (!output.is_open())
    {
        throw std::invalid_argument("Failed to open " + outputPath +
            " for writing\n");
    }
}

void CopyFieldFromFileToVector(std::istream& input, field& fieldResult)
{
    const char ENDL = '\n';

    char ch;
    for (int height = 0; height < MAX_FIELD_SIZE && !input.eof(); height++)
    {
        std::vector<char> row;

        for (int width = 0; input.get(ch) && ch != ENDL; width++)
        {
            if (width < MAX_FIELD_SIZE)
            {
                row.push_back(ch);
            }
        }

        fieldResult.push_back(row);
    }
}

void FillToOutline(field& vec, int x, int y)
{
    if (y >= vec.size() || x >= vec[y].size() || vec[y][x] != ' ')
    {
        return;
    }

    vec[y][x] = FILL_SYMBOL;

    FillToOutline(vec, x + 1, y);
    FillToOutline(vec, x - 1, y);
    FillToOutline(vec, x, y + 1);
    FillToOutline(vec, x, y - 1);
}

void Fill(field& vec)
{
    for (int y = 0; y < vec.size(); y++)
    {
        for (int x = 0; x < vec[y].size(); x++)
        {
            if (vec[y][x] == FILL_START_SYMBOL)
            {
                FillToOutline(vec, x + 1, y);
                FillToOutline(vec, x - 1, y);
                FillToOutline(vec, x, y + 1);
                FillToOutline(vec, x, y - 1);
            }
        }
    }
}

void CopyVectorToOutFile(field vec, std::ostream& outFile)
{
    for (auto row : vec)
    {
        for (auto ch : row)
        {
            outFile << ch;
        }
        outFile << std::endl;
    }

    if (!outFile.flush())
    {
        throw std::invalid_argument("Failed to save data on disk\n");
    }
}

int main(int argc, char* argv[])
{
    std::ifstream input;
    std::ofstream output;

	try
	{
        ValidateParameters(argc, argv, input, output);
	}
	catch (const std::invalid_argument& e)
	{
		std::cout << e.what();
		return 1;
	}

    field fieldToFill;

    CopyFieldFromFileToVector(input, fieldToFill);

    Fill(fieldToFill);

    try
    {
        CopyVectorToOutFile(fieldToFill, output);
    }
    catch (const std::invalid_argument& e)
    {
        std::cout << e.what();
        return 1;
    }

    return 0;
}