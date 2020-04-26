// JSON.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "json_parser.h"
#include "json_node.h"
using namespace std;
using namespace json;

void printNode(json_node* node,string& ans){
    int seq = 0;
    node_map m;
    node_vec vec;
    vector<string> seq_vec;
    node_type t = node->get_node_type();
    unordered_map<string,json_node*>::iterator iter;
    switch (t){
        case OBJECT:
            ans.push_back('{');
            m = node->get_node_map();
            seq_vec = node->get_seq_vec();
            for (auto &i:seq_vec){
                if (seq++>0) ans.push_back(',');
                iter = m.find(i);
                if (iter==m.end()) throw std::runtime_error("no such key in hashmap");
                ans.insert(ans.size(),string('\"'+iter->first+'\"'+':'));
                printNode(iter->second,ans);
            }
            ans.push_back('}');
            break;
        case ARRAY:
            vec = node->get_node_vec();
            ans.push_back('[');
            for (auto &i:vec){
                if (seq++>0) ans.push_back(',');
                printNode(i,ans);
            }
            ans.push_back(']');
            break;
        case node_type::NUMBER:
            ans.insert(ans.size(),to_string((int)node->get_node_num()));
            break;
        case node_type::STRING:
            ans.push_back('\"');
            ans.insert(ans.size(),node->get_string());
            ans.push_back('\"');
            break;
        case TRUE:
            ans.insert(ans.size(),string("true"));
            break;
        case FALSE:
            ans.insert(ans.size(),string("false"));
            break;
        case node_type::NUL:
            ans.insert(ans.size(),string("null"));
            break;
        default:
            ans.insert(ans.size(),string("unknown"));
            break;
    }
}

int main()
{
    string s ="{\"a\":{\"aa\":true},\"s\":[1,33],\"c\":null,\"d\":\"hello from\"}";
    json_parser parser(s);
    json_node* node = nullptr;
    try {
        node = parser.parse();
    } catch (const std::exception& ex) {
        cerr << ex.what() << endl;
        exit(-1);
    }
    
    string ans;
    printNode(node,ans);
    cout<<ans;
    cout<<endl;
    cout<<s<<endl;
    cout<<"equal = "<<(ans==s)<<endl;
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
