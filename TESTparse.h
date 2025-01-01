#ifndef TEST_PARSE_H  
#define TEST_PARSE_H  

#include <vector>  
#include <string> 

// ����֧�ֵ�token����  
const char* const TOKEN_TYPES[] = {
    "keyword", "identifier", "operator", "int", "delimiter"
};

// ���йؼ���  
const char* const KEYWORDS[] = {
    "if", "else", "for", "while", "int", "write", "read"
};

// ���������  
const char* const OPERATORS[] = {
    "+", "-", "*", "/", "=", "<", ">", ">=", "<=", "!=", "=="
};

// ���зָ���  
const char* const DELIMITERS[] = {
    "(", ")", ";", "{", "}"
};

// Token�ṹ�嶨��  
struct Token {
    std::string type;    // keyword, identifier, operator, int, delimiter  
    std::string value;
    int line;
};

// ��������ں���  
void testParse(const std::vector<std::string>& tokens, std::vector<std::string>& intermediateCode);

#endif