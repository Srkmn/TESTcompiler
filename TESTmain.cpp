#include <iostream>  
#include <fstream>  
#include <vector>

#include "TESTscan.h"
#include "TESTparse.h"

using namespace std;

int main() {
    ifstream inputFile("./CompilePending.txt"); // �����ļ�  
    ofstream outputFile("./Lex.txt"); // ����ļ�  

    if (!inputFile) {
        cerr << "Error opening input file!" << endl;
        return 1;
    }

    vector<string> tokens;
    vector<string> errors;

    // �ʷ�����
    testScan(inputFile, outputFile, tokens, errors);

    // ִ���﷨����
    testParse(tokens);

    inputFile.close();
    outputFile.close();

    return 0;
}