// JSON.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "json_parser.h"
#include "json_node.h"
using namespace std;
using namespace json;

void printNode(json_node* node){
    int seq = 0;
    node_map m;
    node_vec vec;
    node_type t = node->get_node_type();
    switch (t){
        case OBJECT:
            cout<<'{';
            m = node->get_node_map();
            for (auto &i:m){
                if (seq++>0) cout<<',';
                cout<<'\''<<i.first<<'\''<<':';
                printNode(i.second);
            }
            cout<<'}';
            break;
        case ARRAY:
            vec = node->get_node_vec();
            cout<<'[';
            for (auto &i:vec){
                if (seq++>0) cout<<',';
                printNode(i);
            }
            cout<<']';
            break;
        case node_type::NUMBER:
            cout<<node->get_node_num();
            break;
        case node_type::STRING:
            cout<<node->get_string();
            break;
        case TRUE:
            cout<<"true";
            break;
        case FALSE:
            cout<<"false";
            break;
        case node_type::NUL:
            cout<<"null";
            break;
        default:
            cout<<"unknown";
            break;
    }
}

int main()
{
    string s ="{\"a\":{\"aa\":true},\"b\":[1,33],\"c\":null, \"d\":\"hello from \\u007A\"}";
    json_parser parser(s);
    json_node* node = nullptr;
    try {
        node = parser.parse();
    } catch (const std::exception& ex) {
        cerr << ex.what() << endl;
        exit(-1);
    }
    
    // cout<<node->getType()<<endl;
    // cout<<"size="<<m.size()<<endl;
    printNode(node);
    cout<<endl;
    releaseNode(node);
}

/*
typedef enum {
    END_JSON,
    BEGIN_OBJ,
    END_OBJ,
    BEGIN_ARR,
    END_ARR,
    COLON,
    COMMA,
    NUMBER,
    STRING,
    BOOL,
    NUL
} token_type;
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
