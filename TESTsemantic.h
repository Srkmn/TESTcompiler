#pragma once  
#ifndef TEST_SEMANTIC_H  
#define TEST_SEMANTIC_H  

#include <string>  
#include <vector>  

// 类型相关  
enum class ValueType {
    INTEGER,
    FLOAT,
    BOOLEAN,
    UNKNOWN
};

// 符号相关信息  
struct SymbolInfo {
    std::string name;
    ValueType type;
    bool isInitialized;
};

// 初始化语义分析器  
void initializeSemanticAnalyzer();

// 符号表管理  
void addVariable(const std::string& name);
bool checkIdentifierDeclared(const std::string& name);
bool checkInitialized(const std::string& name);
void markInitialized(const std::string& name);

// 语义错误处理  
void semanticError(const std::string& message);

// 中间代码生成  
static void generateCode(const std::string& code);

// 获取符号信息  
static SymbolInfo* getSymbolInfo(const std::string& name);

#endif // TEST_SEMANTIC_H