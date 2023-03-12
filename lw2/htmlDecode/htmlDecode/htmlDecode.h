#pragma once

#include <iostream>
#include <string>

#include "consts.h"

char GetDecodeHtmlSymbol(const std::string& htmlEntity)
{
    if (htmlEntity == ENCODE_DOUBLE_QUOTE) return DECODE_DOUBLE_QUOTE;
    else if (htmlEntity == ENCODE_APOSTROPHE) return DECODE_APOSTROPHE;
    else if (htmlEntity == ENCODE_SIGN_LESS) return DECODE_SIGN_LESS;
    else if (htmlEntity == ENCODE_SIGN_MORE) return DECODE_SIGN_MORE;
    else if (htmlEntity == ENCODE_AMPERSAND) return DECODE_AMPERSAND;
    else throw std::runtime_error("Unknown html entity");
}

bool CheckStringForMatchingHtmlEntity(const std::string& str)
{
    return (str == ENCODE_DOUBLE_QUOTE || str == ENCODE_APOSTROPHE ||
        str == ENCODE_SIGN_LESS || str == ENCODE_SIGN_MORE ||
        str == ENCODE_AMPERSAND);
}

std::string HtmlDecode(const std::string& html)
{
    const char ENTITY_START_CHARACTER = '&';
    const char ENTITY_END_CHARACTER = ';';

    // исп. так : std::string decodeString;
    std::string decodeString = std::string();
    // название не верное
    // сделать без временной строки, через индексы изнач. строки
    std::string temp;
    bool htmlEntityProcessing = false;

    for (const char ch : html)
    {
        if (ch == ENTITY_START_CHARACTER)
        {
            decodeString += temp;

            htmlEntityProcessing = true;
            temp = ch;
        }
        else if (htmlEntityProcessing)
        {
            temp += ch;

            if (ch == ENTITY_END_CHARACTER)
            {
                // не искать дважды
                if (CheckStringForMatchingHtmlEntity(temp))
                {
                    temp = GetDecodeHtmlSymbol(temp);
                }

                decodeString += temp;
                temp.clear();
                htmlEntityProcessing = false;
            }
        }
        else
        {
            decodeString += ch;
        }
    }
    decodeString += temp;

    return decodeString;
}

void DecodeInputToOutput(std::istream& input, std::ostream& output)
{
    std::string str;

    while (getline(input, str))
    {
        std::string strDecode = HtmlDecode(str);

        output << strDecode << std::endl;
    }

    if (!output.flush())
    {
        throw std::runtime_error("Failed to save data on disk\n");
    }
}
