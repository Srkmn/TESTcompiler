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
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // ���֮ǰ�����뻺��  
    std::cin.get(); // �ȴ�����  
}

int main(int argc, char* argv[]) {
    // ��������в���  
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <source_file_path>" << std::endl;
        std::cerr << "Example: " << argv[0] << " ./test.txt" << std::endl;
        waitPressToExit();
        return 1;
    }

    // ��ȡԴ�ļ�·��  
    std::string sourcePath = argv[1];

    // ���������ļ�·������Դ�ļ���ͬһĿ¼�£�  
    std::string directory = sourcePath.substr(0, sourcePath.find_last_of("/\\") + 1);
    if (directory == sourcePath) {
        directory = "./";  // ���û��Ŀ¼�ָ�������ʹ�õ�ǰĿ¼  
    }

    std::string lexPath = directory + "Lex.txt";
    std::string intermediatePath = directory + "Intermediate.txt";

    // ���ļ�  
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

    // �ʷ�����  
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

    // �﷨�������������  
    try {
        std::cout << "Performing syntax and semantic analysis..." << std::endl;
        testParse(tokens, intermediateCode);

        // ����м���뵽�ļ�  
        for (const auto& code : intermediateCode) {
            intermediateFile << code << std::endl;
        }

        std::cout << "Syntax and semantic analysis completed successfully." << std::endl;
        std::cout << "Intermediate code generated: " << intermediatePath << std::endl;

        // �ر��ļ�  
        inputFile.close();
        outputFile.close();
        intermediateFile.close();

        // ����ɹ������������ִ���м����  
        std::cout << "\nExecuting the compiled program..." << std::endl;
        std::cout << "----------------------------------------" << std::endl;

        TESTMachine machine(false);  // Ĭ�ϲ���������ģʽ  
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