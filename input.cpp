#include <iostream>
#include <string>
#include <vector>
#include "classes.hpp"

std::vector<std::string> Recognizers::input_data(){
    std::string input;
    std::cout << "input names" << std::endl;
    std::string flag = "stop";
    std::vector<std::string> all_strings;
    while (true) {
        std::getline(std::cin, input);
        if (input == flag) {
            break;
        }
        all_strings.push_back(input);
    }
    return all_strings;
}

void Recognizers::output(){
    auto stat = this->get_statistic();
    for (const auto& pair : stat){
        std::cout << pair.first << ":" << pair.second << std::endl;
    }
}

/*
void Recognizers::file_input(Recognizers& recognizer, const std::string& file_path) {
    std::ifstream file_stream(file_path);
    if (!file_stream) 
        throw;
    recognizer.string_recognizer(file_stream);
}*/
