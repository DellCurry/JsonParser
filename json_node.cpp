#include "json_node.h"
#include <stdexcept>

namespace json{

    json_node::json_node(node_type type) {
        this->type = type;
    }

    json_node::~json_node()
    {
    }

    node_map json_node::get_node_map(){
        return this->u.map;
    }

    node_vec json_node::get_node_vec(){
        return this->u.vec;
    }

    std::string json_node::get_node_num(){
        return this->u.num;
    }

    std::vector<std::string> json_node::get_seq_vec(){
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

    std::string json_node::get_string() {
        return this->u.str;
    }

    node_type json_node::get_node_type() {
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


    void releaseNode(json_node* node){
        int seq = 0;
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