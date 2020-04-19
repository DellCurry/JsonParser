#pragma once
#include <string>
#include "char_reader.h"
#include "json_token.h"

class token_reader
{
public:
    char_reader reader;
	token_reader(std::string&);
	~token_reader();
    token_type read_next();
    bool bool_reader();
    double number_reader();
    std::string string_reader();
    void null_reader();
};

