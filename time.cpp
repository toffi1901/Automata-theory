#include <iostream>
#include <fstream>
#include <chrono>
#include <string>
#include <random>
#include <ctime>
#include <algorithm>
#include "classes.hpp"
#include "AppClass.hpp"
#include "flex_lexer.hpp"

#include <algorithm>
#include <random>

static std::mt19937 gen(std::random_device{}());

std::string generate_server_name(int len) {
    const std::string letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const std::string digits = "0123456789";
    
    len = std::clamp(len, 1, 15);
    std::string name;
    

    name += letters[std::uniform_int_distribution<>(0, letters.size()-1)(gen)];
    
    for (int i = 1; i < len; ++i) {
        name += std::bernoulli_distribution(0.7)(gen) ? 
               letters[std::uniform_int_distribution<>(0, letters.size()-1)(gen)] :
               digits[std::uniform_int_distribution<>(0, digits.size()-1)(gen)];
    }
    
    return name;
}


std::string generate_resource_name(int len) {
    const std::string chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    
    len = std::clamp(len, 1, 32);
    std::string name;
    

    for (int i = 0; i < len-1; ++i) {
        name += chars[std::uniform_int_distribution<>(0, chars.size()-1)(gen)];
    }
    
    if (len > 0 && std::bernoulli_distribution(0.2)(gen)) {
        name += '$';
    } else {
        name += chars[std::uniform_int_distribution<>(0, chars.size()-2)(gen)]; // без $
    }
    
    return name;
}


std::string generate_path(int len) {
    const std::string chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    
    std::string path;
    while (len > 0) {
        int seg_len = std::uniform_int_distribution<>(1, std::min(32, len))(gen);
        for (int i = 0; i < seg_len; ++i) {
            path += chars[std::uniform_int_distribution<>(0, chars.size()-1)(gen)];
        }
        path += '\\';
        len -= (seg_len + 1);
    }
    
    return path;
}


std::string generate_correct_string(int total_len) {
    total_len = std::min(total_len, 100);

    int server_len = std::uniform_int_distribution<>(
        4, 
        std::min(15, total_len-6) 
    )(gen);
    
    std::string server = generate_server_name(server_len - 2); 
    
    int res_len = std::uniform_int_distribution<>(
        3,
        std::min(32, total_len - server_len - 3)
    )(gen);
    
    std::string resource = generate_resource_name(res_len - 1); 
    
    int path_len = total_len - (server_len + res_len + 1);
    std::string path = generate_path(path_len);
 
    std::string result = "\\\\" + server + "\\" + resource + "\\" + path;
    
    if (result.size() > total_len) {
        result.resize(total_len);
        if (result.back() != '\\') {
            size_t last_slash = result.rfind('\\');
            if (last_slash != std::string::npos) {
                result.resize(last_slash + 1);
            }
        }
    }
    
    return result;
}


std::string generate_incorrect_string(int total_len) {
    static std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<> error_type(0, 8);
    
    std::string s = generate_correct_string(total_len);
    if (s.empty()) return s;

    auto safe_insert = [](std::string& str, size_t pos, const std::string& value) {
        if (pos > str.size()) pos = str.size();
        str.insert(pos, value);
    };

    auto safe_erase = [](std::string& str, size_t pos, size_t count) {
        if (pos >= str.size()) return;
        str.erase(pos, std::min(count, str.size() - pos));
    };

    switch(error_type(gen)) {
        case 0: 
            if (s.size() > 2) {
                s[2] = '0' + gen() % 10;
            }
            break;
            
        case 1: { 
            size_t first_slash = s.find('\\', 2);
            size_t second_slash = s.find('\\', first_slash + 1);
            
            if (first_slash != std::string::npos && second_slash != std::string::npos) {
                size_t res_start = first_slash + 1;
                size_t res_end = second_slash;
                
                if (res_end > res_start + 1) {
                    size_t dollar_pos = res_start + gen() % (res_end - res_start - 1);
                    if (s.find('$', res_start) == std::string::npos) {
                        s[dollar_pos] = '$';
                    } else {
                        size_t existing = s.find('$', res_start);
                        std::swap(s[dollar_pos], s[existing]);
                    }
                }
            }
            break;
        }
            
        case 2: 
            s[gen() % s.size()] = '@';
            break;
            
        case 3: { 
            size_t slash_pos = s.find('\\', 3);
            if (slash_pos != std::string::npos) {
                safe_erase(s, slash_pos, 1);
            }
            break;
        }
            
        case 4: { 
            size_t slash_pos = s.find('\\', 2);
            if (slash_pos != std::string::npos && slash_pos + 1 < s.size()) {
                safe_insert(s, slash_pos + 1, std::string(35, 'a'));
            }
            break;
        }
            
        case 5: { 
            size_t first_slash = s.find('\\', 2);
            size_t second_slash = s.find('\\', first_slash + 1);
            if (first_slash != std::string::npos && second_slash != std::string::npos) {
                safe_erase(s, first_slash + 1, second_slash - first_slash - 1);
            }
            break;
        }
            
        case 6: 
            if (s.size() > 1) {
                size_t pos = gen() % (s.size() - 1) + 1;
                safe_insert(s, pos, "\\");
            }
            break;
            
        case 7: {
            size_t path_start = s.find('\\', s.find('\\', 2) + 1);
            if (path_start != std::string::npos && path_start + 1 < s.size()) {
                size_t pos = path_start + 1 + gen() % (s.size() - path_start - 1);
                s[pos] = '$';
            }
            break;
        }
            
        case 8:
            s.append(200, 'a');
            break;
    }
    if (s.empty()) {
        s = "\\invalid\\";
    }
    if (s.size() > total_len) {
        s.resize(total_len);
    }

    return s;
}


void create_file(int len, int count){
    std::ofstream outfile("smc.csv");
    if (!outfile.is_open()) {
        throw std::runtime_error("Не удалось открыть файл smc.csv");
    }
    
    outfile << "smc\n";
    Flex_Recognizer regex_recognizer;
    IRecognizers& recognizer = regex_recognizer;
    for (int i = 0; i < count; ++i)
    {
        std::string res = generate_incorrect_string(len);
        outfile << res  << "\n";
    }
    outfile.close();
}



void time_file_processing(const std::string& input_file, 
                        const std::string& output_file,
                        IRecognizers& recognizer,  
                        int measurement_interval = 100) {
    std::ifstream in(input_file);
    std::ofstream out(output_file);
    
    if (!in.is_open()) {
        throw std::runtime_error("Не удалось открыть входной файл");
    }
    if (!out.is_open()) {
        throw std::runtime_error("Не удалось создать файл результатов");
    }

    std::string line;
    int line_count = 0;
    auto start_time = std::chrono::high_resolution_clock::now();

    out << "line_count,time_ms\n";  
    while (std::getline(in, line)) {
        recognizer.string_recognizer(line);  
        line_count++;
        
        if (line_count % measurement_interval == 0) {
            auto end_time = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
            
            out << line_count << "," << duration.count() << "\n";
            start_time = std::chrono::high_resolution_clock::now();
        }
    }

    if (line_count % measurement_interval != 0) {
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
        out << line_count << "," << duration.count() << "\n";
    }

    in.close();
    out.close();
}

/*
int main() {
    try {
        std::cout << "Input length, number of strings and server, resource and catalog lengths" << std::endl;
        int len, count;
        std::cin >> len >> count;

        My_Regex regex_recognizer;
        IRecognizers& recognizer = regex_recognizer;

        create_file(len, count); 
        time_file_processing("smc.csv", "incorrect_timings_flex.csv", recognizer, 100);  
        recognizer.output();
        std::cout << "Тайминг завершен. Результаты в timings.csv" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}*/