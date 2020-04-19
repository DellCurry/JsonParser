#include <string>
#include <math>
#include "JSONToken.h"
#include "TokenReader.h"
#include "CharReader.h"


TokenReader::TokenReader(CharReader& reader)
{
    this.reader = reader;
}

Token TokenReader::readNext() {
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
            token = bool;
            break;
        case 'n':
            token = NUL;
            break;
    }
    if ( c >= '0' && c <= '9' )
        token = NUMBER;

    return token;
}

bool TokenReader::boolReader() {
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

double TokenReader::NumberReader() {
    string intPart; 
	string fraPart; 
	string expPart;
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
						throw bad_exception("unexpected char"+ch);
					}
					reader.next();
					hasFraPart = true;
					status = FRA_PART;
				} 
                else if (ch == 'e' || ch == 'E'){
                    if (intPart.empty()) {
						throw bad_exception("unexpected char"+ch);
					}
					reader.next();
					hasExpPart = true;
					char signE = reader.peek();
					if (signE == '-' || signE == '+'){
						expMinus = (signE == '-');
						reader.next();
					}
					status = EXP_PART;
				} 
                else{
					if (intPart.empty()){
						throw bad_exception("unexpected char"+ch);
					}
					status = NUM_END;
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
					status = EXP_PART;
				}else{
					if (fraPart.empty()){
						throw bad_exception("unexpected char"+ch);
					}
					status = NUM_END;
				}
				continue;
            case EXP_PART:
				if (ch >= '0' && ch <= '9') {
					expPart.push_back(reader.next());
				} else {
					if (expPart.empty()) {
						throw bad_exception("unexpected char"+ch);
					}
					status = NUM_END;
				}
				continue;
            case NUM_END:
				if (intPart.empty())
				    throw bad_exception("No int part");
				int int_part = minusSign ? -stoi(intPart)
						: stoi(intPart);
				if (!hasFraPart && !hasExpPart) {
					return int_part;
				}
				if (hasFraPart && fraPart == null) {
				    throw bad_exception("No frag part");
				}
				double frag_part = hasFraPart ? (minusSign ? -stod("0."+fraPart):stod("0."+fraPart)):0.0;
				double number = hasExpPart? ((int_part + frag_part)* pow(10, expMinusSign ? -stoi(expPart):stoi(expPart)))
						:(int_part + frag_part);
				return number;
        }

    }
}

string TokenReader::StringReader() {}

void TokenReader::NullReader() {}

TokenReader::~TokenReader()
{
}