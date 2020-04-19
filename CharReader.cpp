#include "CharReader.h"
CharReader::CharReader(){
    
}
CharReader::CharReader(std::string& s){
    jsonStr = s;
    pos = 0;
    size = jsonStr.size();
}

CharReader::CharReader(CharReader& reader){
    std::string jsonStr = reader.jsonStr;
	int pos = reader.pos;
	int size = reader.size;
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
        throw std::out_of_range("no next char");
    return jsonStr[pos++];
}

std::string CharReader::next(int n){
    if (pos+n-1==size)
        throw std::out_of_range("no next char");
    std::string ans(jsonStr,pos,n);
    pos+=n;
    return ans;
}

char CharReader::peek(){
    if (pos==size)
        throw std::out_of_range("no next char");
    return jsonStr[pos];
}
