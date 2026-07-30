#include <cctype>
#include <string>
#include "AppClass.hpp"


void AppClass::string_recognizer(const std::string &input) {
    this->reset();
    if (input.size() > 100) {
        this->log_error();
        return;
    }
    _fsm.enterStartState();
    
    try {
        for(char c : input) {
            this->set_char(c);
            if (isdigit(c)) {
                _fsm.on_digit();
            }
            else if (isalpha(c)) {
                _fsm.on_letter();
            }
            else if (c == '\\') {
                _fsm.on_slash();
            }
            else if (c == '$') {
                _fsm.on_dollar();
            }
            else {
                _fsm.on_default();
            }
        }
        _fsm.on_end();
        if (&_fsm.getState() != &Map1::Success &&
            &_fsm.getState() != &Map1::Catalog_name &&
            &_fsm.getState() != &Map1::Catalog_slash &&
            &_fsm.getState() != &Map1::Resourse_name) {
                this->set_error(true);
        }          
    }
    catch (const statemap::TransitionUndefinedException&) {
        this->set_error(true);
    }
    /*if (&_fsm.getState() != &Map1::Error) 
        _fsm.on_end();  */
    if (this->get_error()) 
        this->log_error();
    else
        this->log_server();
    
}

void AppClass::check_slash() {
    if (this->get_char() != '\\') 
    this->set_error(true);  
}

void AppClass::check_first_sym() {
    char c = this->get_char();
    if (!isalpha(c) || !isupper(c)) 
    this->set_error(true);  
}

void AppClass::add_to_server_name() {
    std::string name = this->get_server();  
    if (name.empty() && !isupper(this->get_char())) {
        this->set_error(true);  
        return;
    }
    if (name.size() > 15)
        this->set_error(true);
    name += this->get_char();
    this->set_server(name);
}

void AppClass::reset() {
    this->set_server("");
    this->set_error(false);
    _fsm.setState(Map1::Start);  
}


void AppClass::valid_resourse_name() {
    std::string res = this->get_resourse();
    char c = this->get_char();
    
    if (res.size() >= 32) {
        this->set_error(true);
        return;
    }
    
    if (c == '$') {
        if (res.find('$') != std::string::npos) {
            this->set_error(true);
            return;
        }
    }
    
    res += c;
    this->set_resourse(res);
}

void AppClass::valid_catalog_name() {
    std::string cat = this->get_catalog();
    if (cat.size() >= 32) {
        this->set_error(true);
        return;
    }
    cat += this->get_char();
    this->set_catalog(cat);
}

/*void AppClass::check_length(size_t max_len) {
    size_t current_len = 0;
    
    if (&_fsm.getState() == &Map1::Server_name) 
        current_len = this->get_server().size();
    else if (&_fsm.getState() == &Map1::Resourse_name) 
        current_len = this->get_resourse().size();
    else if (&_fsm.getState() == &Map1::Catalog_name) 
        current_len = this->get_catalog().size();
    else 
        return;
    
    if (current_len > max_len) {
        this->set_error(true);  
    }
}*/