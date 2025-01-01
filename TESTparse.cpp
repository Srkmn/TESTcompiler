#include "TESTparse.h"   

/**
 * 语法分析器实现
 *
 * 文法规则已消除左递归和左公因子，符合LL(1)文法要求
 * 主要的改造包括：
 * 1. 算术表达式的左递归被消除：
 *    原始：arithmetic_expression → arithmetic_expression + term | term
 *    现在：arithmetic_expression → term arithmetic_expression'
 *         arithmetic_expression' → + term arithmetic_expression' | ε
 *
 * 2. 语句的左公因子被消除：
 *    通过先读取token类型进行分支判断
 *
 * 3. 需要超前扫描的情况：
 *    - 赋值语句与表达式语句的区分
 */

 // 全局变量  
static Token currentToken;        // 当前处理的token  
static std::vector<std::string> tokenList;  // token列表  
static size_t currentPos = 0;    // 当前处理位置  

// 超前扫描函数  
static Token lookAhead() {
    if (currentPos < tokenList.size()) {
        Token next;
        parseToken(tokenList[currentPos], next);
        return next;
    }
    return Token{ "EOF", "EOF", -1 };
}

// 解析单个token字符串  
static void parseToken(const std::string& tokenStr, Token& token) {
    size_t pos1 = tokenStr.find(' ');
    size_t pos2 = tokenStr.find(' ', pos1 + 1);

    token.type = tokenStr.substr(0, pos1);
    token.value = tokenStr.substr(pos1 + 1, pos2 - pos1 - 1);
    token.line = std::stoi(tokenStr.substr(pos2 + 1));
}

// 获取下一个token  
static void nextToken() {
    if (currentPos < tokenList.size()) {
        parseToken(tokenList[currentPos++], currentToken);
    }
}

// 语法错误处理  
static void syntaxError(const std::string& message) {
    std::cout << "Syntax error at line " << currentToken.line
        << ": " << message << std::endl;
    exit(1);
}

// 检查当前token是否匹配预期  
static bool match(const std::string& type, const std::string& value) {
    return (currentToken.type == type && currentToken.value == value);
}

/**
 * 语法规则实现
 * 每个函数对应文法中的一个非终结符
 */

 // program → { declaration_list statement_list }  
static void program() {
    if (!match("delimiter", "{")) {
        syntaxError("Expected '{'");
    }
    nextToken();

    declaration_list();
    statement_list();

    if (!match("delimiter", "}")) {
        syntaxError("Expected '}'");
    }
    nextToken();
}

// declaration_list → int identifier ; declaration_list | ε  
static void declaration_list() {
    while (match("keyword", "int")) {
        declaration_stat();
    }
}

// declaration_stat → int identifier ;  
static void declaration_stat() {
    nextToken();  // 跳过 "int"  

    if (currentToken.type != "identifier") {
        syntaxError("Expected identifier after 'int'");
    }
    nextToken();

    if (!match("delimiter", ";")) {
        syntaxError("Expected ';'");
    }
    nextToken();
}

// statement_list → statement statement_list | ε  
static void statement_list() {
    while (!match("delimiter", "}")) {
        statement();
    }
}

// statement → if_stat | while_stat | for_stat | read_stat | write_stat   
//          | compound_stat | assignment_stat | ;  
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
    else if (match("delimiter", ";")) {
        nextToken();
    }
    else {
        // 超前扫描判断是否为赋值语句  
        Token next = lookAhead();
        if (currentToken.type == "identifier" &&
            next.type == "operator" && next.value == "=") {
            assignment_stat();
        }
        else {
            syntaxError("Invalid statement");
        }
    }
}

// if_stat → if ( bool_expression ) statement [else statement]  
static void if_stat() {
    nextToken();  // 跳过 "if"  

    if (!match("delimiter", "(")) {
        syntaxError("Expected '(' after 'if'");
    }
    nextToken();

    bool_expression();

    if (!match("delimiter", ")")) {
        syntaxError("Expected ')'");
    }
    nextToken();

    statement();

    if (match("keyword", "else")) {
        nextToken();
        statement();
    }
}

// while_stat → while ( bool_expression ) statement  
static void while_stat() {
    nextToken();

    if (!match("delimiter", "(")) {
        syntaxError("Expected '('");
    }
    nextToken();

    bool_expression();

    if (!match("delimiter", ")")) {
        syntaxError("Expected ')'");
    }
    nextToken();

    statement();
}

// for_stat → for ( assignment_expression ; bool_expression ;   
//                  assignment_expression ) statement  
static void for_stat() {
    nextToken();

    if (!match("delimiter", "(")) {
        syntaxError("Expected '('");
    }
    nextToken();

    assignment_expression();

    if (!match("delimiter", ";")) {
        syntaxError("Expected ';'");
    }
    nextToken();

    bool_expression();

    if (!match("delimiter", ";")) {
        syntaxError("Expected ';'");
    }
    nextToken();

    assignment_expression();

    if (!match("delimiter", ")")) {
        syntaxError("Expected ')'");
    }
    nextToken();

    statement();
}

// write_stat → write arithmetic_expression ;  
static void write_stat() {
    nextToken();
    arithmetic_expression();

    if (!match("delimiter", ";")) {
        syntaxError("Expected ';'");
    }
    nextToken();
}

// read_stat → read identifier ;  
static void read_stat() {
    nextToken();

    if (currentToken.type != "identifier") {
        syntaxError("Expected identifier after 'read'");
    }
    nextToken();

    if (!match("delimiter", ";")) {
        syntaxError("Expected ';'");
    }
    nextToken();
}

// compound_stat → { statement_list }  
static void compound_stat() {
    nextToken();
    statement_list();

    if (!match("delimiter", "}")) {
        syntaxError("Expected '}'");
    }
    nextToken();
}

// assignment_expression → identifier = arithmetic_expression  
static void assignment_expression() {
    if (currentToken.type != "identifier") {
        syntaxError("Expected identifier");
    }
    nextToken();

    if (!match("operator", "=")) {
        syntaxError("Expected '='");
    }
    nextToken();

    arithmetic_expression();
}

// assignment_stat → assignment_expression ;  
static void assignment_stat() {
    assignment_expression();

    if (!match("delimiter", ";")) {
        syntaxError("Expected ';'");
    }
    nextToken();
}

// bool_expression → arithmetic_expression comparison_op arithmetic_expression  
static void bool_expression() {
    arithmetic_expression();

    if (currentToken.type != "operator" ||
        (currentToken.value != ">" && currentToken.value != "<" &&
            currentToken.value != ">=" && currentToken.value != "<=" &&
            currentToken.value != "==" && currentToken.value != "!=")) {
        syntaxError("Expected comparison operator");
    }
    nextToken();

    arithmetic_expression();
}

// arithmetic_expression → term { (+|-) term }    {(+|-) term} 表示可以有0个或多个 "+term" 或 "-term" 
static void arithmetic_expression() {
    term();

    while (match("operator", "+") || match("operator", "-")) {
        nextToken();
        term();
    }
}

// term → factor { (*|/) factor }  
static void term() {
    factor();

    while (match("operator", "*") || match("operator", "/")) {
        nextToken();
        factor();
    }
}

// factor → ( arithmetic_expression ) | identifier | number  
static void factor() {
    if (match("delimiter", "(")) {
        nextToken();
        arithmetic_expression();

        if (!match("delimiter", ")")) {
            syntaxError("Expected ')'");
        }
        nextToken();
    }
    else if (currentToken.type == "identifier" || currentToken.type == "int") {
        nextToken();
    }
    else {
        syntaxError("Invalid factor");
    }
}

// 主解析函数  
void testParse(const std::vector<std::string>& tokens) {
    tokenList = tokens;
    currentPos = 0;

    try {
        nextToken();
        program();

        if (currentPos < tokenList.size()) {
            syntaxError("Unexpected tokens after program end");
        }

        std::cout << "Syntax analysis completed successfully." << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "Parser error: " << e.what() << std::endl;
    }
}