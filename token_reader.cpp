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
        default:
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
        throw std::runtime_error("Unexpected boolean character: "
                                 + std::string(1, ch));
    }
    for (char i : predict) {
        ch_move = this->reader.next();
        if (ch_move != i) {
            throw std::runtime_error("Unexpected boolean character: "
                                     + std::string(1, ch_move));
        }
    }
    return ch == 't';
}

std::string token_reader::number_reader() {
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
    while(true){
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
						throw std::runtime_error("Unexpected numeric char: .");
					}
					reader.next();
					hasFraPart = true;
					state = FRA_PART;
				} 
                else if (ch == 'e' || ch == 'E'){
                    if (intPart.empty()) {
                        throw std::runtime_error("Unexpected numeric character: "
                                                 + std::string(1, ch));
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
                        throw std::runtime_error("Unexpected char in int part: "
                                                 + std::string(1, ch));
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
                        throw std::runtime_error("Unexpected char in frag part: "
                                                 + std::string(1, ch));
					}
					state = NUM_END;
				}
				continue;
            case EXP_PART:
				if (ch >= '0' && ch <= '9') {
					expPart.push_back(reader.next());
				} else {
					if (expPart.empty()) {
                        throw std::runtime_error("Unexpected char in exp part: "
                                                 + std::string(1, ch));
					}
					state = NUM_END;
				}
				continue;
            case NUM_END:
				if (intPart.empty())
				    throw std::runtime_error("No int part");
				std::string int_part = minus ? ('-'+intPart)
						: intPart;
				if (!hasFraPart && !hasExpPart) {
					return int_part;
				}
				if (hasFraPart && fraPart.empty()) {
				    throw std::runtime_error("No frag part");
				}
				std::string num_part = int_part + (hasFraPart ? ('.'+fraPart):std::string());
				std::string number = hasExpPart? (num_part + 'e' + (expMinus?('-'+expPart):expPart))
						                        :num_part;
				return number;
        }

    }
}

std::string token_reader::string_reader() {
    std::string str;
    int u = 0; // for unicode
    char c = reader.next();
    if (c != '"') {
        throw std::runtime_error("Unexpected string character: not \" ");
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
                case 'n':
                case 'r':
                case 't':
                    str.push_back('\f');
                    break;
                case 'u':
                    // unicode
                    for (int i = 0; i < 4; i++) {
                        char uch = reader.next();
                        if (uch >= '0' && uch <= '9') {
                            u = (u << 4) + (uch - '0');
                        } else if (uch >= 'a' && uch <= 'f') {
                            u = (u << 4) + (uch - 'a') + 10;
                        } else if (uch >= 'A' && uch <= 'F') {
                            u = (u << 4) + (uch - 'A') + 10;
                        } else {
                            throw std::runtime_error("Unexpected unicode character: "
                                                     + std::string(1, uch));
                        }
                    }

                    str.push_back((char16_t) u);
                    break;
                default:
                    throw std::runtime_error("Unexpected string value in string: " + std::string(1, escape));
            }
        } else if (c == '"') {
            break;
        } else if (c == '\r' || c == '\n')
            throw std::runtime_error("Unexpected string character: \\r&&\\n");
        else
            str.push_back(c);
    }
    return str;
}

void token_reader::null_reader() {
    char ch_move;
    std::string predict = "null";
    for (char i : predict) {
        ch_move = reader.next();
        if (ch_move != i) {
            throw std::runtime_error("Unexpected null character in null: " + std::string(1, ch_move));
        }
    }
}

token_reader::~token_reader()
= default;