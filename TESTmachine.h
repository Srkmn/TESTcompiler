#pragma once  
#ifndef TESTMACHINE_H  
#define TESTMACHINE_H  

#include <string>  
#include <map>  
#include <vector>  

class TESTMachine {
private:
    std::vector<int> stack;              // ������ջ  
    std::map<std::string, int> memory;   // �ڴ浥Ԫ  
    std::map<std::string, int> labels;   // �洢��ǩλ��  
    std::vector<std::string> instructions; // �洢ָ��  
    int pc = 0;                          // ���������  
    bool debug_mode = false;             // ����ģʽ����  

    // ջ������������  
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