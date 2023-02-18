#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sstream>

using field = std::vector<std::vector<char>>;

const int MAX_FIELD_SIZE = 100;

const char FILL_SYMBOL = '.';
const char FILL_START_SYMBOL = 'O';

void OpenFiles(
    std::ifstream& input, 
    std::ofstream& output, 
    const std::string& inputPath,
    const std::string& outputPath)
{
    std::string failedToOpenForReadingMsg = "Failed to open " +
        inputPath + " for reading\n";
    std::string failedToOpenForWritingMsg = "Failed to open " +
        outputPath + " for writing\n";

    try
    {
        input.open(inputPath);
    }
    catch (const std::exception&)
    {
        throw std::invalid_argument(failedToOpenForReadingMsg);
    }

    try
    {
        output.open(outputPath);
    }
    catch (const std::exception&)
    {
        throw std::invalid_argument(failedToOpenForWritingMsg);
    }

    if (!input.is_open())
    {
        throw std::invalid_argument(failedToOpenForReadingMsg);
    }

    if (!output.is_open())
    {
        throw std::invalid_argument(failedToOpenForWritingMsg);
    }
}

void ValidateParameters(
    int argc, char* argv[], std::ifstream& input, std::ofstream& output)
{
    if (argc != 3)
    {
        throw std::invalid_argument("Invalid arguments count\n"
            "Usage: fill.exe <input file> <output file>\n");
    }

    try
    {
        OpenFiles(input, output, argv[1], argv[2]);
    }
    catch (const std::invalid_argument&)
    {
        throw;
    }
}

void EqualizeSizeOfVectorsInVector(field& vec)
{
    size_t maxSize = 0;

    for (auto row : vec)
    {
        size_t rowSize = row.size();

        if (rowSize > maxSize)
        {
            maxSize = rowSize;
        }
    }

    std::transform(vec.begin(), vec.end(), vec.begin(), 
    [maxSize](std::vector<char>& row) {
        size_t rowSize = row.size();

        for (rowSize; rowSize < maxSize; rowSize++)
        {
            row.push_back(' ');
        }

        return row;
    });
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

    EqualizeSizeOfVectorsInVector(fieldResult);
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
        std::ostringstream oss;

        std::copy(row.begin(), row.end(), std::ostream_iterator<char>(oss, ""));

        outFile << oss.str() << std::endl;
    }

    if (!outFile.flush())
    {
        throw std::runtime_error("Failed to save data on disk\n");
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
    catch (const std::runtime_error& e)
    {
        std::cout << e.what();
        return 1;
    }

    return 0;
}