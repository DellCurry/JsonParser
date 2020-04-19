// JSON.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "JSONParser.h"
#include "JSONNode.h"
using namespace std;
using namespace json;
int main()
{
    string s ="{\"a\":{\"aa\":true},\"b\":[1,33],\"c\":null}";
    JSONParser parser(s);
    JSONNode* node = parser.parse();
    NodeMap m = node->getNodeMap();
    // cout<<node->getType()<<endl;
    // cout<<"size="<<m.size()<<endl;
    for (auto i:m){
        cout<<i.first<<endl;
    }

}

/*
typedef enum {
    END_DOCUMENT,0
    BEGIN_OBJECT,1
    END_OBJECT,2
    BEGIN_ARRAY,3
    END_ARRAY,4
    SEP_COLON,5
    SEP_COMMA,6
    NUMBER,7
    STRING,8
    BOOLEAN,9
    NUL10
} Token;
*/

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
