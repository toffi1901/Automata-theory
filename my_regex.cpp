#include "classes.hpp"
#include <vector>
#include <regex>
#include <string>
#include <ranges>
#include <unordered_map>
#include <algorithm>
#include <iostream>
#include <string_view>

bool My_Regex::check_string() {
  std::string input = this->get_input();
  input.erase(0, input.find_first_not_of(' '));
  input.erase(input.find_last_not_of(' ') + 1);
  static std::regex regex_pattern(R"(^\\\\([A-Z][A-Z0-9]{0,14})\\[a-zA-Z0-9]{1,31}\$?(\\[a-zA-Z0-9]{1,31})*\\?$)");
  std::smatch matches;
  if (input.size() > 100) {
      return false;
  }

  try {
      if (!std::regex_match(input, matches, regex_pattern)) 
          return false;
  } catch (const std::regex_error &e) {
      std::cerr << e.what() << std::endl;
      return false;
  }
  this->set_server(matches[1].str());  
  return true;
}


void Recognizers::log_server(){
  std::string server = this->get_server();
  std::unordered_map<std::string, int> stat = this->get_statistic();
  auto it = stat.find(server);
  if (it != stat.end())
    it->second += 1;
  else
    stat.emplace(server, 1);
  this->set_statistic(stat);
}

void Recognizers::log_error(){
  std::cout << "Incorrect input! It must be like \\\\server_name\\resourse_name\\path\\" << std::endl;
}

void My_Regex::string_recognizer(const std::string &input){
  if (input.empty()){
    this->log_error();
    return;
  }
  this->set_input(input);
  this->set_full_length(input.size());
  if (this->check_string())
    this->log_server();
  else
    this->log_error();
}

