#include"json_parser.h"
#include"json_node.h"

/*
token_type read_next();
bool bool_reader();
int number_reader();
string string_reader();
void null_reader();
*/


json_parser::json_parser(std::string& s){
    tk_reader = new token_reader(s);
}

json_parser::~json_parser(){
    delete tk_reader;
}
// TODO: maybe need to distinguish the string for key and the original string

json::json_node* json_parser::parse() {
    stack = new parser_stack();
    status = EXPECT_SINGLE_VAL | EXPECT_BEGIN_OBJ | EXPECT_BEGIN_ARR;
	while (true) {
		token_type token = tk_reader->read_next();
		switch (token){
		case BOOL:
			if (on_status(EXPECT_SINGLE_VAL)) {
				stack->push(json::json_node::create_bool_node(tk_reader->bool_reader()));
				status = EXPECT_END_JSON;
				continue;
			}
			if (on_status(EXPECT_OBJ_VAL)) {
				std::string key = stack->top()->get_string();
				stack->pop();
				stack->top()->add_to_map(key, json::json_node::create_bool_node(tk_reader->bool_reader()));
				status = EXPECT_COMMA | EXPECT_END_OBJ;
				continue;
			}
			if (on_status(EXPECT_ARR_VAL)) {
				stack->top()->add_to_vec(json::json_node::create_bool_node(tk_reader->bool_reader()));
				status = EXPECT_COMMA | EXPECT_END_ARR;
				continue;
			}
			// TODO: throw exception
			throw std::runtime_error("boolean failed");
		case NUMBER:
			if (on_status(EXPECT_SINGLE_VAL)) {
				stack->push(json::json_node::create_bool_node(tk_reader->number_reader()));
				status = EXPECT_END_JSON;
				continue;
			}
			if (on_status(EXPECT_OBJ_VAL)) {
				std::string key = stack->top()->get_string();
				stack->pop();
				stack->top()->add_to_map(key, json::json_node::create_bool_node(tk_reader->number_reader()));
				status = EXPECT_COMMA | EXPECT_END_OBJ;
				continue;
			}
			if (on_status(EXPECT_ARR_VAL)) {
				stack->top()->add_to_vec(json::json_node::create_bool_node(tk_reader->number_reader()));
				status = EXPECT_COMMA | EXPECT_END_ARR;
				continue;
			}
			// TODO: throw exception
			throw std::runtime_error("number failed");
		case NUL:
			if (on_status(EXPECT_SINGLE_VAL)) {
				tk_reader->null_reader();
				stack->push(json::json_node::create_null_node());
				status = EXPECT_END_JSON;
				continue;
			}
			if (on_status(EXPECT_OBJ_VAL)) {
				tk_reader->null_reader();
				std::string key = stack->top()->get_string();
				stack->pop();
				stack->top()->add_to_map(key, json::json_node::create_null_node());
				status = EXPECT_COMMA | EXPECT_END_OBJ;
				continue;
			}
			if (on_status(EXPECT_ARR_VAL)) {
				tk_reader->null_reader();
				stack->top()->add_to_vec(json::json_node::create_null_node());
				status = EXPECT_COMMA | EXPECT_END_ARR;
				continue;
			}
			// TODO: throw exception
			throw std::runtime_error("null failed");
		case STRING:
			if (on_status(EXPECT_SINGLE_VAL)) {
				stack->push(json::json_node::create_str_node(tk_reader->string_reader()));
				status = EXPECT_END_JSON;
				continue;
			}
			if (on_status(EXPECT_OBJ_KEY)) {
				stack->push(json::json_node::create_str_node(tk_reader->string_reader()));
				status = EXPECT_COLON;
				continue;
			}
			if (on_status(EXPECT_OBJ_VAL)) {
				std::string key = stack->top()->get_string();
				stack->pop();
				stack->top()->add_to_map(key, json::json_node::create_str_node(tk_reader->string_reader()));
				status = EXPECT_COMMA | EXPECT_END_OBJ;
				continue;
			}
			if (on_status(EXPECT_ARR_VAL)) {
				stack->top()->add_to_vec(json::json_node::create_str_node(tk_reader->string_reader()));
				status = EXPECT_COMMA | EXPECT_END_ARR;
				continue;
			}
			// TODO: throw exception
			throw std::runtime_error("string failed");
		case COLON:
			if (status == EXPECT_COLON) {
				status = EXPECT_OBJ_VAL | EXPECT_BEGIN_OBJ | EXPECT_BEGIN_ARR;
				continue;
			}
			// TODO: throw exception
			throw std::runtime_error("sep colon failed");
		case COMMA:
			if (on_status(EXPECT_COMMA)) {
				if (on_status(EXPECT_END_OBJ)) {
					status = EXPECT_OBJ_KEY;
					continue;
				}
				if (on_status(EXPECT_END_ARR)) {
					status = EXPECT_ARR_VAL | EXPECT_BEGIN_ARR | EXPECT_BEGIN_OBJ;
					continue;
				}
			}
			// TODO: throw exception
			throw std::runtime_error("sep comma failed");
		case BEGIN_ARR:
			if (on_status(EXPECT_BEGIN_ARR)) {
				stack->push(json::json_node::create_array_node());
				status = EXPECT_ARR_VAL | EXPECT_BEGIN_OBJ | EXPECT_BEGIN_ARR | EXPECT_END_ARR;
				continue;
			}
			// TODO: throw exception
			throw std::runtime_error("begin array failed");
		case BEGIN_OBJ:
			if (on_status(EXPECT_BEGIN_OBJ)) {
				stack->push(json::json_node::create_object_node());
				status = EXPECT_OBJ_KEY | EXPECT_BEGIN_OBJ | EXPECT_END_OBJ;
				continue;
			}
			// TODO: throw exception
			throw std::runtime_error("begin object failed");
		case END_ARR:
			if (on_status(EXPECT_END_ARR)) {
				if (stack->size() == 1) {
					status = EXPECT_END_JSON;
					continue;
				}
				json::json_node* tempArray = stack->top();
				stack->pop();
				json::node_type type = stack->top()->get_node_type();
				if (type == json::STRING) {
					// key: [ CURRENT ] ,}
					std::string key = stack->top()->get_string();
					stack->pop();
					stack->top()->add_to_map(key, tempArray);
					status = EXPECT_COMMA | EXPECT_END_OBJ;
					continue;
				}
				if (type == json::ARRAY) {
					// xx, xx, [CURRENT] ,]
					stack->top()->add_to_vec(tempArray);
					status = EXPECT_COMMA | EXPECT_END_ARR;
					continue;
				}
			}
			// TODO: throw exception
			throw std::runtime_error("end array failed");
		case END_OBJ:
			if (on_status(EXPECT_END_OBJ)) {
				if (stack->size() == 1) {
					status = EXPECT_END_JSON;
					continue;
				}
				json::json_node* tempObject = stack->top();
				stack->pop();
				json::node_type type = stack->top()->get_node_type();
				if (type == json::STRING) {
					// key: { CURRENT } ,}
					std::string key = stack->top()->get_string();
					stack->pop();
					stack->top()->add_to_map(key, tempObject);
					status = EXPECT_COMMA | EXPECT_END_OBJ;
					continue;
				}
				if (type == json::ARRAY) {
					// xx, xx, {CURRENT} ,]
					stack->top()->add_to_vec(tempObject);
					status = EXPECT_COMMA | EXPECT_END_ARR;
					continue;
				}
			}
			// TODO: throw exception
			throw std::runtime_error("end object failed");
		case END_JSON:
			if (on_status(EXPECT_END_JSON)) {
				if (stack->size() == 1) {
					return stack->top();
				}
			}
			// TODO: throw exception 
			throw std::runtime_error("end document failed");
		default:
			throw std::runtime_error("unexpected char in parser");
		}
	}
	delete stack;
}

inline bool json_parser::on_status(int status)
{
	return (this->status & status) != 0;
}
