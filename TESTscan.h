#pragma once

#include <iostream>  
#include <fstream>  
#include <string>  
#include <cctype>  
#include <vector>  
#include <map>  
#include <algorithm> 

using namespace std;

// 保留字  
const vector<string> keywords = { "if", "else", "for", "while", "int", "write", "read" };

// 运算符和分界符  
const vector<string> operators = { "+", "-", "*", "/", "=", "<", ">", "<=", ">=", "==", "!=", };
const vector<char> delimiters = { '(', ')', '{', '}', ';' };

void outputTokens(const vector<string>& tokens, ofstream& outFile);
void outputErrors(const vector<string>& errors, ofstream& outFile);
bool isTableSymbol(const char& op);
bool isKeyword(const string& word);
bool canOperator(const string& op);
bool isOperator(const string& op);
bool isIdentifierChar(char c);
bool isValidIdentifier(const string& id);
bool isValidUnsignedInteger(const string& num);

void testScan(ifstream& inputFile, ofstream& outputFile, vector<string>& tokens, vector<string>& errors);