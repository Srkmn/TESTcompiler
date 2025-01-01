#pragma once  
#ifndef TEST_SEMANTIC_H  
#define TEST_SEMANTIC_H  

#include <string>  
#include <vector>  

// ��ʼ�����������  
void initializeSemanticAnalyzer();

// ���ű����  
void addVariable(const std::string& name);
bool checkIdentifierDeclared(const std::string& name);

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

// ���������  
void semanticError(const std::string& message);

// �м��������  
void generateCode(const std::string& code);

// ��ȡ������Ϣ  
SymbolInfo* getSymbolInfo(const std::string& name);

#endif // TEST_SEMANTIC_H