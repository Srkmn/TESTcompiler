#include <iostream>  
#include <fstream>  
#include <vector>

#include "TESTscan.h"
#include "TESTparse.h"

using namespace std;

int main() {
    ifstream inputFile("./CompilePending.txt"); // 输入文件  
    ofstream outputFile("./Lex.txt"); // 输出文件  

    if (!inputFile) {
        cerr << "Error opening input file!" << endl;
        return 1;
    }

    vector<string> tokens;
    vector<string> errors;

    // 词法分析
    testScan(inputFile, outputFile, tokens, errors);

    // 执行语法分析
    testParse(tokens);

    inputFile.close();
    outputFile.close();

    return 0;
}