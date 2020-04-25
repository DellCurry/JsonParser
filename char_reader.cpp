#include "char_reader.h"

char_reader::char_reader(){
    
}

char_reader::char_reader(std::string& s){
    json_str = s;
    pos = 0;
    size = json_str.size();
}

char_reader::char_reader(char_reader& reader){
    std::string json_str = reader.json_str;
	int pos = reader.pos;
	int size = reader.size;
}

char_reader::~char_reader(){

}

bool char_reader::more(){
    return pos<size;
}

bool char_reader::more(int n){
    return pos+n-1<size;
}

int char_reader::get_pos() {
    return pos;
}

std::string char_reader::get_readed() {
    return json_str.substr(0, pos);
}

char char_reader::next(){
    if (pos==size)
        throw std::out_of_range("no next char");
    return json_str[pos++];
}

std::string char_reader::next(int n){
    if (pos+n-1==size)
        throw std::out_of_range("no next char");
    std::string ans(json_str,pos,n);
    pos+=n;
    return ans;
}

char char_reader::peek(){
    if (pos==size)
        throw std::out_of_range("no next char");
    return json_str[pos];
}
