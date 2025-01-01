#include "TESTsemantic.h"  
#include <unordered_map>  
#include <vector>  
#include <stdexcept>  
#include <iostream>  

// 全局变量  
static std::vector<SymbolInfo> symbolTable;  // 按声明顺序存储变量信息  
static std::unordered_map<std::string, int> symbolIndex;  // 用于快速查找变量索引  
static std::vector<std::string> generatedCode;  // 生成的中间代码  

// 辅助函数：获取类型字符串（用于错误消息）  
static std::string getTypeString(ValueType type) {
    switch (type) {
    case ValueType::INTEGER: return "integer";
    case ValueType::FLOAT: return "float";
    case ValueType::BOOLEAN: return "boolean";
    default: return "unknown";
    }
}

// 辅助函数：检查类型兼容性  
static bool areTypesCompatible(ValueType type1, ValueType type2) {
    // 如果任一类型未知，假定兼容  
    if (type1 == ValueType::UNKNOWN || type2 == ValueType::UNKNOWN) {
        return true;
    }

    // 数值类型之间兼容  
    if ((type1 == ValueType::INTEGER || type1 == ValueType::FLOAT) &&
        (type2 == ValueType::INTEGER || type2 == ValueType::FLOAT)) {
        return true;
    }

    // 相同类型兼容  
    return type1 == type2;
}

// 类型检查辅助函数  
static void checkTypeCompatibility(const std::string& op, ValueType leftType, ValueType rightType) {
    if (!areTypesCompatible(leftType, rightType)) {
        semanticError("Type mismatch in " + op + " operation: " +
            getTypeString(leftType) + " " + op + " " +
            getTypeString(rightType));
    }
}

// 初始化语义分析器  
void initializeSemanticAnalyzer() {
    symbolTable.clear();
    generatedCode.clear();
}

// 添加变量到符号表  
void addVariable(const std::string& name) {
    // 检查是否已声明  
    if (symbolIndex.find(name) != symbolIndex.end()) {
        semanticError("Variable '" + name + "' already declared");
    }

    // 添加到符号表  
    SymbolInfo info;
    info.name = name;
    info.type = ValueType::UNKNOWN;
    info.isInitialized = false;

    symbolIndex[name] = symbolTable.size();  // 记录变量在vector中的位置  
    symbolTable.push_back(info);
}

// 检查标识符是否已声明  
bool checkIdentifierDeclared(const std::string& name) {
    return getSymbolInfo(name) != nullptr;
}

// 变量初始化检查  
bool checkInitialized(const std::string& name) {
    SymbolInfo* info = getSymbolInfo(name);
    if (info && !info->isInitialized) {
        std::cout << "Warning: Variable '" << name << "' may be used uninitialized" << std::endl;
        return false;
    }
    return true;
}

// 标记变量已初始化  
void markInitialized(const std::string& name) {
    SymbolInfo* info = getSymbolInfo(name);
    if (info) {
        info->isInitialized = true;
    }
}

// 语义错误处理  
void semanticError(const std::string& message) {
    throw std::runtime_error("Semantic error: " + message);
}

// 生成中间代码  
static void generateCode(const std::string& code) {
    generatedCode.push_back(code);
}

// 获取符号信息  
static SymbolInfo* getSymbolInfo(const std::string& name) {
    auto it = symbolIndex.find(name);
    if (it != symbolIndex.end()) {
        return &symbolTable[it->second];
    }
    return nullptr;
}