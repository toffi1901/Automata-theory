#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <chrono>
#include <random>
#include <ctime>
#include <iomanip>
#include "classes.hpp"
#include "flex_lexer.hpp"
#include "AppClass.hpp"
#include <algorithm>


int main(){
    Flex_Recognizer regex_recognizer;
    IRecognizers& recognizer = regex_recognizer;
    std::cout << "Input strings, enter stop, if you finish";
    std::vector<std::string> input = recognizer.input_data();

    for (const auto &adress : input){
        recognizer.string_recognizer(adress);
    }
    recognizer.output();
}
