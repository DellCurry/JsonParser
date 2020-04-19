#include "CharReader.h"

CharReader::CharReader(string& s){
    jsonStr = s;
    pos = 0;
    size = jsonStr.size();
}

CharReader::~CharReader(){

}

bool CharReader::hasMore(){
    return pos<size;
}

bool CharReader::hasMore(int n){
    return pos+n-1<size;
}

char CharReader::next(){
    if (pos==size)
        throw out_of_range("no next char");
    return jsonStr[pos++];
}

string CharReader::next(int n){
    if (pos+n-1==size)
        throw out_of_range("no next char");
    string ans(jsonStr,pos,n);
    pos+=n;
    return ans;
}

char CharReader::peek(){
    if (pos==size)
        throw out_of_range("no next char");
    return jsonStr[pos];
}
