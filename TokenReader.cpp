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
        case '}':
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
    if ( c == '-' || (c >= '0' && c <= '9') )
        token = NUMBER;

    return token;
}

bool TokenReader::BooleanReader() {
    char ch = this.reader.next();
    char ch_move;
    std::string predict;
    if (ch == 't')
        predict = "rue";
    else if (ch == 'f')
        predict = "alse";
    else
        throw "Unexpected boolean value";
    for (int i=0; i < predict.length(); i++) {
        ch_move = this.reader.next();
        if (ch_move != predict.at(i)) {
            throw "Unexpected boolean value";
        }
    }
    return ch == 't';
}

int TokenReader::NumberReader() {}

std::string TokenReader::StringReader() {
    std::string str;
    char c = this->reader.next();
    if (c != '\"') {
        throw "Unexpected sting value";
    }

    while (this->reader.hasMore()) {
        c = this->reader.next();
        if (c == '\\') {
            char escape = this->reader.next();
            switch (escape) {
                case '\"':
                case '\\':
                case '/':
                    str.push_back(escape);
                    break;
                case 'b:
                    str.push_back('\b');
                    break;
                case 'f':
                    str.push_back('\f');
                    break;
                case 'n':
                    str.push_back('\f');
                    break;
                case 'r':
                    str.push_back('\f');
                    break;
                case 't':
                    str.push_back('\f');
                    break;
                default:
                    throw "Unexpected string value";
            }
        } else if (c == '\"') {
            break;
        } else if (c == '\r' || c == '\n')
            throw "Unexpected string value '\r' or '\n'";
        else
            str.push_back(c);
    }
    return str;
}

void TokenReader::NullReader() {
    for (int i=0; i < predict.length(); i++) {
        ch_move = this.reader.next();
        if (ch_move != predict.at(i)) {
            throw "Unexpected null value";
        }
    }
}

TokenReader::~TokenReader()
{
}