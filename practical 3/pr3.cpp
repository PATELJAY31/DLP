#include <iostream>
#include <fstream>
#include <vector>
#include <cctype>
#include <cstring>
#include <unordered_set>

using namespace std;


enum TokenType {
    KEYWORD, IDENTIFIER, CONSTANT, OPERATOR, PUNCTUATION, STRING, UNKNOWN
};

struct Token {
    string value;
    TokenType type;
};


const char* keywords[] = {"int", "float", "if", "else", "while", "return", "void", "main", "char"};
const int keywordCount = 9;
const char operators[] = "+-*/=<>!&|";
const char delimiters[] = ";(){}[],";

unordered_set<string> symbolTable;
vector<string> lexicalErrors;


bool isKeyword(const string &token) {
    for (int i = 0; i < keywordCount; i++) {
        if (token == keywords[i])
            return true;
    }
    return false;
}


bool isOperator(char c) {
    for (char op : operators) {
        if (c == op)
            return true;
    }
    return false;
}


bool isDelimiter(char c) {
    for (char del : delimiters) {
        if (c == del)
            return true;
    }
    return false;
}


TokenType getTokenType(const string &token) {
    if (isKeyword(token))
        return KEYWORD;
    if (isdigit(token[0])) {
        
        for (char c : token) {
            if (!isdigit(c)) {
                lexicalErrors.push_back(token + " invalid lexeme");
                return UNKNOWN;
            }
        }
        return CONSTANT;
    }
    if (isalpha(token[0]) || token[0] == '_') {
        if (token != "main") { 
            symbolTable.insert(token);
        }
        return IDENTIFIER;
    }
    return UNKNOWN;
}


vector<Token> tokenize(string line) {
    vector<Token> tokens;
    string token;
    bool inString = false;
    bool inSingleLineComment = false;
    bool inMultiLineComment = false;

    for (size_t i = 0; i < line.size(); i++) {
        if (inSingleLineComment) {
            break; 
        }
        if (inMultiLineComment) {
            if (line[i] == '*' && i + 1 < line.size() && line[i + 1] == '/') {
                inMultiLineComment = false;
                i++; 
            }
            continue;
        }
        if (line[i] == '/' && i + 1 < line.size()) {
            if (line[i + 1] == '/') {
                inSingleLineComment = true;
                continue;
            } else if (line[i + 1] == '*') {
                inMultiLineComment = true;
                i++; // Skip '/*'
                continue;
            }
        }
        if (line[i] == '"') {
            if (inString) {
                token += line[i];
                tokens.push_back({token, STRING});
                token.clear();
            } else {
                token += line[i];
            }
            inString = !inString;
        } else if (!inString) {
            if (isspace(line[i])) {
                if (!token.empty()) {
                    tokens.push_back({token, getTokenType(token)});
                    token.clear();
                }
            } else if (isOperator(line[i]) || isDelimiter(line[i])) {
                if (!token.empty()) {
                    tokens.push_back({token, getTokenType(token)});
                    token.clear();
                }
                tokens.push_back({string(1, line[i]), isOperator(line[i]) ? OPERATOR : PUNCTUATION});
            } else {
                token += line[i];
            }
        } else {
            token += line[i];
        }
    }
    if (!token.empty()) {
        tokens.push_back({token, getTokenType(token)});
    }
    return tokens;
}

int main() {
    ifstream file("file1.c");
    if (!file) {
        cerr << "Error opening file.c" << endl;
        return 1;
    }

    cout << "TOKENS\n";
    string line;
    while (getline(file, line)) {
        vector<Token> tokens = tokenize(line);
        for (const auto &t : tokens) {
            cout << (t.type == KEYWORD ? "Keyword: " :
                     t.type == IDENTIFIER ? "Identifier: " :
                     t.type == CONSTANT ? "Constant: " :
                     t.type == OPERATOR ? "Operator: " :
                     t.type == PUNCTUATION ? "Punctuation: " :
                     t.type == STRING ? "String: " : "Unknown: ")
                 << t.value << endl;
        }
    }
    file.close();

    if (!lexicalErrors.empty()) {
        cout << "\nLEXICAL ERRORS\n";
        for (const auto &error : lexicalErrors) {
            cout << error << endl;
        }
    }

    if (!symbolTable.empty()) {
        cout << "\nSYMBOL TABLE ENTRIES\n";
        int index = 1;
        for (const auto &symbol : symbolTable) {
            cout << index++ << ") " << symbol << endl;
        }
    }
    return 0;
}
