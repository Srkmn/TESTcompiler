#ifndef TEST_PARSE_H  
#define TEST_PARSE_H  

#include <vector>  
#include <string> 

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

// 解析器入口函数  
void testParse(const std::vector<std::string>& tokens, std::vector<std::string>& intermediateCode);

#endif