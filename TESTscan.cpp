
#include "TESTscan.h" 

// ������  
void outputTokens(const vector<string>& tokens, ofstream& outFile) {
    for (const auto& token : tokens) {
        outFile << token << endl;
    }
}

// �������  
void outputErrors(const vector<string>& errors, ofstream& outFile) {
    for (const auto& error : errors) {
        outFile << "Error: " << error << endl;
    }
}

// �Ƿ����Ʊ��
bool isTableSymbol(const char& op) {
    if (op == ' ' || op == '\n' || op == '\t' || op == '\r') {
        return true;
    }

    return false;
}

// ����Ƿ��Ǳ�����  
bool isKeyword(const string& word) {
    return find(keywords.begin(), keywords.end(), word) != keywords.end();
}

// ���������������Ŀ���
bool canOperator(const string& op) {
    for (const auto& It : operators) {
        if (It.front() == op[0]) {
            return true;
        }
    }
    return false;
}

// ����Ƿ��������  
bool isOperator(const string& op) {
    for (const auto& It : operators) {
        if (It == op) {
            return true;
        }
    }
    return false;
}

// ����ַ��Ƿ��ǺϷ��ı�ʶ���ַ�  
bool isIdentifierChar(char c) {
    return isalnum(c) || c == '_';  // ������ĸ�����ֺ��»���  
}

// У���ʶ���Ƿ�Ϸ�  
bool isValidIdentifier(const string& id) {
    return !id.empty() && isalpha(id[0]) && all_of(id.begin() + 1, id.end(), isIdentifierChar);
}

// ����޷��������Ƿ�Ϸ�  
bool isValidUnsignedInteger(const string& num) {
    if (num == "0") return true; // ��������0  
    return num.length() > 0 && num[0] != '0' && all_of(num.begin(), num.end(), isdigit);
}

void testScan(ifstream& inputFile, ofstream& outputFile, vector<string>& tokens, vector<string>& errors) {
    int line = 1;
    char c;
    string currentToken;

    while (inputFile.get(c)) { // �����ȡ�ַ�  
        // �����հ��ַ�  
        if (isTableSymbol(c)) {
            if (c == '\n') {
                ++line;
            }
            continue; // ���Կո񡢻��С��Ʊ��  
        }

        // ��ⵥ��ע��  
        if (c == '/') {
            char nextChar;
            if (inputFile.get(nextChar) && nextChar == '/') {
                // ��ȡֱ���������з���������һ��  
                while (inputFile.get(c) && c != '\n') {
                    currentToken += c;
                }
                //tokens.push_back("ע��: \'" + currentToken + "\'");
                currentToken.clear();
                inputFile.unget();
                continue;
            }
            else {
                inputFile.unget(); // �����һ���ַ�����'/'������Ż�  
            }
        }

        // ������ĸ����ʶ����ؼ��֣�  
        if (isalpha(c)) {
            currentToken += c;
            while (inputFile.get(c) && isIdentifierChar(c)) {
                currentToken += c;
            }
            inputFile.unget(); // �����һ��������ַ��Ż�����  

            // У���ʶ���Ƿ�Ϸ�  
            if (isValidIdentifier(currentToken)) {
                if (isKeyword(currentToken)) {
                    tokens.push_back("keyword " + currentToken + " " + to_string(line));
                }
                else {
                    tokens.push_back("identifier " + currentToken + " " + to_string(line));
                }
            }
            else {
                errors.push_back("��Ч��ʶ��: \'" + currentToken + "\'" + " �ڵ� " + to_string(line) + " ��");
            }
            currentToken.clear();
        }
        // ��������  
        else if (isdigit(c)) {
            currentToken += c;
            // ������ȡ����������  
            while (inputFile.get(c) && isdigit(c)) {
                currentToken += c;
            }
            inputFile.unget(); // �����һ��������ַ��Ż����� 

            bool bIsInvalidIdentifier = false;
            // �����һ���ַ��Ƿ�Ϊ��ĸ  
            if (inputFile.get(c) && isalpha(c)) {
                bIsInvalidIdentifier = true;
                currentToken += c;
                while (inputFile.get(c) && isalpha(c)) {
                    currentToken += c;
                }
                inputFile.unget(); // �����һ��������ַ��Ż�����  
                errors.push_back("��Ч��ʶ������λΪ���֣�: \'" + currentToken + "\'" + " �ڵ� " + to_string(line) + " ��");
            }
            else {
                inputFile.unget(); // �����һ��������ַ��Ż����� 
            }

            if (!bIsInvalidIdentifier) {
                // У���޷��������Ƿ�Ϸ�  
                if (isValidUnsignedInteger(currentToken)) {
                    tokens.push_back("number " + currentToken + " " + to_string(line));
                }
                else {
                    errors.push_back("��Ч������ǰ��0��: \'" + currentToken + "\'" + " �ڵ� " + to_string(line) + " ��");
                }
            }

            currentToken.clear();
        }
        // ���������  
        else if (canOperator(string(1, c))) {
            currentToken += c; // ��ӵ�ǰ�ַ�  
            // ����Ƿ��γ�˫Ŀ�����  
            if (inputFile.get(c) && isOperator(currentToken + c)) {
                currentToken += c; // �����һ���ַ�  
                tokens.push_back("operator " + currentToken + " " + to_string(line));
            }
            else {
                tokens.push_back("operator " + string(1, currentToken[0]) + " " + to_string(line));
                inputFile.unget(); // �����һ���ַ��Ż�����  
            }
            currentToken.clear();
        }
        // ����ֽ��  
        else if (find(delimiters.begin(), delimiters.end(), c) != delimiters.end()) {
            tokens.push_back("delimiter " + string(1, c) + " " + to_string(line));
        }
        // ���������ַ�  
        else {
            errors.push_back("��Ч�ַ�: \'" + string(1, c) + "\'" + " �ڵ� " + to_string(line) + " ��");
        }
    }

    if (errors.size() > 0) {
        string error; // �����������ֹƵ������IO��
        for (auto I : errors) {
            error += I + "\n";
        }
        cout << error;
        cout << "�ʷ���������ֹͣ����!" << endl;
    }

    // ������ļ�  
    outputTokens(tokens, outputFile);
    outputErrors(errors, outputFile);
}