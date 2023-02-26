#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sstream>

using FieldRow = std::vector<char>;
using FillField = std::vector<FieldRow>;

const int MAX_FIELD_SIZE = 100;

const char FILL_SYMBOL = '.';
const char FILL_START_SYMBOL = 'O';

const char DIRECTION_UP = 'U';
const char DIRECTION_DOWN = 'D';
const char DIRECTION_LEFT = 'L';
const char DIRECTION_RIGHT = 'R';

struct FileParams {
    std::string inputPath;
    std::string outputPath;
};

struct StackField {
    int y;
    int x;
};

using Stack = std::vector<StackField>;

void StackPush(Stack& stack, const int& y, const int& x)
{
    StackField stackField = { y, x };

    stack.push_back(stackField);
}

StackField StackPop(Stack& stack)
{
    StackField stackField = {};

    if (!stack.empty())
    {
        stackField = stack[stack.size() - 1];
        stack.pop_back();
    }

    return stackField;
}

void OpenFiles(
    std::ifstream& input, 
    std::ofstream& output, 
    const std::string& inputPath,
    const std::string& outputPath)
{
    input.open(inputPath);
    output.open(outputPath);

    if (!input.is_open())
    {
        throw std::runtime_error("Failed to open " +
            inputPath + " for reading\n");
    }

    if (!output.is_open())
    {
        throw std::runtime_error("Failed to open " +
            outputPath + " for writing\n");
    }
}

FileParams ParseParameters(int argc, char* argv[])
{
    if (argc != 3)
    {
        throw std::runtime_error("Invalid arguments count\n"
            "Usage: fill.exe <input file> <output file>\n");
    }

    FileParams fileParams;

    fileParams.inputPath = argv[1];
    fileParams.outputPath = argv[2];

    return fileParams;
}

void CopyFieldFromFileToVector(std::istream& input, FillField& fillField)
{
    std::string str;
    for (int i = 0; i < MAX_FIELD_SIZE && std::getline(input, str); i++)
    {
        FieldRow fieldRow = fillField[i];

        for (int j = 0; j < MAX_FIELD_SIZE && j < str.length(); j++)
        {
            fieldRow[j] = str[j];
        }

        fillField[i] = fieldRow;
    }
}

bool SaveInStackWithMetkaUpdate(
    const FillField& fillField, 
    Stack& stack, 
    const bool& putMetka, 
    const int& y, 
    const int& x)
{
    const bool isSpace = fillField[y][x] == ' ';

    if (!putMetka && isSpace)
    {
        StackPush(stack, y, x);
    }

    return isSpace;
}

void FillColumnInDirection(
    FillField& fillField,
    int y,
    const int& x,
    const char& directionY,
    Stack& stack)
{
    bool putMetkaLeft = false;
    bool putMetkaRight = false;

    while (y >= 0 && y < MAX_FIELD_SIZE && fillField[y][x] == ' ')
    {
        fillField[y][x] = FILL_SYMBOL;

        if (x - 1 >= 0)
        {
            putMetkaLeft = SaveInStackWithMetkaUpdate(
                fillField, stack, putMetkaLeft, y, x - 1);
        }
        if (x + 1 < MAX_FIELD_SIZE)
        {
            putMetkaRight = SaveInStackWithMetkaUpdate(
                fillField, stack, putMetkaRight, y, x + 1);
        }

        y = directionY == DIRECTION_UP ? y - 1 : y + 1;
    }
}

void FillRowInDirection(
    FillField& fillField, 
    const int& y, 
    int x, 
    const char& directionX,
    Stack& stack)
{
    bool putMetkaUp = false;
    bool putMetkaDown = false;

    while (x >= 0 && x < MAX_FIELD_SIZE && fillField[y][x] == ' ')
    {
        fillField[y][x] = FILL_SYMBOL;

        if (y - 1 >= 0)
        {
            putMetkaUp = SaveInStackWithMetkaUpdate(
                fillField, stack, putMetkaUp, y - 1, x);
        }
        if (y + 1 < MAX_FIELD_SIZE)
        {
            putMetkaDown = SaveInStackWithMetkaUpdate(
                fillField, stack, putMetkaDown, y + 1, x);
        }

        x = directionX == DIRECTION_LEFT ? x - 1 : x + 1;
    }
}

void FillFromStartPosition(FillField& fillField, int y, int x)
{
    Stack stack;
    StackField stackField = { y, x };

    do
    {
        const char symbolByPosition = fillField[stackField.y][stackField.x];

        if (symbolByPosition == ' ' || symbolByPosition == FILL_START_SYMBOL)
        {
            if (symbolByPosition == ' ')
            {
                fillField[stackField.y][stackField.x] = FILL_SYMBOL;
            }

            FillRowInDirection(fillField, stackField.y, 
                stackField.x - 1, DIRECTION_LEFT, stack);
            FillRowInDirection(fillField, stackField.y, 
                stackField.x + 1, DIRECTION_RIGHT, stack);

            FillColumnInDirection(fillField, stackField.y - 1, 
                stackField.x, DIRECTION_UP, stack);
            FillColumnInDirection(fillField, stackField.y + 1, 
                stackField.x, DIRECTION_DOWN, stack);
        }

        stackField = StackPop(stack);
    } while (!stack.empty());
}

void FillToOutline(FillField& fillField)
{
    for (int y = 0; y < MAX_FIELD_SIZE; y++)
    {
        for (int x = 0; x < MAX_FIELD_SIZE; x++)
        {
            if (fillField[y][x] == FILL_START_SYMBOL)
            {
                FillFromStartPosition(fillField, y, x);
            }
        }
    }
}

void CopyVectorToOutFile(FillField& fillField, std::ostream& outFile)
{
    for (FieldRow fieldRow : fillField)
    {
        std::ostringstream oss;

        std::copy(fieldRow.begin(), fieldRow.end(), 
            std::ostream_iterator<char>(oss, ""));

        outFile << oss.str() << std::endl;
    }

    if (!outFile.flush())
    {
        throw std::runtime_error("Failed to save data on disk\n");
    }
}

void FillVectorWithSpacesWithMaxSize(FillField& fillField)
{
    for (int i = 0; i < MAX_FIELD_SIZE; i++)
    {
        FieldRow fieldRow;

        for (int j = 0; j < MAX_FIELD_SIZE; j++)
        {
            fieldRow.push_back(' ');
        }

        fillField.push_back(fieldRow);
    }
}

int main(int argc, char* argv[])
{
    std::ifstream input;
    std::ofstream output;

    FileParams fileParams;
    FillField fillField;

	try
	{
        fileParams = ParseParameters(argc, argv);

        OpenFiles(
            input, output, fileParams.inputPath, fileParams.outputPath);

        FillVectorWithSpacesWithMaxSize(fillField);

        CopyFieldFromFileToVector(input, fillField);

        FillToOutline(fillField);

        CopyVectorToOutFile(fillField, output);
	}
	catch (const std::exception& e)
	{
		std::cout << e.what();
		return 1;
	}

    return 0;
}