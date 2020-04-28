// JSON.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include "json_parser.h"
#include "json_node.h"
#include <chrono>
using namespace std;
using namespace json;
static const uint32_t ntimes = 10;

int main(int argc, char **arg)
{   
    string s;
    
    if (argc==2){
        fstream f(arg[1],ios::in);
        ostringstream buf;
        char ch;
        while(buf&&f.get(ch)){
            //pass space to make comparison easy
            if (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r')
                continue;
            buf.put(ch);
        }
        s = buf.str();
        f.close();
    }
    else
        s ="{\"a\":{\"aa\":true},\"s\":[1,33],\"c\":null,\"d\":\"hello from\"}";
    
    json_node* node = nullptr;
    chrono::steady_clock::time_point start_time;
    chrono::steady_clock::time_point end_time;
    chrono::microseconds us;
    // try {

    start_time = chrono::steady_clock::now();
    for (uint32_t i = 0; i < ntimes; ++i) {
        json_parser parser(s);
        node = parser.parse();
        releaseNode(node);
        node = nullptr;
    }
    end_time = chrono::steady_clock::now();
    us=(chrono::duration_cast<chrono::microseconds>(
                               end_time - start_time));
    cout << "[+] Finished successfully with an average of: " << (us.count() / ntimes) << " us\n";
        
    // } catch (const std::exception& ex) {
    //     cerr << ex.what() << endl;
    //     exit(-1);
    // }
    
    // string ans;
    // json_parser parser(s);
    // node = parser.parse();
    // printNode(node,ans);
    // cout<<"equal = "<<(ans==s)<<endl;
    // releaseNode(node);
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
