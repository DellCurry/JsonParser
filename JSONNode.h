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
        JSONNode(Type type);
        ~JSONNode();

        static JSONNode* createNumNode(int num);
        static JSONNode* createStrNode(std::string str);
        static JSONNode* createBoolNode(bool b);
        static JSONNode* createNullNode();
        static JSONNode* createArrayNode();
        static JSONNode* createObjectNode();

        NodeMap getNodeMap();
        std::string getString();
        Type getType();
        void addToVec(JSONNode* node);
        void addToMap(std::string key, JSONNode* node);
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