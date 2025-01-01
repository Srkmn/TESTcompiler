#include <iostream>  
#include <fstream>  
#include <vector>  

#include "TESTscan.h"  
#include "TESTparse.h"  
#include "TESTsemantic.h"  
#include "TESTmachine.h"

using namespace std;

void waitPressToExit() {
    std::cout << "\nPress Enter to exit...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // 清除之前的输入缓冲  
    std::cin.get(); // 等待按键  
}

int main(int argc, char* argv[]) {
    // 检查命令行参数  
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <source_file_path>" << std::endl;
        std::cerr << "Example: " << argv[0] << " ./test.txt" << std::endl;
        waitPressToExit();
        return 1;
    }

    // 获取源文件路径  
    std::string sourcePath = argv[1];

    // 构造其他文件路径（与源文件在同一目录下）  
    std::string directory = sourcePath.substr(0, sourcePath.find_last_of("/\\") + 1);
    if (directory == sourcePath) {
        directory = "./";  // 如果没有目录分隔符，就使用当前目录  
    }

    std::string lexPath = directory + "Lex.txt";
    std::string intermediatePath = directory + "Intermediate.txt";

    // 打开文件  
    std::ifstream inputFile(sourcePath);
    std::ofstream outputFile(lexPath);
    std::ofstream intermediateFile(intermediatePath);

    if (!inputFile) {
        std::cerr << "Error: Cannot open source file: " << sourcePath << std::endl;
        waitPressToExit();
        return 1;
    }

    if (!outputFile) {
        std::cerr << "Error: Cannot create lexical analysis output file: " << lexPath << std::endl;
        waitPressToExit();
        return 1;
    }

    if (!intermediateFile) {
        std::cerr << "Error: Cannot create intermediate code file: " << intermediatePath << std::endl;
        waitPressToExit();
        return 1;
    }

    std::vector<std::string> tokens;
    std::vector<std::string> errors;
    std::vector<std::string> intermediateCode;

    // 词法分析  
    std::cout << "Performing lexical analysis..." << std::endl;
    testScan(inputFile, outputFile, tokens, errors);

    if (!errors.empty()) {
        std::cout << "Lexical Analysis Errors:" << std::endl;
        for (const auto& error : errors) {
            std::cout << error << std::endl;
        }
        waitPressToExit();
        return 1;
    }

    std::cout << "Lexical analysis completed successfully." << std::endl;

    // 语法分析和语义分析  
    try {
        std::cout << "Performing syntax and semantic analysis..." << std::endl;
        testParse(tokens, intermediateCode);

        // 输出中间代码到文件  
        for (const auto& code : intermediateCode) {
            intermediateFile << code << std::endl;
        }

        std::cout << "Syntax and semantic analysis completed successfully." << std::endl;
        std::cout << "Intermediate code generated: " << intermediatePath << std::endl;

        // 关闭文件  
        inputFile.close();
        outputFile.close();
        intermediateFile.close();

        // 编译成功后，运行虚拟机执行中间代码  
        std::cout << "\nExecuting the compiled program..." << std::endl;
        std::cout << "----------------------------------------" << std::endl;

        TESTMachine machine(false);  // 默认不开启调试模式  
        machine.loadFromFile(intermediatePath);
        machine.execute();

        std::cout << "----------------------------------------" << std::endl;
        std::cout << "Program execution completed." << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Compilation error: " << e.what() << std::endl;
        inputFile.close();
        outputFile.close();
        intermediateFile.close();
        waitPressToExit();
        return 1;
    }

    
    waitPressToExit();
    return 0;
}