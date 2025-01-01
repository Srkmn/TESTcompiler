#include "TESTsemantic.h"  
#include <unordered_map>  
#include <vector>  
#include <stdexcept>  
#include <iostream>  

// ȫ�ֱ���  
static std::vector<SymbolInfo> symbolTable;  // ������˳��洢������Ϣ  
static std::unordered_map<std::string, int> symbolIndex;  // ���ڿ��ٲ��ұ�������  
static std::vector<std::string> generatedCode;  // ���ɵ��м����  

// ������������ȡ�����ַ��������ڴ�����Ϣ��  
static std::string getTypeString(ValueType type) {
    switch (type) {
    case ValueType::INTEGER: return "integer";
    case ValueType::FLOAT: return "float";
    case ValueType::BOOLEAN: return "boolean";
    default: return "unknown";
    }
}

// ����������������ͼ�����  
static bool areTypesCompatible(ValueType type1, ValueType type2) {
    // �����һ����δ֪���ٶ�����  
    if (type1 == ValueType::UNKNOWN || type2 == ValueType::UNKNOWN) {
        return true;
    }

    // ��ֵ����֮�����  
    if ((type1 == ValueType::INTEGER || type1 == ValueType::FLOAT) &&
        (type2 == ValueType::INTEGER || type2 == ValueType::FLOAT)) {
        return true;
    }

    // ��ͬ���ͼ���  
    return type1 == type2;
}

// ���ͼ�鸨������  
static void checkTypeCompatibility(const std::string& op, ValueType leftType, ValueType rightType) {
    if (!areTypesCompatible(leftType, rightType)) {
        semanticError("Type mismatch in " + op + " operation: " +
            getTypeString(leftType) + " " + op + " " +
            getTypeString(rightType));
    }
}

// ��ʼ�����������  
void initializeSemanticAnalyzer() {
    symbolTable.clear();
    generatedCode.clear();
}

// ��ӱ��������ű�  
void addVariable(const std::string& name) {
    // ����Ƿ�������  
    if (symbolIndex.find(name) != symbolIndex.end()) {
        semanticError("Variable '" + name + "' already declared");
    }

    // ��ӵ����ű�  
    SymbolInfo info;
    info.name = name;
    info.type = ValueType::UNKNOWN;
    info.isInitialized = false;

    symbolIndex[name] = symbolTable.size();  // ��¼������vector�е�λ��  
    symbolTable.push_back(info);
}

// ����ʶ���Ƿ�������  
bool checkIdentifierDeclared(const std::string& name) {
    return getSymbolInfo(name) != nullptr;
}

// ������ʼ�����  
bool checkInitialized(const std::string& name) {
    SymbolInfo* info = getSymbolInfo(name);
    if (info && !info->isInitialized) {
        std::cout << "Warning: Variable '" << name << "' may be used uninitialized" << std::endl;
        return false;
    }
    return true;
}

// ��Ǳ����ѳ�ʼ��  
void markInitialized(const std::string& name) {
    SymbolInfo* info = getSymbolInfo(name);
    if (info) {
        info->isInitialized = true;
    }
}

// ���������  
void semanticError(const std::string& message) {
    throw std::runtime_error("Semantic error: " + message);
}

// �����м����  
static void generateCode(const std::string& code) {
    generatedCode.push_back(code);
}

// ��ȡ������Ϣ  
static SymbolInfo* getSymbolInfo(const std::string& name) {
    auto it = symbolIndex.find(name);
    if (it != symbolIndex.end()) {
        return &symbolTable[it->second];
    }
    return nullptr;
}