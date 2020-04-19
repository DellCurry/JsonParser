#include"JSONParser.h"
#include"JSONNode.h"

/*
Token readNext();
bool BooleanReader();
int NumberReader();
string StringReader();
void NullReader();
*/

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
		case SEP_COLON:
			if (status == EXPECT_COLON) {
				status = EXPECT_OBJ_VAL | EXPECT_BEGIN_OBJ | EXPECT_BEGIN_ARR;
				continue;
			}
			// TODO: throw exception
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
		case BEGIN_ARRAY:
			if (onStatus(EXPECT_BEGIN_ARR)) {
				stack->push(json::JSONNode::createArrayNode());
				status = EXPECT_ARR_VAL | EXPECT_BEGIN_OBJ | EXPECT_BEGIN_ARR | EXPECT_END_ARR;
				continue;
			}
			// TODO: throw exception
		case BEGIN_OBJECT:
			if (onStatus(EXPECT_BEGIN_OBJ)) {
				stack->push(json::JSONNode::createObjectNode());
				status = EXPECT_OBJ_KEY | EXPECT_BEGIN_OBJ | EXPECT_END_OBJ;
				continue;
			}
			// TODO: throw exception
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
		case END_DOCUMENT:
			if (onStatus(EXPECT_END_JSON)) {
				if (stack->size() == 1) {
					return stack->top();
				}
			}
			// TODO: throw exception 
		default:
			break;
		}
	}
}

inline bool JSONParser::onStatus(int status)
{
	return (this->status & status) != 0;
}
