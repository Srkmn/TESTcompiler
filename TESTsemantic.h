#pragma once  
#ifndef TEST_SEMANTIC_H  
#define TEST_SEMANTIC_H  

#include <string>  
#include <vector>  

// �������  
enum class ValueType {
    INTEGER,
    FLOAT,
    BOOLEAN,
    UNKNOWN
};

// ���������Ϣ  
struct SymbolInfo {
    std::string name;
    ValueType type;
    bool isInitialized;
};

// ��ʼ�����������  
void initializeSemanticAnalyzer();

// ���ű����  
void addVariable(const std::string& name);
bool checkIdentifierDeclared(const std::string& name);
bool checkInitialized(const std::string& name);
void markInitialized(const std::string& name);

// ���������  
void semanticError(const std::string& message);

// �м��������  
static void generateCode(const std::string& code);

// ��ȡ������Ϣ  
static SymbolInfo* getSymbolInfo(const std::string& name);

#endif // TEST_SEMANTIC_H