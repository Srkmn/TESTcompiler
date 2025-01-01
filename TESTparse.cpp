#include "TESTparse.h"  
#include "TESTsemantic.h"  
#include <iostream>  
#include <stdexcept>  
#include <vector>  
#include <string>  

/**
 * �﷨������ʵ��
 */

 // ȫ�ֱ���  
static Token currentToken;        // ��ǰ�����token  
static std::vector<std::string> tokenList;  // token�б�  
static size_t currentPos = 0;    // ��ǰ����λ��  
static int labelCounter = 0;
static std::vector<std::string> generatedCode;

// ������������  
static void nextToken();
static bool match(const std::string& expectedType, const std::string& expectedValue = "");
static void syntaxError(const std::string& message);

// ǰ�����������﷨��������  
static void program();
static void declaration_list();
static void declaration_stat();
static void statement_list();
static void statement();
static void if_stat();
static void while_stat();
static void for_stat();
static void read_stat();
static void write_stat();
static void compound_stat();
static void assignment_stat();
static void assignment_expression();  
static void bool_expression();          
static void arithmetic_expression();    
static void term();                     
static void factor();                  

// ��ȡ��һ��token  
static void nextToken() {
    if (currentPos < tokenList.size()) {
        std::string tokenStr = tokenList[currentPos++];
        size_t spacePos = tokenStr.find(' ');
        if (spacePos != std::string::npos) {
            currentToken.type = tokenStr.substr(0, spacePos);
            size_t lastSpacePos = tokenStr.rfind(' ');
            if (lastSpacePos != spacePos) {
                currentToken.value = tokenStr.substr(spacePos + 1, lastSpacePos - spacePos - 1);
                currentToken.line = std::stoi(tokenStr.substr(lastSpacePos + 1));
            }
        }
    }
}

// �﷨������  
static void syntaxError(const std::string& message) {
    std::string errorMsg = "Syntax error at line " + std::to_string(currentToken.line) +
        ": " + message +
        "\nFound token: Type='" + currentToken.type +
        "', Value='" + currentToken.value + "'";
    throw std::runtime_error(errorMsg);
}

// ��鵱ǰtoken�Ƿ�ƥ��Ԥ��  
static bool match(const std::string& expectedType, const std::string& expectedValue) {
    return currentToken.type == expectedType &&
        (expectedValue.empty() || currentToken.value == expectedValue);
}

// <program> �� {<declaration_list><statement_list>}    
static void program() {
    if (!match("delimiter", "{")) {
        syntaxError("Expected '{' at program start");
    }
    nextToken();

    declaration_list();  // ���ȴ�����������  
    statement_list();    // Ȼ�����������  

    if (!match("delimiter", "}")) {
        syntaxError("Expected '}' at program end");
    }
    nextToken();
}

// <declaration_list> �� <declaration_list><declaration_stat> | ��  
static void declaration_list() {
    while (match("keyword", "int")) {  // ���ݹ���3��ֻ֧��int����  
        declaration_stat();
    }
}

// <declaration_stat> �� int ID;  
static void declaration_stat() {
    nextToken();  // ���� 'int'  

    if (!match("identifier")) {
        syntaxError("Expected identifier after 'int'");
    }
    addVariable(currentToken.value);
    nextToken();

    if (!match("delimiter", ";")) {
        syntaxError("Expected ';' after identifier in declaration");
    }
    nextToken();
}

//  <statement_list> �� <statement_list><statement> | ��  
static void statement_list() {
    while (!match("delimiter", "}")) {  // ֱ����������������  
        statement();
    }
}

// <statement> �� <if_stat>|<while_stat>|<for_stat>|<read_stat> 
static void statement() {
    if (match("keyword", "if")) {
        if_stat();
    }
    else if (match("keyword", "while")) {
        while_stat();
    }
    else if (match("keyword", "for")) {
        for_stat();
    }
    else if (match("keyword", "read")) {
        read_stat();
    }
    else if (match("keyword", "write")) {
        write_stat();
    }
    else if (match("delimiter", "{")) {
        compound_stat();
    }
    else if (match("identifier")) {
        assignment_stat();
    }
    else {
        std::string expectedTokens = "Expected '{', 'if', 'while', 'for', 'write', 'read' or identifier";
        syntaxError(expectedTokens);
    }
}

// if (<bool_expression >) <statement >
static void if_stat() {
    nextToken();  // ���� if  

    if (!match("delimiter", "(")) {
        syntaxError("Expected '(' after 'if'");
    }
    nextToken();

    bool_expression();

    if (!match("delimiter", ")")) {
        syntaxError("Expected ')'");
    }
    nextToken();

    // ��¼������ת��λ��  
    size_t falseBranch = generatedCode.size();
    generatedCode.push_back("BRF " + std::to_string(0));  // ������  

    statement();  // if ������  

    // ����Ƿ��� else  
    if (currentToken.type == "keyword" && currentToken.value == "else") {
        size_t skipElse = generatedCode.size();
        generatedCode.push_back("BR " + std::to_string(0));  // ���� else ���ֵ���תָ��  

        size_t elseStart = generatedCode.size();
        // ���� falseBranch��ʹ����ת�� else �Ŀ�ʼ  
        generatedCode[falseBranch] = "BRF " + std::to_string(elseStart);

        nextToken();  // ���� else  
        statement();  // else ������  

        // ���� skipElse��ʹ����ת������ if-else �ṹ֮��  
        size_t afterElse = generatedCode.size();
        generatedCode[skipElse] = "BR " + std::to_string(afterElse);
    }
    else {
        // û�� else��ֱ�ӻ��� falseBranch  
        generatedCode[falseBranch] = "BRF " + std::to_string(generatedCode.size());
    }
}

// <while_stat> �� while (<bool_expression>) <statement>  
static void while_stat() {
    nextToken();

    if (!match("delimiter", "(")) {
        syntaxError("Expected '(' after 'while'");
    }
    nextToken();

    size_t conditionStart = generatedCode.size();
    bool_expression();  // �������㣬�����ջ��  

    if (!match("delimiter", ")")) {
        syntaxError("Expected ')'");
    }
    nextToken();

    // ����Ϊ��ʱ����ѭ��  
    size_t brf_pos = generatedCode.size();
    generatedCode.push_back("BRF " + std::to_string(0)); // ��ʱ��0  

    statement();  // ѭ����  

    // ���������������жϴ�  
    generatedCode.push_back("BR " + std::to_string(conditionStart));

    // ��������Ϊ��ʱ����ת��ַ  
    generatedCode[brf_pos] = "BRF " + std::to_string(generatedCode.size());
}

// <for_stat> �� for (<assignment_expression>; <bool_expression>; <assignment_expression>) <statement> 
static void for_stat() {
    nextToken();
    if (!match("delimiter", "(")) {
        syntaxError("Expected '(' after 'for'");
    }
    nextToken();

    // ��ʼ������  
    assignment_expression();
    if (!match("delimiter", ";")) {
        syntaxError("Expected ';' after initialization");
    }
    nextToken();

    // ��¼ѭ����ʼλ��  
    size_t loop_start = generatedCode.size();

    // �����ж�  
    bool_expression();
    if (!match("delimiter", ";")) {
        syntaxError("Expected ';' after condition");
    }
    nextToken();

    // ѭ��������ת  
    size_t exit_jump = generatedCode.size();
    generatedCode.push_back("BRF " + std::to_string(0));  // ������  

    // ������������  
    std::vector<std::string> increment_code;
    size_t increment_start = generatedCode.size();
    assignment_expression();
    for (size_t i = increment_start; i < generatedCode.size(); i++) {
        increment_code.push_back(generatedCode[i]);
    }
    generatedCode.resize(increment_start);  // �Ƴ���������  

    if (!match("delimiter", ")")) {
        syntaxError("Expected ')'");
    }
    nextToken();

    // ѭ����  
    statement();

    // ��������  
    generatedCode.insert(generatedCode.end(), increment_code.begin(), increment_code.end());

    // ����ѭ����ʼ  
    generatedCode.push_back("BR " + std::to_string(loop_start));

    // ѭ����������  
    size_t end_loop = generatedCode.size();
    generatedCode[exit_jump] = "BRF " + std::to_string(end_loop);
}

// <read_stat> �� read ID; 
static void read_stat() {
    nextToken();

    if (!match("identifier")) {
        syntaxError("Expected identifier after 'read'");
    }

    if (!checkIdentifierDeclared(currentToken.value)) {
        semanticError("Undefined variable: " + currentToken.value);
    }

    std::string id = currentToken.value;
    generatedCode.push_back("IN");
    generatedCode.push_back("STO " + id);
    generatedCode.push_back("POP");

    nextToken();

    if (!match("delimiter", ";")) {
        syntaxError("Expected ';' after read statement");
    }
    nextToken();
}

// <write_stat> �� write <arithmetic_expression>;  
static void write_stat() {
    nextToken();

    arithmetic_expression();  // �������ջ��  
    generatedCode.push_back("OUT");

    if (!match("delimiter", ";")) {
        syntaxError("Expected ';' after write statement");
    }
    nextToken();
}

static void compound_stat() {
    nextToken();  // ���� '{'  
    statement_list();
    if (!match("delimiter", "}")) {
        syntaxError("Expected '}'");
    }
    nextToken();
}

// <assignment_stat> �� <assignment_expression>;  
static void assignment_stat() {
    if (!match("identifier")) {
        syntaxError("Expected identifier");
    }

    assignment_expression();  // ����ֵ���ʽ  

    if (!match("delimiter", ";")) {
        syntaxError("Expected ';' after assignment");
    }
    nextToken();  // ���� ';'  
}

// <assignment_expression> �� ID = <arithmetic_expression>   
static void assignment_expression() {
    std::string id = currentToken.value;
    if (!checkIdentifierDeclared(id)) {
        semanticError("Undefined variable: " + id);
    }
    nextToken();

    if (!match("operator", "=")) {
        syntaxError("Expected '=' in assignment");
    }
    nextToken();

    arithmetic_expression();
    generatedCode.push_back("STO " + id);
    generatedCode.push_back("POP");
}

// <bool_expression> �� <arithmetic_expression> relop <arithmetic_expression>  
static void bool_expression() {
    arithmetic_expression();

    if (!(match("operator", ">") || match("operator", "<") ||
        match("operator", ">=") || match("operator", "<=") ||
        match("operator", "==") || match("operator", "!="))) {
        syntaxError("Expected relational operator");
    }

    std::string op = currentToken.value;
    nextToken();

    arithmetic_expression();

    if (op == ">") generatedCode.push_back("GT");
    else if (op == "<") generatedCode.push_back("LES");
    else if (op == ">=") generatedCode.push_back("GE");
    else if (op == "<=") generatedCode.push_back("LE");
    else if (op == "==") generatedCode.push_back("EQ");
    else if (op == "!=") generatedCode.push_back("NOTEQ");
}

// arithmetic_expression �� term { (+|-) term }    {(+|-) term} ��ʾ������0������ "+term" �� "-term"
static void arithmetic_expression() {
    term();

    while (match("operator", "+") || match("operator", "-")) {
        std::string op = currentToken.value;
        nextToken();

        term();

        if (op == "+") generatedCode.push_back("ADD");
        else generatedCode.push_back("SUB");
    }
}

// term �� factor { (*|/) factor }  
// <term> �� <term> *|/ <factor> | <factor>  
static void term() {
    factor();

    while (match("operator", "*") || match("operator", "/")) {
        std::string op = currentToken.value;
        nextToken();

        factor();

        if (op == "*") generatedCode.push_back("MULT");
        else generatedCode.push_back("DIV");
    }
}

// factor �� ( arithmetic_expression ) | identifier | number   
static void factor() {
    if (match("delimiter", "(")) {
        nextToken();
        arithmetic_expression();
        if (!match("delimiter", ")")) {
            syntaxError("Expected ')'");
        }
        nextToken();
    }
    else if (match("identifier")) {
        if (!checkIdentifierDeclared(currentToken.value)) {
            semanticError("Undefined variable: " + currentToken.value);
        }
        generatedCode.push_back("LOAD " + currentToken.value);
        nextToken();
    }
    else if (match("number")) {
        generatedCode.push_back("LOADI " + currentToken.value);
        nextToken();
    }
    else {
        syntaxError("Expected '(', identifier, or number");
    }
}

// �﷨����������ʵ��  
void testParse(const std::vector<std::string>& tokens, std::vector<std::string>& intermediateCode) {
    tokenList = tokens;
    currentPos = 0;
    labelCounter = 0;
    generatedCode.clear();

    try {
        initializeSemanticAnalyzer();
        nextToken();
        program();

        if (currentPos < tokenList.size()) {
            syntaxError("Unexpected tokens after program end");
        }

        intermediateCode = generatedCode;
        std::cout << "Syntax analysis completed successfully." << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "\nParser error details:" << std::endl;
        std::cerr << e.what() << std::endl;

        // ��ʾ��ǰλ�õ�token��Ϣ  
        std::cerr << "Current position in token stream: " << currentPos << "/"
            << tokenList.size() << std::endl;

        // ��ʾ����λ�ø�����tokens  
        if (currentPos < tokenList.size()) {
            std::cerr << "Nearby tokens:" << std::endl;
            size_t start = (currentPos >= 2) ? currentPos - 2 : 0;
            size_t end = std::min(currentPos + 3, tokenList.size());
            for (size_t i = start; i < end; i++) {
                std::cerr << (i == currentPos - 1 ? "-> " : "   ") << tokenList[i] << std::endl;
            }
        }
        throw;
    }
}