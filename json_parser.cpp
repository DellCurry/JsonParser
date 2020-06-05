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
				json::json_node* top;
				top =  stack->top();
				stack->pop();
				delete top;
				stack->top()->add_to_map(key, json::json_node::create_bool_node(tk_reader->bool_reader()));
				stack->top()->add_to_seq_vec(key);
				status = EXPECT_COMMA | EXPECT_END_OBJ;
				continue;
			}
			if (on_status(EXPECT_ARR_VAL)) {
				stack->top()->add_to_vec(json::json_node::create_bool_node(tk_reader->bool_reader()));
				status = EXPECT_COMMA | EXPECT_END_ARR;
				continue;
			}
			delete stack;
			throw std::runtime_error("Unexpected bool at position: " 
				+ std::to_string(tk_reader->reader.get_pos()) + "\nAccepted JSON string: " + tk_reader->reader.get_readed());
		case NUMBER:
			if (on_status(EXPECT_SINGLE_VAL)) {
				stack->push(json::json_node::create_num_node(tk_reader->number_reader()));
				status = EXPECT_END_JSON;
				continue;
			}
			if (on_status(EXPECT_OBJ_VAL)) {
				std::string key = stack->top()->get_string();
				json::json_node* top;
				top =  stack->top();
				stack->pop();
				delete top;
				json::json_node* newnode = json::json_node::create_num_node(tk_reader->number_reader());
				stack->top()->add_to_map(key, newnode);
				newnode = nullptr;
				stack->top()->add_to_seq_vec(key);
				status = EXPECT_COMMA | EXPECT_END_OBJ;
				continue;
			}
			if (on_status(EXPECT_ARR_VAL)) {
				stack->top()->add_to_vec(json::json_node::create_num_node(tk_reader->number_reader()));
				status = EXPECT_COMMA | EXPECT_END_ARR;
				continue;
			}
			delete stack;
			throw std::runtime_error("Unexpected number at position: "
				+ std::to_string(tk_reader->reader.get_pos()) + "\nAccepted JSON string: " + tk_reader->reader.get_readed());
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
				json::json_node* top;
				top =  stack->top();
				stack->pop();
				delete top;
				stack->top()->add_to_map(key, json::json_node::create_null_node());
				stack->top()->add_to_seq_vec(key);
				status = EXPECT_COMMA | EXPECT_END_OBJ;
				continue;
			}
			if (on_status(EXPECT_ARR_VAL)) {
				tk_reader->null_reader();
				stack->top()->add_to_vec(json::json_node::create_null_node());
				status = EXPECT_COMMA | EXPECT_END_ARR;
				continue;
			}
			delete stack;
			throw std::runtime_error("Unexpected null at position: "
				+ std::to_string(tk_reader->reader.get_pos()) + "\nAccepted JSON string: " + tk_reader->reader.get_readed());
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
				json::json_node* top;
				top =  stack->top();
				stack->pop();
				delete top;
				stack->top()->add_to_map(key, json::json_node::create_str_node(tk_reader->string_reader()));
				stack->top()->add_to_seq_vec(key);
				status = EXPECT_COMMA | EXPECT_END_OBJ;
				continue;
			}
			if (on_status(EXPECT_ARR_VAL)) {
				stack->top()->add_to_vec(json::json_node::create_str_node(tk_reader->string_reader()));
				status = EXPECT_COMMA | EXPECT_END_ARR;
				continue;
			}
			delete stack;
			throw std::runtime_error("Unexpected string at position: "
				+ std::to_string(tk_reader->reader.get_pos()) + "\nAccepted JSON string: " + tk_reader->reader.get_readed());
		case COLON:
			if (status == EXPECT_COLON) {
				status = EXPECT_OBJ_VAL | EXPECT_BEGIN_OBJ | EXPECT_BEGIN_ARR;
				continue;
			}
			delete stack;
			throw std::runtime_error("Unexpected ':' at position: "
				+ std::to_string(tk_reader->reader.get_pos()) + "\nAccepted JSON string: " + tk_reader->reader.get_readed());
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
			delete stack;
			throw std::runtime_error("Unexpected ',' at position: "
				+ std::to_string(tk_reader->reader.get_pos()) + "\nAccepted JSON string: " + tk_reader->reader.get_readed());
		case BEGIN_ARR:
			if (on_status(EXPECT_BEGIN_ARR)) {
				stack->push(json::json_node::create_array_node());
				status = EXPECT_ARR_VAL | EXPECT_BEGIN_OBJ | EXPECT_BEGIN_ARR | EXPECT_END_ARR;
				continue;
			}
			delete stack;
			throw std::runtime_error("Unexpected '[' at position: "
				+ std::to_string(tk_reader->reader.get_pos()) + "\nAccepted JSON string: " + tk_reader->reader.get_readed());
		case BEGIN_OBJ:
			if (on_status(EXPECT_BEGIN_OBJ)) {
				stack->push(json::json_node::create_object_node());
				status = EXPECT_OBJ_KEY | EXPECT_BEGIN_OBJ | EXPECT_END_OBJ;
				continue;
			}
			delete stack;
			throw std::runtime_error("Unexpected '{' at position: "
				+ std::to_string(tk_reader->reader.get_pos()) + "\nAccepted JSON string: " + tk_reader->reader.get_readed());
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
					json::json_node* top;
					top =  stack->top();
					stack->pop();
					delete top;
					stack->top()->add_to_map(key, tempArray);
					stack->top()->add_to_seq_vec(key);
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
			delete stack;
			throw std::runtime_error("Unexpected '[' at position: "
				+ std::to_string(tk_reader->reader.get_pos()) + "\nAccepted JSON string: " + tk_reader->reader.get_readed());
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
					json::json_node* top;
					top =  stack->top();
					stack->pop();
					delete top;
					stack->top()->add_to_map(key, tempObject);
					stack->top()->add_to_seq_vec(key);
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
			delete stack;
			throw std::runtime_error("Unexpected '}' at position: "
				+ std::to_string(tk_reader->reader.get_pos()) + "\nAccepted JSON string: " + tk_reader->reader.get_readed());
		case END_JSON:
			if (on_status(EXPECT_END_JSON)) {
				if (stack->size() == 1) {
					json::json_node* ans = stack->top();
					delete stack;
					return ans;
				}
			}
			delete stack;
			throw std::runtime_error("Unexpected end JSON at position: "
				+ std::to_string(tk_reader->reader.get_pos()) + "\nAccepted JSON string: " + tk_reader->reader.get_readed());
		default:
			delete stack;
			throw std::runtime_error("Unexpected character at position: "
				+ std::to_string(tk_reader->reader.get_pos()) + "\nAccepted JSON string: " + tk_reader->reader.get_readed());
		}
	}
	delete stack;
}

inline bool json_parser::on_status(int status)
{
	return (this->status & status) != 0;
}
