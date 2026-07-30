#ifndef CLASSES_H
#define CLASSES_H

#include <vector>
#include <regex>
#include <string>
#include <ranges>
#include <unordered_map>

class IRecognizers {
    public:
        virtual ~IRecognizers() = default;

        virtual std::string get_server()= 0;
        virtual size_t get_full_length() = 0;
        virtual std::string get_input() = 0;
        virtual std::unordered_map<std::string, int>& get_statistic()= 0;
        virtual void set_server(const std::string &new_server) = 0;
        virtual void set_full_length(size_t new_len) = 0;
        virtual void set_input(const std::string &new_input) = 0;
        virtual void set_statistic(const std::unordered_map<std::string, int>& new_map) = 0;

        virtual void string_recognizer(const std::string &input) = 0;
        virtual void log_server() = 0;
        virtual void log_error() = 0;
        virtual std::vector<std::string> input_data() = 0;
        virtual void output() = 0;
    };
    
class Recognizers : virtual public IRecognizers {
    private:
        std::string server_name;
        std::string input;
        size_t full_length;
        std::unordered_map<std::string, int> statistic;
    
    public:
        Recognizers() : server_name(""), input(""), full_length(0) {
            statistic = {};
        };
        std::string get_server() override { return server_name; };
        size_t get_full_length() override { return full_length; };
        std::string get_input() override { return input; };
        std::unordered_map<std::string, int>& get_statistic() override { return statistic; };
        void set_server(const std::string &new_server) override { server_name = new_server; };
        void set_full_length(size_t new_len) override { full_length = new_len; };
        void set_input(const std::string &new_input) override { input = new_input; };
        void set_statistic(const std::unordered_map<std::string, int>& new_map) override{
            statistic = new_map;
        };
        //void string_recognizer(const std::string &input) override;
        void log_error() override;
        void log_server() override;
        std::vector<std::string> input_data() override;
        void output() override;
        virtual ~Recognizers() = default;
    };
    

    class IMy_Regex : virtual public IRecognizers{
        public:
            virtual ~IMy_Regex() = default;
    };

    class My_Regex : public Recognizers, public IMy_Regex{
    public:
        My_Regex(){};

        using Recognizers::get_server;
        using Recognizers::get_full_length;
        using Recognizers::get_input;
        using Recognizers::get_statistic;

        using Recognizers::set_server;
        using Recognizers::set_full_length;
        using Recognizers::set_input;
        using Recognizers::set_statistic;

        using Recognizers::log_error;
        using Recognizers::log_server;
        using Recognizers::input_data;
        using Recognizers::output;
        void string_recognizer(const std::string &input) override;
        bool check_string();
        virtual ~My_Regex() = default;
    };

    


#endif