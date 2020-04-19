#pragma once
#include <string>
#include "JSONToken.h"

class TokenReader
{
public:
    CharReader reader;
	TokenReader(CharReader& reader);
	~TokenReader();

    Token readNext();
    bool BooleanReader();
    double NumberReader();
    string StringReader();
    void NullReader();
private:
    bool isSpace(char ch);
};

bool TokenReader::isSpace(char ch) {
    if (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r')
        return true
    else
        return false
}