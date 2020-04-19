#include "CharReader.h"

CharReader::CharReader(string& s){
    jsonStr = s;
    pos = 0;
    size = jsonStr.size();
}

CharReader::~CharReader(){

}

void CharReader::catchError(int pos){
    if (pos>=size)
        throw "out of range";
}

bool CharReader::more(){
    return pos<size;
}

bool CharReader::more(int n){
    return pos+n-1<size;
}

char CharReader::next(){
    /*try{
        catchError(pos);
    }
    catch(const char* msg){
        cout<<msg<<endl;
    }*/
    return jsonStr[pos++];
}

string CharReader::next(int n){
    /*try{
        catchError(pos+n-1);
    }
    catch(const char* msg){
        cout<<msg<<endl;
    }*/
    string ans(jsonStr,pos,n);
    pos+=n;
    return ans;
}

char CharReader::peek(){
    /*try{
        catchError(pos);
    }
    catch(const char* msg){
        cout<<msg<<endl;
    }*/
    return jsonStr[pos];
}
