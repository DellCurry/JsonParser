#pragma once
#include <string>
#include "CharReader.h"
#include "JSONToken.h"

class TokenReader
{
public:
    CharReader reader;
	TokenReader(std::string&);
	~TokenReader();
    Token readNext();
    bool BooleanReader();
    double NumberReader();
    std::string StringReader();
    void NullReader();
};

