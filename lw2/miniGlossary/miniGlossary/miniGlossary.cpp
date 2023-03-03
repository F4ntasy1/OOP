#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include <map>
#include <algorithm>

using Glossary = std::map<std::string, std::string>;

void OpenFileForReading(
    std::ifstream& inFile, const std::string& inFileName)
{
    inFile.open(inFileName);
    if (!inFile.is_open())
    {
        std::ofstream toCreateFile(inFileName);
        toCreateFile.close();

        inFile.open(inFileName);
        if (!inFile.is_open())
        {
            throw std::runtime_error("Failed to open " + inFileName +
                " for reading\n");
        }
    }
}

void OpenFileForWriting(
    std::ofstream& outFile, const std::string& outFileName)
{
    outFile.open(outFileName);
    if (!outFile.is_open())
    {
        throw std::runtime_error("Failed to open " + outFileName +
            " for writing\n");
    }
}

bool CheckStringForRussianCharacters(const std::string& str)
{
    bool result = true;

    std::for_each(str.begin(), str.end(), [&result](const char ch) {
        if ((ch < 'А' || ch > 'я') && ch != 'ё' && ch != 'Ё' && ch != ' ')
        {
            result = false;
        }
        });

    return result;
}

bool CheckStringForEnglishCharacters(const std::string& str)
{
    bool result = true;

    std::for_each(str.begin(), str.end(), [&result](const char ch) {
        if ((ch < 'A' || (ch > 'Z' && ch < 'a') || ch > 'z') && ch != ' ')
        {
            result = false;
        }
        });

    return result;
}

void SaveGlossaryInOutFile(
    Glossary& glossary, const std::string& outFileName)
{
    std::ofstream outFile;

    OpenFileForWriting(outFile, outFileName);

    for (const auto& item : glossary)
    {
        outFile << item.first << std::endl << item.second << std::endl;
    }

    if (!outFile.flush())
    {
        throw std::runtime_error("Failed to save data on disk\n");
    }
}

void CopyGlossaryFromFileToVector(std::istream& input, Glossary& glossary)
{
    std::string phrase;
    while (getline(input, phrase))
    {
        if (!getline(input, glossary[phrase]))
        {
            throw std::runtime_error("Glossary file has error format\n");
        }
    }
}

std::string ConvertStringToLowerCase(std::string str)
{
    std::transform(str.begin(), str.end(),
        str.begin(), tolower);

    return str;
}

std::string FindTranslateInGlossary(
    std::string phrase, Glossary& glossary)
{
    phrase = ConvertStringToLowerCase(phrase);

    for (const auto& item : glossary)
    {
        if (ConvertStringToLowerCase(item.first) == phrase)
        {
            return item.second;
        }
    }

    return std::string();
}

void EnterTranslationToSaveIntoGlossary(
    std::istream& input, Glossary& glossary, const std::string& phrase)
{
    std::cout << R"(Неизвестное слово ")" << phrase << 
        "\". Введите перевод или пустую строку для отказа.\n";

    std::string translate = std::string();

    do
    {
        if (!translate.empty())
        {
            std::cout << "Словарь поддерживает англо-русский перевод.\n"
                << "Введите русский перевод.\n";
        }
        getline(input, translate);
    } while (!CheckStringForRussianCharacters(translate));

    if (!translate.empty())
    {
        glossary[phrase] = translate;
        
        std::cout << R"(Слово ")" << phrase << 
            R"(" сохранено в словаре как ")" << translate << "\"\n";
    }
    else
    {
        std::cout << "Слово " << phrase << " проигнорировано.\n";
    }
}

void Exit(
    std::istream& input, 
    const std::string& outFileName, 
    Glossary& glossary,
    const int glossaryStartSize)
{
    if (glossary.size() == glossaryStartSize)
    {
        return;
    }

    std::cout << "В словарь были внесены изменения.\nВведите "
        << "Y или y для сохранения перед выходом.\n";

    char ch;
    input >> ch;

    if (ch == 'Y' || ch == 'y' || ch == 'у')
    {
        SaveGlossaryInOutFile(glossary, outFileName);
        std::cout << "Изменения сохранены.\n";
    }
    else
    {
        std::cout << "Изменения не сохранены.\n";
    }
}

void WorkWithGlossaryInDialogMode(
    std::istream& input, const std::string& glossaryFileName)
{
    const std::string EXIT_METKA = "...";

    std::ifstream glossaryFile;
    OpenFileForReading(glossaryFile, glossaryFileName);

    Glossary glossary;
    CopyGlossaryFromFileToVector(glossaryFile, glossary);
    const int glossaryStartSize = glossary.size();

    std::string inputPhrase;
    while (getline(input, inputPhrase))
    {
        if (inputPhrase == EXIT_METKA)
        {
            return Exit(
                input, glossaryFileName, glossary, glossaryStartSize);
        }
        else if (!CheckStringForEnglishCharacters(inputPhrase))
        {
            std::cout << "Словарь поддерживает англо-русский перевод.\n"
                << "Введите английское слово/словосочетание.\n";
            continue;
        }

        std::string translate = 
            FindTranslateInGlossary(inputPhrase, glossary);

        if (translate.empty())
        {
            EnterTranslationToSaveIntoGlossary(
                input, glossary, inputPhrase);
        }
        else
        {
            std::cout << translate << std::endl;
        }
    }
}

std::string GetGlossaryFileNameFromParams(const int argc, char* argv[])
{
    const std::string DEFAULT_GLOSSARY_FILE_NAME = "glossary.txt";

    if (argc > 2)
    {
        throw std::runtime_error("Too many arguments\n"
            "Usage: miniGlossary.exe <glossaryFile>");
    }
    else if (argc == 2)
    {
        return argv[1];
    }
   
    return DEFAULT_GLOSSARY_FILE_NAME;
}

int main(int argc, char* argv[])
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    try
    {
        std::string glossaryFileName = 
            GetGlossaryFileNameFromParams(argc, argv);

        WorkWithGlossaryInDialogMode(std::cin, glossaryFileName);
    }
    catch (const std::exception& e)
    {
        std::cout << e.what();
        return 1;
    }

    return 0;
}