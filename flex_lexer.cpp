#include <string>
#include "flex_lexer.hpp"
#include <iostream>

Flex_Recognizer* recognizer = nullptr;


void Flex_Recognizer::string_recognizer(const std::string &input) {
    recognizer = this;
    if (input.size() > 100){
        this->log_error();
        return;
    }
    auto input_stream = std::make_unique<std::istringstream>(input);
    lexer->switch_streams(input_stream.get(), nullptr);
    this->set_input(input);
    
    int token;
    bool valid = true; 
    while ((token = this->yylex()) != 0) {
        switch (token) {
            case TOKEN_SERVER:
                break;
            case TOKEN_RESOURCE:
                break;
            case TOKEN_PATH:
                break;
            case TOKEN_ERROR:
                valid = false;  
                break;
            default:
                valid = false; 
                break;
        }
    }
    if (valid) {
        this->set_server(this->get_yylval());
        std::cout << this->get_yylval() << std::endl;
        this->log_server(); 
    } else {
        this->log_error();
    }
    this->get_yylval().clear(); 
}