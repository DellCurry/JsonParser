#pragma once
#include<string>
#include<unordered_map>
#include<vector>

namespace json{
    class JSONNode;
    typedef std::unordered_map<std::string, JSONNode*> NodeMap;
    typedef std::vector<JSONNode*> NodeVec;
    typedef enum {
            NUMBER,
            STRING,
            ARRAY,
            OBJECT,
            TRUE,
            FALSE,
            NUL,
            UNKNOWN
        } Type;

    class JSONNode
    {
    public:
        JSONNode();
        ~JSONNode();
    private:
        Type type;
        std::string name;
        union {
            double num;
            std::string str;
            NodeMap nodeMap;
            NodeVec nodeVec;
        };
    };

};