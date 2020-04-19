#pragma once
#include <string>
#include "JSONToken.h"

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
    bool isSpace(char ch);
};

bool TokenReader::isSpace(char ch) {
    if (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r')
        return True
    else
        return False
}

TokenReader::TokenReader(CharReader reader)
{
    this.reader = reader;
}

TokenReader::readNext() {
    for (;;) {
        
    }
}

bool TokenReader::BooleanReader() {
    char ch = reader.next();
    char ch_move;
    string predict;
    if (ch == 't')
        predict = "rue";
    else if (ch == 'f')
        predict = "alse";
    else
        return -1;
    for (int i=0; i < predict.length(); i++) {
        ch_move = reader.next();
        if (ch_move != predict.at(i)) {
            return -1;
        }
    }
}

int TokenReader::NumberReader() {}

string TokenReader::StringReader() {}

void TokenReader::NullReader() {}

TokenReader::~TokenReader()
{
}