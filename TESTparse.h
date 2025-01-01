#ifndef TEST_PARSE_H  
#define TEST_PARSE_H  

#include <vector>  
#include <string>  
#include <stdio.h>  
#include <string.h>  
#include <stdlib.h>  
#include <iostream>  

// 所有支持的token类型  
const char* const TOKEN_TYPES[] = {
    "keyword", "identifier", "operator", "int", "delimiter"
};

// 所有关键字  
const char* const KEYWORDS[] = {
    "if", "else", "for", "while", "int", "write", "read"
};

// 所有运算符  
const char* const OPERATORS[] = {
    "+", "-", "*", "/", "=", "<", ">", ">=", "<=", "!=", "=="
};

// 所有分隔符  
const char* const DELIMITERS[] = {
    "(", ")", ";", "{", "}"
};

// Token结构体定义  
struct Token {
    std::string type;    // keyword, identifier, operator, int, delimiter  
    std::string value;
    int line;
};

// 函数声明  
static void parseToken(const std::string& tokenStr, Token& token);
static void syntaxError(const std::string& message);
static void nextToken();
static bool match(const std::string& type, const std::string& value);
static Token lookAhead();
static void program();
static void declaration_list();
static void declaration_stat();
static void statement_list();
static void statement();
static void if_stat();
static void while_stat();
static void for_stat();
static void write_stat();
static void read_stat();
static void compound_stat();
static void assignment_expression();
static void assignment_stat();
static void bool_expression();
static void arithmetic_expression();
static void term();
static void factor();

// 语法分析主函数  
void testParse(const std::vector<std::string>& tokens);

#endif