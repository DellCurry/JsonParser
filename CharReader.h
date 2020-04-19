#pragma once
#include<string>


class CharReader
{
public:
	CharReader();
	~CharReader();

	char next();
	char peek();
	std::string next(int n);
	bool more();

private:
	std::string jsonStr;

};

CharReader::CharReader()
{
}

CharReader::~CharReader()
{
}