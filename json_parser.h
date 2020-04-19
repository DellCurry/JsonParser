#pragma once
#include<string>
#include<stack>

#include"json_node.h"

#include"token_reader.h"

typedef std::stack<json::json_node*> parser_stack;

class json_parser
{
public:
	json_parser(std::string&);
	~json_parser();

    json::json_node* parse();

private:
    token_reader* tk_reader;

    int status;
    parser_stack* stack;

    bool on_status(int status);

    /**
     * Should read EOF for next token.
     */
    static const int EXPECT_END_JSON = 1 << 0;

    /**
     * Should read "{" for next token.
     */
    static const int EXPECT_BEGIN_OBJ = 1 << 1;

    /**
     * Should read "}" for next token.
     */
    static const int EXPECT_END_OBJ = 1 << 2;

    /**
     * Should read object key for next token.
     */
    static const int EXPECT_OBJ_KEY = 1 << 3;

    /**
     * Should read object value for next token.
     */
    static const int EXPECT_OBJ_VAL = 1 << 4;

    /**
     * Should read ":" for next token.
     */
    static const int EXPECT_COLON = 1 << 5;

    /**
     * Should read "," for next token.
     */
    static const int EXPECT_COMMA = 1 << 6;

    /**
     * Should read "[" for next token.
     */
    static const int EXPECT_BEGIN_ARR = 1 << 7;

    /**
     * Should read "]" for next token.
     */
    static const int EXPECT_END_ARR = 1 << 8;

    /**
     * Should read array value for next token.
     */
    static const int EXPECT_ARR_VAL = 1 << 9;

    /**
     * Should read a single value for next token (must not be "{" or "[").
     */
    static const int EXPECT_SINGLE_VAL = 1 << 10;

};

