#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

using matrix = std::vector<std::vector<float>>;

const int MATRIX_SIZE = 3;

void ReadMatrixFromFileToVector(std::istream& file, matrix& matrixResult)
{
    float num;
    for (int i = 0; i < MATRIX_SIZE; i++)
    {
        std::vector<float> row;

        for (int j = 0; j < MATRIX_SIZE; j++)
        {
            if (!(file >> num))
            {
                throw std::runtime_error("Unsupported matrix format\n"
                    "Use numbers as coefficients, and also "
                    "consider the size of the matrix - 3x3\n");
            }

            row.push_back(num);
        }

        matrixResult.push_back(row);
    }
}

void SaveMatrixInOutFile(const matrix& matrixToSave, std::ostream& outFile)
{
    for (auto row : matrixToSave)
    {
        std::ostringstream oss;

        std::copy(row.begin(), row.end(), std::ostream_iterator<float>(oss, "\t"));

        outFile << oss.str() << std::endl;
    }

    if (!outFile.flush())
    {
        throw std::runtime_error("Failed to save data on disk\n");
    }
}

void MultiplyMatrices(
    const matrix& matrix1, 
    const matrix& matrix2, 
    matrix& matrixResult)
{
    for (int i = 0; i < MATRIX_SIZE; i++)
    {
        std::vector<float> row;

        for (int j = 0; j < MATRIX_SIZE; j++)
        {
            row.push_back(matrix1[i][j] * matrix2[j][i]);
        }

        matrixResult.push_back(row);
    }
}

void OpenFiles(
    std::ifstream& fileMatrix1,
    std::ifstream& fileMatrix2,
    std::ofstream& output,
    const std::string& fileMatrixPath1,
    const std::string& fileMatrixPath2)
{
    const std::string OUTPUT_FILE_NAME = "matrix-result.txt";

    std::string failedToOpenFileOneForReadingMsg = "Failed to open " +
        fileMatrixPath1 + " for reading\n";
    std::string failedToOpenFileTwoForReadingMsg = "Failed to open " +
        fileMatrixPath2 + " for reading\n";
    std::string failedToOpenOutFileForWritingMsg = "Failed to open " +
        OUTPUT_FILE_NAME + " for reading\n";

    try
    {
        fileMatrix1.open(fileMatrixPath1);
    }
    catch (const std::exception&)
    {
        throw std::invalid_argument(failedToOpenFileOneForReadingMsg);
    }

    try
    {
        fileMatrix2.open(fileMatrixPath2);
    }
    catch (const std::exception&)
    {
        throw std::invalid_argument(failedToOpenFileTwoForReadingMsg);
    }

    if (!fileMatrix1.is_open())
    {
        throw std::invalid_argument(failedToOpenFileOneForReadingMsg);
    }

    if (!fileMatrix2.is_open())
    {
        throw std::invalid_argument(failedToOpenFileTwoForReadingMsg);
    }

    try
    {
        output.open(OUTPUT_FILE_NAME);
    }
    catch (const std::exception&)
    {
        throw std::invalid_argument(failedToOpenOutFileForWritingMsg);
    }

    if (!output.is_open())
    {
        throw std::invalid_argument(failedToOpenOutFileForWritingMsg);
    }
}

void ValidateParameters(
    int argc, 
    char* argv[], 
    std::ifstream& fileMatrix1,
    std::ifstream& fileMatrix2,
    std::ofstream& output)
{
    if (argc != 3)
    {
        throw std::invalid_argument("Invalid arguments count\n"
            "Usage: multmatrix.exe <matrix file1> <matrix file2>\n");
    }

    OpenFiles(fileMatrix1, fileMatrix2, output, argv[1], argv[2]);
}

int main(int argc, char* argv[])
{
    std::ifstream fileMatrix1;
    std::ifstream fileMatrix2;
    std::ofstream output;

    try
    {
        ValidateParameters(argc, argv, fileMatrix1, fileMatrix2, output);
    }
    catch (const std::invalid_argument& e)
    {
        std::cout << e.what();
        return 1;
    }

    matrix matrix1;
    matrix matrix2;

    try
    {
        ReadMatrixFromFileToVector(fileMatrix1, matrix1);
    }
    catch (const std::runtime_error& e)
    {
        std::cout << e.what();
        return 1;
    }

    try
    {
        ReadMatrixFromFileToVector(fileMatrix2, matrix2);
    }
    catch (const std::runtime_error& e)
    {
        std::cout << e.what();
        return 1;
    }

    matrix matrixResult;
    MultiplyMatrices(matrix1, matrix2, matrixResult);

    try
    {
        SaveMatrixInOutFile(matrixResult, output);
    }
    catch (const std::runtime_error& e)
    {
        std::cout << e.what();
        return 1;
    }

    return 0;
}
