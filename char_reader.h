#pragma once
#include <string>
#include <iostream>
#include <string>
class char_reader {
public:
	char_reader();
	char_reader(std::string& s);
	char_reader(char_reader& reader);
	~char_reader();
	char next();
	char peek();
	std::string next(int);
	bool more();
	bool more(int);
	int get_pos();
	std::string get_readed();
	void catch_error(int);

private:
	std::string json_str;
	int pos;
	int size;

};
