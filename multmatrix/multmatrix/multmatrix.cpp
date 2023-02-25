#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

using matrix = std::vector<std::vector<float>>;

using matrixRow = std::vector<float>;

const int MATRIX_SIZE = 3;

struct Params {
    std::string fileMatrixPath1;
    std::string fileMatrixPath2;
};

void ReadMatrixFromFileToVector(std::istream& file, matrix& matrixResult)
{
    float num;
    for (int i = 0; i < MATRIX_SIZE; i++)
    {
        matrixRow row;

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
        matrixRow row;

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

    fileMatrix1.open(fileMatrixPath1);
    fileMatrix2.open(fileMatrixPath2);
    output.open(OUTPUT_FILE_NAME);

    if (!fileMatrix1.is_open())
    {
        throw std::runtime_error("Failed to open " +
            fileMatrixPath1 + " for reading\n");
    }

    if (!fileMatrix2.is_open())
    {
        throw std::runtime_error("Failed to open " +
            fileMatrixPath2 + " for reading\n");
    }

    if (!output.is_open())
    {
        throw std::runtime_error("Failed to open " +
            OUTPUT_FILE_NAME + " for reading\n");
    }
}

Params ParseParameters(int argc, char* argv[])
{
    if (argc != 3)
    {
        throw std::runtime_error("Invalid arguments count\n"
            "Usage: multmatrix.exe <matrix file1> <matrix file2>\n");
    }

    Params params;

    params.fileMatrixPath1 = argv[1];
    params.fileMatrixPath2 = argv[2];

    return params;
}

int main(int argc, char* argv[])
{
    std::ifstream fileMatrix1;
    std::ifstream fileMatrix2;
    std::ofstream output;

    matrix matrix1;
    matrix matrix2;
    matrix matrixResult;

    try
    {
        Params params;

        params = ParseParameters(argc, argv);

        OpenFiles(fileMatrix1, fileMatrix2, output, 
            params.fileMatrixPath1, params.fileMatrixPath2);

        ReadMatrixFromFileToVector(fileMatrix1, matrix1);
        ReadMatrixFromFileToVector(fileMatrix2, matrix2);

        MultiplyMatrices(matrix1, matrix2, matrixResult);

        SaveMatrixInOutFile(matrixResult, output);
    }
    catch (const std::exception& e)
    {
        std::cout << e.what();
        return 1;
    }

    return 0;
}
