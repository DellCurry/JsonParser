#pragma once
#include<string>

#include"JSONNode.h"

class JSONParser
{
public:
	JSONParser();
	~JSONParser();

	JSONNode parse(std::string jsonStr);

private:
    /**
     * Should read EOF for next token.
     */
    static const int STATUS_EXPECT_END_DOCUMENT = 1 << 0;

    /**
     * Should read "{" for next token.
     */
    static const int STATUS_EXPECT_BEGIN_OBJECT = 1 << 1;

    /**
     * Should read "}" for next token.
     */
    static const int STATUS_EXPECT_END_OBJECT = 1 << 2;

    /**
     * Should read object key for next token.
     */
    static const int STATUS_EXPECT_OBJECT_KEY = 1 << 3;

    /**
     * Should read object value for next token.
     */
    static const int STATUS_EXPECT_OBJECT_VALUE = 1 << 4;

    /**
     * Should read ":" for next token.
     */
    static const int STATUS_EXPECT_COLON = 1 << 5;

    /**
     * Should read "," for next token.
     */
    static const int STATUS_EXPECT_COMMA = 1 << 6;

    /**
     * Should read "[" for next token.
     */
    static const int STATUS_EXPECT_BEGIN_ARRAY = 1 << 7;

    /**
     * Should read "]" for next token.
     */
    static const int STATUS_EXPECT_END_ARRAY = 1 << 8;

    /**
     * Should read array value for next token.
     */
    static const int STATUS_EXPECT_ARRAY_VALUE = 1 << 9;

    /**
     * Should read a single value for next token (must not be "{" or "[").
     */
    static const int STATUS_EXPECT_SINGLE_VALUE = 1 << 10;

};

JSONParser::JSONParser()
{
}

JSONParser::~JSONParser()
{
}
