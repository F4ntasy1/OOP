#include <iostream>
#include <fstream>
#include <vector>

struct TranslatePhrase {
    std::string phrase;
    std::string translate;
};

using Glossary = std::vector<TranslatePhrase>;

void OpenFileForReading(std::ifstream& inFile, const std::string& inFileName)
{
    inFile.open(inFileName);
    if (!inFile.is_open())
    {
        throw std::runtime_error("Failed to open " + inFileName +
            " for reading\n");
    }
}

void OpenFileForWriting(std::ofstream& outFile, const std::string& outFileName)
{
    outFile.open(outFileName);
    if (!outFile.is_open())
    {
        throw std::runtime_error("Failed to open " + outFileName +
            " for writing\n");
    }
}

void SaveGlossaryInOutFile(Glossary& glossary, const std::string& outFileName)
{
    std::ofstream outFile;

    OpenFileForWriting(outFile, outFileName);

    for (TranslatePhrase translatePhrase : glossary)
    {
        outFile << translatePhrase.phrase << ' ' << translatePhrase.translate 
            << std::endl;
    }

    if (!outFile.flush())
    {
        throw std::runtime_error("Failed to save data on disk\n");
    }
}

void SavePhraseInGlossary(const TranslatePhrase& translatePhrase, Glossary& glossary)
{

}

std::string FindTranslateInGlossary(const std::string& phrase, Glossary& glossary)
{

}

void WorkWithGlossaryInDialogMode(std::istream& input, std::istream& glossaryFile)
{

}

int main(int argc, char* argv[])
{
    
}