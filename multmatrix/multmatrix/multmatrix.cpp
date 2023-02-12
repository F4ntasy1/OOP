#include <iostream>
#include <fstream>
#include <string>
#include <vector>

std::vector<std::vector<float>> readMatrixFromFileToVector(
    std::ifstream& file)
{
    std::vector<std::vector<float>> matrix;
    std::vector<float> row;

    float num;
    for (int i = 0; i < 3; i++)
    {
        row.clear();

        for (int j = 0; j < 3; j++)
        {
            if (!(file >> num))
            {
                row.clear();
                break;
            }

            row.push_back(num);
        }

        if (row.empty())
        {
            matrix.clear();
            break;
        }

        matrix.push_back(row);
    }

    return matrix;
}

int saveMatrixInFile(
    std::vector<std::vector<float>> matrix, std::ofstream& outFile)
{
    const char TABULATION = '\t';

    for (auto row : matrix)
    {
        short tabulationCount = 0;

        for (auto element : row)
        {
            outFile << element;
            
            if (tabulationCount < 2)
            {
                outFile << TABULATION;
            }

            tabulationCount++;
        }

        outFile << std::endl;
    }

    if (!outFile.flush())
    {
        std::cout << "Failed to save data on disk\n";
        return 1;
    }

    return 0;
}

std::vector<std::vector<float>> multiplyMatrices(
    std::vector<std::vector<float>> matrix1, 
    std::vector<std::vector<float>> matrix2)
{
    std::vector<std::vector<float>> matrixResult;

    std::vector<float> row;
    for (int i = 0; i < 3; i++)
    {
        row.clear();

        for (int j = 0; j < 3; j++)
        {
            row.push_back(matrix1[i][j] * matrix2[j][i]);
        }

        matrixResult.push_back(row);
    }

    return matrixResult;
}

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        std::cout << "Invalid arguments count\n"
            << "Usage: multmatrix.exe <matrix file1> <matrix file2>\n";
        return 1;
    }

    std::ifstream fileMatrix1(argv[1]);
    if (!fileMatrix1.is_open())
    {
        std::cout << "Failed to open " << argv[1] << " for reading\n";
        return 1;
    }

    std::ifstream fileMatrix2(argv[2]);
    if (!fileMatrix2.is_open())
    {
        std::cout << "Failed to open " << argv[2] << " for reading\n";
        return 1;
    }

    auto matrix1 = readMatrixFromFileToVector(fileMatrix1);
    auto matrix2 = readMatrixFromFileToVector(fileMatrix2);

    if (matrix1.empty() || matrix2.empty())
    {
        std::cout << "Unsupported matrix format\n"
            << "Use numbers as coefficients, and also "
            << "consider the size of the matrix - 3x3\n";
        return 1;
    }

    auto result = multiplyMatrices(matrix1, matrix2);

    std::ofstream output("matrixResult.txt");

    if (!output.is_open())
    {
        std::cout << "Failed to open result file for writing\n";
        return 1;
    }

    return saveMatrixInFile(result, output) ? 1 : 0;
}
