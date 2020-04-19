#pragma once
#include <string>
#include "CharReader.h"
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
    std::string StringReader();
    void NullReader();
};

