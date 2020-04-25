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
        return this->map;
    }

    node_vec json_node::get_node_vec(){
        return this->vec;
    }

    double json_node::get_node_num(){
        return this->num;
    }

    json_node* json_node::create_num_node(int num) {
        json_node* node = new json_node(NUMBER);
        node->num = num;
        return node;
    }

    json_node* json_node::create_str_node(std::string str) {
        json_node* node = new json_node(STRING);
        node->str = str;
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
        node->vec = node_vec();
        return node;
    }

    json_node* json_node::create_object_node() {
        json_node* node = new json_node(OBJECT);
        node->map = node_map();
        return node;
    }

    std::string json_node::get_string() {
        return this->str;
    }

    node_type json_node::get_node_type() {
        return this->type;
    }

    void json_node::add_to_vec(json_node* node) {
        if (this->type == ARRAY) {
            this->vec.push_back(node);
        }
        else
        // TODO: throw exception
            throw std::runtime_error("type not equal to array");
    }

    void json_node::add_to_map(std::string key, json_node* node) {
        if (this->type == OBJECT) {
            this->map[key] = node;
        }
        // TODO: throw exception
        else
            throw std::runtime_error("type not equal to object");
    }

};