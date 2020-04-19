#pragma once
#include<string>
#include<unordered_map>
#include<vector>

typedef std::unordered_map<std::string, JSONNode> NodeMap;
typedef std::vector<JSONNode> NodeVec;

class JSONNode
{
public:
	JSONNode();
	~JSONNode();

private:
    enum {
        NUMBER,
        STRING,
        ARRAY,
        OBJECT,
        TRUE,
        FALSE,
        NUL,
        UNKNOWN
    } type;
    std::string name;
    union {
        double num;
        std::string str;
        NodeMap nodeMap;
        NodeVec nodeVec;
    };
};

JSONNode::JSONNode()
{
}

JSONNode::~JSONNode()
{
}