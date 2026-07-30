%option noyywrap
%{
#include <string>
#include <cctype>
#include <iostream>
#include "flex_lexer.hpp"

extern Flex_Recognizer* recognizer;
%}
%x SERVER RESOURCE CATALOG

int yylex();

%%
<INITIAL>{
^\\\\ { std::cout << "start" << std::endl; BEGIN(SERVER); }
\n {return 0; }
. { return TOKEN_ERROR; }
}


<SERVER>{
([A-Z][A-Z0-9]{0,14})\\ {
    BEGIN(RESOURCE); 
    recognizer->get_yylval() = std::string(yytext, yyleng - 1);
    std::string server(yytext, yyleng-1);
    std::cout << "сервер: " << server << std::endl;
    return TOKEN_SERVER; 
}
.|\n    { return TOKEN_ERROR; }
}

<RESOURCE>{
    ([a-zA-Z0-9]{1,31}[$]?)\\ {
        std::string res(yytext, yyleng-1);
        std::cout << "ресурс: " << res << std::endl;
        BEGIN(CATALOG);
        return TOKEN_RESOURCE;
    }
    ([a-zA-Z0-9]{1,31}[$]?) {
        BEGIN(INITIAL);
        return TOKEN_RESOURCE;
    }
    .|\n {  std::string temp(yytext, yyleng); std::cerr << "Ошибка: некорректное имя сервера" << temp << std::endl; return TOKEN_ERROR; }
}

<CATALOG>{
([A-Za-z0-9]{1,32})\\ {
    std::string path(yytext, yyleng-1);
    std::cout << "каталог: " << path << std::endl;
    BEGIN(CATALOG); 
    std::cout << "каталог: " << CATALOG << std::endl;
    return TOKEN_PATH;
}
([A-Za-z0-9]{1,32}) {
    std::string path(yytext, yyleng-1);
    std::cout << "каталог: " << path << std::endl;
    BEGIN(INITIAL); 
    return TOKEN_PATH;
}
\n {
    BEGIN(INITIAL); 
    return 0;
}
.       { return TOKEN_ERROR; }
}

<<EOF>> { 
    BEGIN(INITIAL); 
    return 0; 
}
%%