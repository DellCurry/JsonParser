#include <string>
#include "JSONToken.h"
#include "TokenReader.h"


TokenReader::TokenReader(CharReader reader)
{
    this.reader = reader;
}

TokenReader::readNext() {
    char c;
    Token token;
    while (this->reader.hasMore()) {
        c = this->reader.peek();
        if (isSpace(c))
            this->reader.next();
        else
            break;
    }
    switch (c) {
        case '{':
            token = BEGIN_OBJECT;
            this->reader.next();
            break;
        case '}:
            token = END_OBJECT;
            this->reader.next();
            break;
        case ':':
            token = SEP_COLON;
            this->reader.next();
            break;
        case ',':
            token = SEP_COMMA;
            this->reader.next();
            break;
        case '[':
            token = BEGIN_ARRAY;
            this->reader.next();
            break;
        case ']':
            token = END_ARRAY;
            this->reader.next();
            break;
        case '\"':
            token = STRING;
            break;
        case 't':
        case 'f':
            token = BOOLEAN;
            break;
        case 'n':
            token = NUL;
            break;
    }
    if ( c >= '0' && c <= '9' )
        token = NUMBER;

    return token;
}

bool TokenReader::BooleanReader() {
    char ch = this.reader.next();
    char ch_move;
    string predict;
    if (ch == 't')
        predict = "rue";
    else if (ch == 'f')
        predict = "alse";
    else
        return -1;
    for (int i=0; i < predict.length(); i++) {
        ch_move = this.reader.next();
        if (ch_move != predict.at(i)) {
            return -1;
        }
    }
}

int TokenReader::NumberReader() {
    
}

string TokenReader::StringReader() {}

void TokenReader::NullReader() {}

TokenReader::~TokenReader()
{
}