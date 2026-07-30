#ifndef APPCLASS_H
#define APPCLASS_H

#include <iostream>
#include <unordered_map>
#include "classes.hpp"
#define default defaultAction
#include "parser_sm.h"
#undef default
#include "classes.hpp" 



enum State{
    Start,
    First_slash,
    Server_slash,
    Server_name,
    Resourse_slash,
    Resourse_name,
    Catalog_slash,
    Catalog_name,
    Success,
    Error,
    Reset
};


class AppClass : public Recognizers{
private:
    std::string resourse;
    std::string catalog;
    char current_char;
    bool err = false;
    parserContext _fsm; 
public:
    AppClass() : _fsm(*this)
    {
        _fsm.enterStartState(); 
        reset();
    }
    
    
    ~AppClass() {};  
    void set_resourse(const std::string &new_res){
        resourse = new_res;
    }
    void set_catalog(const std::string &new_cat){
        catalog = new_cat;
    }
    std::string get_resourse(){
        return resourse;
    }
    std::string get_catalog(){
        return catalog;
    }
    void set_char(char c){
        current_char = c;
    }
    char get_char(){
        return current_char;
    }
    void set_error(bool c){
        err = c;
    }
    bool get_error(){
        return err;
    }
    void check_slash();
    void check_first_sym();
    void valid_resourse_name();
    void valid_catalog_name();
    void string_recognizer(const std::string &input) override;
    void add_to_server_name();  
   // void check_length(size_t max_len);
    void reset();
};

#endif 