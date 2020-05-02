#include "json_node.h"
#include <stdexcept>

namespace json{

    json_node::json_node(node_type type) {
        this->type = type;
    }

    json_node::~json_node()
    {
    }

    node_map& json_node::get_node_map(){
        return this->u.map;
    }

    node_vec& json_node::get_node_vec(){
        return this->u.vec;
    }

    std::string& json_node::get_node_num(){
        return this->u.num;
    }

    std::vector<std::string>& json_node::get_seq_vec(){
        return this->seq_vec;
    }

    json_node* json_node::create_num_node(std::string num) {
        json_node* node = new json_node(NUMBER);
        new (&node->u.num) std::string(num);
        return node;
    }

    json_node* json_node::create_str_node(std::string str) {
        json_node* node = new json_node(STRING);
        new (&node->u.str) std::string(str);
        return node;
    }

    json_node* json_node::create_bool_node(bool b) {
        return new json_node(b ? TRUE : FALSE);
    }

    json_node* json_node::create_null_node() {
        return new json_node(NUL);
    }

    json_node* json_node::create_array_node() {
        json_node* node = new json_node(ARRAY);
        new (&node->u.vec) node_vec;
        //node->u.vec = node_vec();
        return node;
    }

    json_node* json_node::create_object_node() {
        json_node* node = new json_node(OBJECT);
        new (&node->u.map) node_map;
        //node->map["ssss"] = nullptr;
        //node->u.map = node_map();
        return node;
    }

    std::string& json_node::get_string() {
        return this->u.str;
    }

    node_type& json_node::get_node_type() {
        return this->type;
    }

	void json_node::add_to_seq_vec(std::string& key) {
        this->seq_vec.push_back(key);
	}

    void json_node::add_to_vec(json_node* node) {
        if (this->type == ARRAY) {
            this->u.vec.push_back(node);
        }
        else
        // TODO: throw exception
            throw std::runtime_error("type not equal to array");
    }

    void json_node::add_to_map(std::string& key, json_node* node) {
        if (this->type == OBJECT) {
            this->u.map[key] = node;
        }
        // TODO: throw exception
        else
            throw std::runtime_error("type not equal to object");
    }

    void printNode(json_node* node,std::string& ans){
        int seq = 0;
        node_map m;
        node_vec vec;
        std::vector<std::string> seq_vec;
        node_type t = node->get_node_type();
        std::unordered_map<std::string,json_node*>::iterator iter;
        switch (t){
            case OBJECT:
                ans.push_back('{');
                m = node->get_node_map();
                seq_vec = node->get_seq_vec();
                for (auto &i:seq_vec){
                    if (seq++>0) ans.push_back(',');
                    iter = m.find(i);
                    if (iter==m.end()) throw std::runtime_error("no such key in hashmap");
                    ans.insert(ans.size(),std::string('\"'+iter->first+'\"'+':'));
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
                ans.insert(ans.size(),node->get_node_num());
                break;
            case node_type::STRING:
                ans.push_back('\"');
                ans.insert(ans.size(),node->get_string());
                ans.push_back('\"');
                break;
            case TRUE:
                ans.insert(ans.size(),std::string("true"));
                break;
            case FALSE:
                ans.insert(ans.size(),std::string("false"));
                break;
            case node_type::NUL:
                ans.insert(ans.size(),std::string("null"));
                break;
            default:
                ans.insert(ans.size(),std::string("unknown"));
                break;
        }
}

    void releaseNode(json_node* node){
        node_map m;
        node_vec vec;
        node_type t = node->get_node_type();
        switch (t){
            case OBJECT:
                m = node->get_node_map();
                for (auto &i:m)
                    releaseNode(i.second);
                delete node;
                break;
            case ARRAY:
                vec = node->get_node_vec();
                for (auto &i:vec)
                    releaseNode(i);
                delete node;
                break;
            default:
                delete node;
        }
    }
};