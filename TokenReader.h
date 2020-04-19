#pragma once
#include <string>
#include "CharReader.h"

class TokenReader
{
public:
    CharReader reader;
	TokenReader(CharReader reader);
	~TokenReader();

    Token readNext();
    bool BooleanReader();
    int NumberReader();
    string StringReader();
    void NullReader();
private:
    bool isSpace(char ch) return ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r';
};
