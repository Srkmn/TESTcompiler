
#include "TESTscan.h" 

// 输出结果  
void outputTokens(const vector<string>& tokens, ofstream& outFile) {
    for (const auto& token : tokens) {
        outFile << token << endl;
    }
}

// 输出错误  
void outputErrors(const vector<string>& errors, ofstream& outFile) {
    for (const auto& error : errors) {
        outFile << "Error: " << error << endl;
    }
}

// 是否是制表符
bool isTableSymbol(const char& op) {
    if (op == ' ' || op == '\n' || op == '\t' || op == '\r') {
        return true;
    }

    return false;
}

// 检查是否是保留字  
bool isKeyword(const string& word) {
    return find(keywords.begin(), keywords.end(), word) != keywords.end();
}

// 检查有无是运算符的可能
bool canOperator(const string& op) {
    for (const auto& It : operators) {
        if (It.front() == op[0]) {
            return true;
        }
    }
    return false;
}

// 检查是否是运算符  
bool isOperator(const string& op) {
    for (const auto& It : operators) {
        if (It == op) {
            return true;
        }
    }
    return false;
}

// 检查字符是否是合法的标识符字符  
bool isIdentifierChar(char c) {
    return isalnum(c) || c == '_';  // 允许字母、数字和下划线  
}

// 校验标识符是否合法  
bool isValidIdentifier(const string& id) {
    return !id.empty() && isalpha(id[0]) && all_of(id.begin() + 1, id.end(), isIdentifierChar);
}

// 检查无符号整数是否合法  
bool isValidUnsignedInteger(const string& num) {
    if (num == "0") return true; // 允许单独的0  
    return num.length() > 0 && num[0] != '0' && all_of(num.begin(), num.end(), isdigit);
}

void testScan(ifstream& inputFile, ofstream& outputFile, vector<string>& tokens, vector<string>& errors) {
    int line = 1;
    char c;
    string currentToken;

    while (inputFile.get(c)) { // 逐个读取字符  
        // 跳过空白字符  
        if (isTableSymbol(c)) {
            if (c == '\n') {
                ++line;
            }
            continue; // 忽略空格、换行、制表符  
        }

        // 检测单行注释  
        if (c == '/') {
            char nextChar;
            if (inputFile.get(nextChar) && nextChar == '/') {
                // 读取直到遇到换行符，跳过这一行  
                while (inputFile.get(c) && c != '\n') {
                    currentToken += c;
                }
                //tokens.push_back("注释: \'" + currentToken + "\'");
                currentToken.clear();
                inputFile.unget();
                continue;
            }
            else {
                inputFile.unget(); // 如果下一个字符不是'/'，则将其放回  
            }
        }

        // 处理字母（标识符或关键字）  
        if (isalpha(c)) {
            currentToken += c;
            while (inputFile.get(c) && isIdentifierChar(c)) {
                currentToken += c;
            }
            inputFile.unget(); // 将最后一个读入的字符放回流中  

            // 校验标识符是否合法  
            if (isValidIdentifier(currentToken)) {
                if (isKeyword(currentToken)) {
                    tokens.push_back("keyword " + currentToken + " " + to_string(line));
                }
                else {
                    tokens.push_back("identifier " + currentToken + " " + to_string(line));
                }
            }
            else {
                errors.push_back("无效标识符: \'" + currentToken + "\'" + " 在第 " + to_string(line) + " 行");
            }
            currentToken.clear();
        }
        // 处理数字  
        else if (isdigit(c)) {
            currentToken += c;
            // 继续读取后续的数字  
            while (inputFile.get(c) && isdigit(c)) {
                currentToken += c;
            }
            inputFile.unget(); // 将最后一个读入的字符放回流中 

            bool bIsInvalidIdentifier = false;
            // 检查下一个字符是否为字母  
            if (inputFile.get(c) && isalpha(c)) {
                bIsInvalidIdentifier = true;
                currentToken += c;
                while (inputFile.get(c) && isalpha(c)) {
                    currentToken += c;
                }
                inputFile.unget(); // 将最后一个读入的字符放回流中  
                errors.push_back("无效标识符（首位为数字）: \'" + currentToken + "\'" + " 在第 " + to_string(line) + " 行");
            }
            else {
                inputFile.unget(); // 将最后一个读入的字符放回流中 
            }

            if (!bIsInvalidIdentifier) {
                // 校验无符号整数是否合法  
                if (isValidUnsignedInteger(currentToken)) {
                    tokens.push_back("number " + currentToken + " " + to_string(line));
                }
                else {
                    errors.push_back("无效整数（前导0）: \'" + currentToken + "\'" + " 在第 " + to_string(line) + " 行");
                }
            }

            currentToken.clear();
        }
        // 处理运算符  
        else if (canOperator(string(1, c))) {
            currentToken += c; // 添加当前字符  
            // 检查是否形成双目运算符  
            if (inputFile.get(c) && isOperator(currentToken + c)) {
                currentToken += c; // 添加下一个字符  
                tokens.push_back("operator " + currentToken + " " + to_string(line));
            }
            else {
                tokens.push_back("operator " + string(1, currentToken[0]) + " " + to_string(line));
                inputFile.unget(); // 将最后一个字符放回流中  
            }
            currentToken.clear();
        }
        // 处理分界符  
        else if (find(delimiters.begin(), delimiters.end(), c) != delimiters.end()) {
            tokens.push_back("delimiter " + string(1, c) + " " + to_string(line));
        }
        // 处理特殊字符  
        else {
            errors.push_back("无效字符: \'" + string(1, c) + "\'" + " 在第 " + to_string(line) + " 行");
        }
    }

    if (errors.size() > 0) {
        string error; // 缓存输出，防止频繁调用IO流
        for (auto I : errors) {
            error += I + "\n";
        }
        cout << error;
        cout << "词法分析错误，停止编译!" << endl;
    }

    // 输出到文件  
    outputTokens(tokens, outputFile);
    outputErrors(errors, outputFile);
}