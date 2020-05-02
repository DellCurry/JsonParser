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
static const uint32_t ntimes = 100;

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

    /*
     * Performance Test
     */
    chrono::steady_clock::time_point start_time;
    chrono::steady_clock::time_point end_time;
    chrono::microseconds us;
    
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
    
    /*
     * Function Test
     */
    // try {
    //     string ans;
    //     json_parser parser(s);
    //     node = parser.parse();
    //     printNode(node,ans);
    //     cout<<"equal = "<<(ans==s)<<endl;
    //     releaseNode(node);
    // } catch (const std::exception& ex) {
    //     cerr << ex.what() << endl;
    //     exit(-1);
    // }
}

