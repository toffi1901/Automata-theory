#ifndef FLEX_LEXER_H
#define FLEX_LEXER_H
#include <FlexLexer.h> 
#include "classes.hpp"

enum Tokens{
	TOKEN_SERVER = 1,
	TOKEN_RESOURCE = 2,
	TOKEN_PATH = 3,
	TOKEN_SLASH = 4,
	TOKEN_ERROR = 5
};

//extern int yylex();
extern std::string yylval;

class IFlex_Recognizer : virtual public IRecognizers{
    public:
        virtual ~IFlex_Recognizer() = default;
        virtual int yylex() = 0;
        virtual void string_recognizer(const std::string &input) = 0;
};

class Flex_Recognizer : public Recognizers, public IFlex_Recognizer {
    private:
        std::string yylval;
        yyFlexLexer* lexer;
    public:
        Flex_Recognizer() : lexer(new yyFlexLexer) {} 
        ~Flex_Recognizer() override {
            delete lexer; 
        }
    
        int yylex() override {
            return lexer->yylex(); 
        }
        std::string& get_yylval() {
            return yylval;
        }
    
        void string_recognizer(const std::string &input) override;
    };
       
#endif