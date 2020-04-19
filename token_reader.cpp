#include <string>
#include <cmath>
#include "json_token.h"
#include "token_reader.h"


token_reader::token_reader(std::string& s)
{
    reader = char_reader(s);
}

inline bool is_space(char ch) {
    return (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r');
}

token_type token_reader::read_next() {
    char c;
    token_type token;
    if (!reader.more())
        return END_JSON;
    while (this->reader.more()) {
        c = this->reader.peek();
        if (is_space(c))
            this->reader.next();
        else
            break;
    }
    switch (c) {
        case '{':
            token = BEGIN_OBJ;
            this->reader.next();
            break;
        case '}':
            token = END_OBJ;
            this->reader.next();
            break;
        case ':':
            token = COLON;
            this->reader.next();
            break;
        case ',':
            token = COMMA;
            this->reader.next();
            break;
        case '[':
            token = BEGIN_ARR;
            this->reader.next();
            break;
        case ']':
            token = END_ARR;
            this->reader.next();
            break;
        case '"':
            token = STRING;
            break;
        case 't':
        case 'f':
            token = BOOL;
            break;
        case 'n':
            token = NUL;
            break;
    }
    if ( c == '-' || (c >= '0' && c <= '9') )
        token = NUMBER;

    return token;
}

bool token_reader::bool_reader() {
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

double token_reader::number_reader() {
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
        if (reader.more()){
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

std::string token_reader::string_reader() {
    std::string str;
    char c = reader.next();
    if (c != '"') {
        throw std::runtime_error("Unexpected string value: not \" ");
    }

    while (reader.more()) {
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

void token_reader::null_reader() {
    char ch_move;
    std::string predict = "null";
    for (int i=0; i < predict.length(); i++) {
        ch_move = reader.next();
        if (ch_move != predict.at(i)) {
            throw std::runtime_error("Unexpected null value in null");
        }
    }
}

token_reader::~token_reader()
{
}