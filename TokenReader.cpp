#include <string>
#include <cmath>
#include "JSONToken.h"
#include "TokenReader.h"


TokenReader::TokenReader(std::string& s)
{
    reader = CharReader(s);
}

static bool isSpace(char ch) {
    if (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r')
        return true;
    else
        return false;
}

Token TokenReader::readNext() {
    char c;
    Token token;
    if (!reader.hasMore())
        return END_DOCUMENT;
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
        case '"':
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
    char ch = reader.next();
    char ch_move;
    std::string predict;
    if (ch == 't')
        predict = "rue";
    else if (ch == 'f')
        predict = "alse";
    else{
        throw std::runtime_error("Unexpected boolean value");
    }
    for (int i=0; i < predict.length(); i++) {
        ch_move = this->reader.next();
        if (ch_move != predict.at(i)) {
            throw std::runtime_error("Unexpected boolean value");
        }
    }
    return ch == 't';
}

double TokenReader::NumberReader() {
    std::string intPart; 
	std::string fraPart; 
	std::string expPart;
	bool hasFraPart = false;
	bool hasExpPart = false;
	char ch = reader.peek();
	bool minus = (ch == '-');
	bool expMinus = false;
	if (minus) {
		reader.next();
	}
    int state = INT_PART;
    while(1){
        if (reader.hasMore()){
            ch = reader.peek();
        }
        else{
            state = NUM_END;
        }
        switch (state){
            case INT_PART:
                if (ch >= '0' && ch <= '9'){
					intPart.push_back(reader.next());
				} 
                else if (ch == '.') {
					if (intPart.empty()) {
						throw std::runtime_error("unexpected char .");
					}
					reader.next();
					hasFraPart = true;
					state = FRA_PART;
				} 
                else if (ch == 'e' || ch == 'E'){
                    if (intPart.empty()) {
						throw std::runtime_error("unexpected char e/E");
					}
					reader.next();
					hasExpPart = true;
					char signE = reader.peek();
					if (signE == '-' || signE == '+'){
						expMinus = (signE == '-');
						reader.next();
					}
					state = EXP_PART;
				} 
                else{
					if (intPart.empty()){
						throw std::runtime_error("unexpected char in int part");
					}
					state = NUM_END;
				}
				continue; 
            case FRA_PART:
                if (ch >= '0' && ch <= '9'){
					fraPart.push_back(reader.next());
				}
                else if (ch == 'e' || ch == 'E'){
					reader.next();
					hasExpPart = true;
					char signE = reader.peek();
					if (signE == '-' || signE == '+') {
						expMinus = (signE == '-');
						reader.next();
					}
					state = EXP_PART;
				}else{
					if (fraPart.empty()){
						throw std::runtime_error("unexpected char in frag part");
					}
					state = NUM_END;
				}
				continue;
            case EXP_PART:
				if (ch >= '0' && ch <= '9') {
					expPart.push_back(reader.next());
				} else {
					if (expPart.empty()) {
						throw std::runtime_error("unexpected char in exp part");
					}
					state = NUM_END;
				}
				continue;
            case NUM_END:
				if (intPart.empty())
				    throw std::runtime_error("No int part");
				int int_part = minus ? -stoi(intPart)
						: stoi(intPart);
				if (!hasFraPart && !hasExpPart) {
					return int_part;
				}
				if (hasFraPart && fraPart.empty()) {
				    throw std::runtime_error("No frag part");
				}
				double frag_part = hasFraPart ? (minus ? -stod("0."+fraPart):stod("0."+fraPart)):0.0;
				double number = hasExpPart? ((int_part + frag_part)* pow(10, expMinus ? -stoi(expPart):stoi(expPart)))
						:(int_part + frag_part);
				return number;
        }

    }
}

std::string TokenReader::StringReader() {
    std::string str;
    char c = reader.next();
    if (c != '"') {
        throw std::runtime_error("Unexpected string value: not \" ");
    }

    while (reader.hasMore()) {
        c = reader.next();
        if (c == '\\') {
            char escape = reader.next();
            switch (escape) {
                case '"':
                case '\\':
                case '/':
                    str.push_back(escape);
                    break;
                case 'b':
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
                    throw std::runtime_error("Unexpected string value in string");
            }
        } else if (c == '"') {
            break;
        } else if (c == '\r' || c == '\n')
            throw std::runtime_error("Unexpected string value \\r&&\\n");
        else
            str.push_back(c);
    }
    return str;
}

void TokenReader::NullReader() {
    char ch_move;
    std::string predict = "null";
    for (int i=0; i < predict.length(); i++) {
        ch_move = reader.next();
        if (ch_move != predict.at(i)) {
            throw std::runtime_error("Unexpected null value in null");
        }
    }
}

TokenReader::~TokenReader()
{
}