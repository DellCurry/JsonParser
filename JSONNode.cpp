#include "JSONNode.h"

namespace json{

    JSONNode::JSONNode(Type type) {
        this->type = type;
    }

    JSONNode::~JSONNode()
    {
    }

    JSONNode* JSONNode::createNumNode(int num) {
        JSONNode* node = new JSONNode(NUMBER);
        node->num = num;
        return node;
    }

    JSONNode* JSONNode::createStrNode(std::string str) {
        JSONNode* node = new JSONNode(STRING);
        node->str = str;
        return node;
    }

    JSONNode* JSONNode::createBoolNode(bool b) {
        return new JSONNode(b ? TRUE : FALSE);
    }

    JSONNode* JSONNode::createNullNode() {
        return new JSONNode(NUL);
    }

    JSONNode* JSONNode::createArrayNode() {
        JSONNode* node = new JSONNode(ARRAY);
        node->nodeVec = NodeVec();
        return node;
    }

    JSONNode* JSONNode::createObjectNode() {
        JSONNode* node = new JSONNode(OBJECT);
        node->nodeMap = NodeMap();
        return node;
    }

    std::string JSONNode::getString() {
        return this->name;
    }

    Type JSONNode::getType() {
        return this->type;
    }

    void JSONNode::addToVec(JSONNode* node) {
        if (this->type == ARRAY) {
            this->nodeVec.push_back(node);
        }
        // TODO: throw exception
    }

    void JSONNode::addToMap(std::string key, JSONNode* node) {
        if (this->type == OBJECT) {
            this->nodeMap[key] = node;
        }
        // TODO: throw exception
    }

};