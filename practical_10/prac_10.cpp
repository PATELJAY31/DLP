#include <iostream>
#include <sstream>
#include <stack>
#include <vector>
#include <map>
#include <cmath>
#include <cctype>

using namespace std;

enum TokenType { NUMBER, OPERATOR, LPAREN, RPAREN, INVALID };

struct Token {
    TokenType type;
    double value;
    char op;
};

map<char, pair<int, string>> precedence = {
    {'^', {4, "right"}},
    {'*', {3, "left"}},
    {'/', {3, "left"}},
    {'+', {2, "left"}},
    {'-', {2, "left"}}
};

vector<Token> tokenize(const string& expr) {
    vector<Token> tokens;
    istringstream iss(expr);
    char ch;
    while (iss >> ch) {
        if (isdigit(ch) || ch == '.') {
            iss.putback(ch);
            double val;
            iss >> val;
            tokens.push_back({NUMBER, val, 0});
        } else if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '^') {
            tokens.push_back({OPERATOR, 0, ch});
        } else if (ch == '(') {
            tokens.push_back({LPAREN, 0, 0});
        } else if (ch == ')') {
            tokens.push_back({RPAREN, 0, 0});
        } else if (!isspace(ch)) {
            tokens.push_back({INVALID, 0, 0});
        }
    }
    return tokens;
}

bool apply_operator(stack<double>& output, char op) {
    if (output.size() < 2) return false;
    double b = output.top(); output.pop();
    double a = output.top(); output.pop();
    if (op == '+') output.push(a + b);
    else if (op == '-') output.push(a - b);
    else if (op == '*') output.push(a * b);
    else if (op == '/') output.push(a / b);
    else if (op == '^') output.push(pow(a, b));
    else return false;
    return true;
}

string evaluate(const string& expr) {
    vector<Token> tokens = tokenize(expr);
    for (auto& t : tokens) if (t.type == INVALID) return "Invalid expression";

    stack<double> output;
    stack<char> ops;

    for (auto& token : tokens) {
        if (token.type == NUMBER) {
            output.push(token.value);
        } else if (token.type == OPERATOR) {
            while (!ops.empty() && ops.top() != '(') {
                char top = ops.top();
                if ((precedence[token.op].second == "left" && precedence[token.op].first <= precedence[top].first) ||
                    (precedence[token.op].second == "right" && precedence[token.op].first < precedence[top].first)) {
                    ops.pop();
                    if (!apply_operator(output, top)) return "Invalid expression";
                } else break;
            }
            ops.push(token.op);
        } else if (token.type == LPAREN) {
            ops.push('(');
        } else if (token.type == RPAREN) {
            bool matched = false;
            while (!ops.empty()) {
                if (ops.top() == '(') {
                    matched = true;
                    ops.pop();
                    break;
                } else {
                    char top = ops.top(); ops.pop();
                    if (!apply_operator(output, top)) return "Invalid expression";
                }
            }
            if (!matched) return "Invalid expression";
        }
    }

    while (!ops.empty()) {
        char top = ops.top(); ops.pop();
        if (top == '(' || top == ')') return "Invalid expression";
        if (!apply_operator(output, top)) return "Invalid expression";
    }

    if (output.size() != 1) return "Invalid expression";
    ostringstream result;
    result << output.top();
    return result.str();
}

int main() {
    vector<string> inputs = {
        "(3 + 5) * 2",
        "3 + 5 * 2",
        "3 + 5 * 2 ^ 2",
        "3 + (5 * 2)",
        "3 + 5 ^ 2 * 2",
        "3 * (5 + 2)",
        "(3 + 5) ^ 2",
        "3 ^ 2 ^ 3",
        "3 ^ 2 + 5 * 2",
        "3 + ^ 5",
        "(3 + 5 * 2",
        "(3 + 5 * 2 ^ 2 - 8) / 4 ^ 2 + 6"
    };

    for (auto& expr : inputs) {
        cout << "Input: " << expr << "\nOutput: " << evaluate(expr) << "\n\n";
    }

    return 0;
}
