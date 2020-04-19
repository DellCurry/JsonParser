#ifndef ChARREADER_H
#define CHARREADER_H

#pragma once
#include <string>
#include <iostream>
#include <string>
using namespace std;
class CharReader
{
public:
	CharReader(string& s);
	~CharReader();

	char next();
	char peek();
	string next(int);
	bool more();
	bool more(int);
	void catchError(int);

private:
	string jsonStr;
	int pos;
	int size;

};

#endif