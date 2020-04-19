#include"JSONParser.h"
#include"JSONNode.h"

/*
Token readNext();
bool BooleanReader();
int NumberReader();
string StringReader();
void NullReader();
*/


JSONParser::JSONParser(std::string& s){
    tokenReader = new TokenReader(s);
}

JSONParser::~JSONParser(){
    delete tokenReader;
}
// TODO: maybe need to distinguish the string for key and the original string

json::JSONNode* JSONParser::parse() {
    stack = new ParseStack();
    status = EXPECT_SINGLE_VAL | EXPECT_BEGIN_OBJ | EXPECT_BEGIN_ARR;
	while (true) {
		Token token = tokenReader->readNext();
		switch (token){
		case BOOLEAN:
			if (onStatus(EXPECT_SINGLE_VAL)) {
				stack->push(json::JSONNode::createBoolNode(tokenReader->BooleanReader()));
				status = EXPECT_END_JSON;
				continue;
			}
			if (onStatus(EXPECT_OBJ_VAL)) {
				std::string key = stack->top()->getString();
				stack->pop();
				stack->top()->addToMap(key, json::JSONNode::createBoolNode(tokenReader->BooleanReader()));
				status = EXPECT_COMMA | EXPECT_END_OBJ;
				continue;
			}
			if (onStatus(EXPECT_OBJ_VAL)) {
				stack->top()->addToVec(json::JSONNode::createBoolNode(tokenReader->BooleanReader()));
				status = EXPECT_COMMA | EXPECT_END_ARR;
				continue;
			}
			// TODO: throw exception
			throw std::runtime_error("boolean failed");
		case NUMBER:
			if (onStatus(EXPECT_SINGLE_VAL)) {
				stack->push(json::JSONNode::createBoolNode(tokenReader->NumberReader()));
				status = EXPECT_END_JSON;
				continue;
			}
			if (onStatus(EXPECT_OBJ_VAL)) {
				std::string key = stack->top()->getString();
				stack->pop();
				stack->top()->addToMap(key, json::JSONNode::createBoolNode(tokenReader->NumberReader()));
				status = EXPECT_COMMA | EXPECT_END_OBJ;
				continue;
			}
			if (onStatus(EXPECT_OBJ_VAL)) {
				stack->top()->addToVec(json::JSONNode::createBoolNode(tokenReader->NumberReader()));
				status = EXPECT_COMMA | EXPECT_END_ARR;
				continue;
			}
			// TODO: throw exception
			throw std::runtime_error("number failed");
		case NUL:
			if (onStatus(EXPECT_SINGLE_VAL)) {
				stack->push(json::JSONNode::createNullNode());
				status = EXPECT_END_JSON;
				continue;
			}
			if (onStatus(EXPECT_OBJ_VAL)) {
				std::string key = stack->top()->getString();
				stack->pop();
				stack->top()->addToMap(key, json::JSONNode::createNullNode());
				status = EXPECT_COMMA | EXPECT_END_OBJ;
				continue;
			}
			if (onStatus(EXPECT_OBJ_VAL)) {
				stack->top()->addToVec(json::JSONNode::createNullNode());
				status = EXPECT_COMMA | EXPECT_END_ARR;
				continue;
			}
			// TODO: throw exception
			throw std::runtime_error("null failed");
		case STRING:
			if (onStatus(EXPECT_SINGLE_VAL)) {
				stack->push(json::JSONNode::createStrNode(tokenReader->StringReader()));
				status = EXPECT_END_JSON;
				continue;
			}
			if (onStatus(EXPECT_OBJ_KEY)) {
				stack->push(json::JSONNode::createStrNode(tokenReader->StringReader()));
				status = EXPECT_COLON;
				continue;
			}
			if (onStatus(EXPECT_OBJ_VAL)) {
				std::string key = stack->top()->getString();
				stack->pop();
				stack->top()->addToMap(key, json::JSONNode::createStrNode(tokenReader->StringReader()));
				status = EXPECT_COMMA | EXPECT_END_OBJ;
				continue;
			}
			if (onStatus(EXPECT_OBJ_VAL)) {
				stack->top()->addToVec(json::JSONNode::createStrNode(tokenReader->StringReader()));
				status = EXPECT_COMMA | EXPECT_END_ARR;
				continue;
			}
			// TODO: throw exception
			throw std::runtime_error("string failed");
		case SEP_COLON:
			if (status == EXPECT_COLON) {
				status = EXPECT_OBJ_VAL | EXPECT_BEGIN_OBJ | EXPECT_BEGIN_ARR;
				continue;
			}
			// TODO: throw exception
			throw std::runtime_error("sep colon failed");
		case SEP_COMMA:
			if (onStatus(EXPECT_COMMA)) {
				if (onStatus(EXPECT_END_OBJ)) {
					status = EXPECT_OBJ_KEY;
					continue;
				}
				if (onStatus(EXPECT_END_ARR)) {
					status = EXPECT_ARR_VAL | EXPECT_BEGIN_ARR | EXPECT_BEGIN_OBJ;
					continue;
				}
			}
			// TODO: throw exception
			throw std::runtime_error("sep comma failed");
		case BEGIN_ARRAY:
			if (onStatus(EXPECT_BEGIN_ARR)) {
				stack->push(json::JSONNode::createArrayNode());
				status = EXPECT_ARR_VAL | EXPECT_BEGIN_OBJ | EXPECT_BEGIN_ARR | EXPECT_END_ARR;
				continue;
			}
			// TODO: throw exception
			throw std::runtime_error("begin array failed");
		case BEGIN_OBJECT:
			if (onStatus(EXPECT_BEGIN_OBJ)) {
				stack->push(json::JSONNode::createObjectNode());
				status = EXPECT_OBJ_KEY | EXPECT_BEGIN_OBJ | EXPECT_END_OBJ;
				continue;
			}
			// TODO: throw exception
			throw std::runtime_error("begin object failed");
		case END_ARRAY:
			if (onStatus(EXPECT_END_ARR)) {
				if (stack->size() == 1) {
					status = EXPECT_END_JSON;
					continue;
				}
				json::JSONNode* tempArray = stack->top();
				stack->pop();
				json::Type type = stack->top()->getType();
				if (type == json::STRING) {
					// key: [ CURRENT ] ,}
					std::string key = stack->top()->getString();
					stack->pop();
					stack->top()->addToMap(key, tempArray);
					status = EXPECT_COMMA | EXPECT_END_OBJ;
					continue;
				}
				if (type == json::ARRAY) {
					// xx, xx, [CURRENT] ,]
					stack->top()->addToVec(tempArray);
					status = EXPECT_COMMA | EXPECT_END_ARR;
					continue;
				}
			}
			// TODO: throw exception
			throw std::runtime_error("end array failed");
		case END_OBJECT:
			if (onStatus(EXPECT_END_OBJ)) {
				if (stack->size() == 1) {
					status = EXPECT_END_JSON;
					continue;
				}
				json::JSONNode* tempObject = stack->top();
				stack->pop();
				json::Type type = stack->top()->getType();
				if (type == json::STRING) {
					// key: { CURRENT } ,}
					std::string key = stack->top()->getString();
					stack->pop();
					stack->top()->addToMap(key, tempObject);
					status = EXPECT_COMMA | EXPECT_END_OBJ;
					continue;
				}
				if (type == json::ARRAY) {
					// xx, xx, {CURRENT} ,]
					stack->top()->addToVec(tempObject);
					status = EXPECT_COMMA | EXPECT_END_ARR;
					continue;
				}
			}
			// TODO: throw exception
			throw std::runtime_error("end object failed");
		case END_DOCUMENT:
			if (onStatus(EXPECT_END_JSON)) {
				if (stack->size() == 1) {
					return stack->top();
				}
			}
			// TODO: throw exception 
			throw std::runtime_error("end document failed");
		default:
			break;
		}
	}
	delete stack;
}

inline bool JSONParser::onStatus(int status)
{
	return (this->status & status) != 0;
}
