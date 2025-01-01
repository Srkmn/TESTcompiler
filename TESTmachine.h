#pragma once  
#ifndef TESTMACHINE_H  
#define TESTMACHINE_H  

#include <string>  
#include <map>  
#include <vector>  

class TESTMachine {
private:
    std::vector<int> stack;              // 操作数栈  
    std::map<std::string, int> memory;   // 内存单元  
    std::map<std::string, int> labels;   // 存储标签位置  
    std::vector<std::string> instructions; // 存储指令  
    int pc = 0;                          // 程序计数器  
    bool debug_mode = false;             // 调试模式开关  

    // 栈操作辅助函数  
    void push(int value);
    int pop();
    int top() const;

public:
    TESTMachine(bool debug = false) : debug_mode(debug) {}
    void setDebugMode(bool debug) { debug_mode = debug; }
    void addInstruction(const std::string& instruction);
    int getValue(const std::string& var);
    void execute();
    void loadFromFile(const std::string& filename);
};

#endif