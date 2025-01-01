#include "TESTmachine.h"  
#include <iostream>  
#include <fstream>  
#include <sstream>  

void TESTMachine::push(int value) {  
    stack.push_back(value);  
    if (debug_mode) {  
        std::cout << "Push: " << value << std::endl;  
    }  
}  

int TESTMachine::pop() {  
    if (stack.empty()) {  
        throw std::runtime_error("Stack underflow");  
    }  
    int value = stack.back();  
    stack.pop_back();  
    if (debug_mode) {  
        std::cout << "Pop: " << value << std::endl;  
    }  
    return value;  
}  

int TESTMachine::top() const {  
    if (stack.empty()) {  
        throw std::runtime_error("Stack empty");  
    }  
    return stack.back();  
}  

void TESTMachine::addInstruction(const std::string& instruction) {  
    instructions.push_back(instruction);  
}  

int TESTMachine::getValue(const std::string& var) {  
    return memory[var];  
}  

void TESTMachine::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            instructions.push_back(line);
            if (debug_mode) {
                std::cout << "Loaded instruction: " << line << std::endl;
            }
        }
    }
    file.close();
}

void TESTMachine::execute() {
    stack.clear();
    memory.clear();
    pc = 0;

    while (pc < instructions.size()) {
        std::istringstream iss(instructions[pc]);
        std::string op;
        iss >> op;

        if (debug_mode) {
            std::cout << "Executing: " << instructions[pc] << std::endl;
            std::cout << "Stack: ";
            for (int val : stack) std::cout << val << " ";
            std::cout << std::endl;
        }

        try {
            if (op == "LOAD") {
                std::string var;
                iss >> var;
                push(memory[var]);
            }
            else if (op == "LOADI") {
                int val;
                iss >> val;
                push(val);
            }
            else if (op == "STO") {
                std::string var;
                iss >> var;
                // ±£≥÷’ª∂•÷µ≤ª±‰  
                memory[var] = top();
            }
            else if (op == "POP") {
                pop();
            }
            else if (op == "ADD") {
                int b = pop();
                int a = pop();
                push(a + b);
            }
            else if (op == "SUB") {
                int b = pop();
                int a = pop();
                push(a - b);
            }
            else if (op == "MULT") {
                int b = pop();
                int a = pop();
                push(a * b);
            }
            else if (op == "DIV") {
                int b = pop();
                if (b == 0) throw std::runtime_error("Division by zero");
                int a = pop();
                push(a / b);
            }
            else if (op == "BR") {
                int targetAddr;
                iss >> targetAddr;
                pc = targetAddr;
                continue;
            }
            else if (op == "BRF") {
                int targetAddr;
                iss >> targetAddr;
                if (pop() == 0) {  // ’ª∂•Œ™0 ±Ã¯◊™  
                    pc = targetAddr;
                    continue;
                }
            }
            else if (op == "EQ") {
                int b = pop();
                int a = pop();
                push(a == b ? 1 : 0);
            }
            else if (op == "NOTEQ") {
                int b = pop();
                int a = pop();
                push(a != b ? 1 : 0);
            }
            else if (op == "GT") {
                int b = pop();  // ’ª∂•  
                int a = pop();  // ¥Œ’ª∂•  
                push(a > b ? 1 : 0);  // ¥Œ’ª∂• > ’ª∂•  
            }
            else if (op == "LES") {
                int b = pop();  // ’ª∂•  
                int a = pop();  // ¥Œ’ª∂•  
                push(a < b ? 1 : 0);  // ¥Œ’ª∂• < ’ª∂•  
            }
            else if (op == "GE") {
                int b = pop();  // ’ª∂•  
                int a = pop();  // ¥Œ’ª∂•  
                push(a >= b ? 1 : 0);  // ¥Œ’ª∂• >= ’ª∂•  
            }
            else if (op == "LE") {
                int b = pop();  // ’ª∂•  
                int a = pop();  // ¥Œ’ª∂•  
                push(a <= b ? 1 : 0);  // ¥Œ’ª∂• <= ’ª∂•  
            }
            else if (op == "AND") {
                int b = pop();
                int a = pop();
                push((a && b) ? 1 : 0);
            }
            else if (op == "OR") {
                int b = pop();
                int a = pop();
                push((a || b) ? 1 : 0);
            }
            else if (op == "NOT") {
                push(!pop() ? 1 : 0);
            }
            else if (op == "IN") {
                int value;
                std::cout << "Input: ";
                std::cin >> value;
                push(value);
            }
            else if (op == "OUT") {
                std::cout << pop() << std::endl;
            }
            else if (op == "STOP") {
                break;
            }
            else {
                throw std::runtime_error("Unknown instruction: " + op);
            }
        }
        catch (const std::exception& e) {
            std::cerr << "Runtime error at instruction " << pc << ": " << e.what() << std::endl;
            return;
        }

        pc++;

        if (debug_mode) {
            std::cout << "\nMemory state:" << std::endl;
            for (const auto& var : memory) {
                std::cout << var.first << " = " << var.second << std::endl;
            }
            std::cout << "-------------------\n" << std::endl;
        }
    }
}