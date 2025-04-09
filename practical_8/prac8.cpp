#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <algorithm>

using namespace std;

class PredictiveParser {
private:
    map<char, vector<string>> grammar;
    map<char, set<char>> firstSets;
    map<char, set<char>> followSets;
    set<char> terminals;
    set<char> nonTerminals;
    map<char, map<char, vector<string>>> parsingTable;
    bool isLL1;

    void constructParsingTable() {
        for (const auto& nt : nonTerminals) {
            for (const auto& t : terminals) {
                parsingTable[nt][t] = vector<string>();
            }
        }

        for (const auto& rule : grammar) {
            char nonTerminal = rule.first;
            for (const auto& production : rule.second) {
                if (production == "ε") {
                    for (char terminal : followSets[nonTerminal]) {
                        addToTable(nonTerminal, terminal, production);
                    }
                } else {
                    set<char> firstOfProduction = computeFirstOfProduction(production);
                    for (char terminal : firstOfProduction) {
                        if (terminal != 'ε') {
                            addToTable(nonTerminal, terminal, production);
                        }
                    }
                    
                    if (firstOfProduction.find('ε') != firstOfProduction.end()) {
                        for (char terminal : followSets[nonTerminal]) {
                            addToTable(nonTerminal, terminal, production);
                        }
                    }
                }
            }
        }

        isLL1 = true;
        for (const auto& nt : nonTerminals) {
            for (const auto& t : terminals) {
                if (parsingTable[nt][t].size() > 1) {
                    isLL1 = false;
                    return;
                }
            }
        }
    }

    void addToTable(char nonTerminal, char terminal, const string& production) {
        for (const auto& existingProduction : parsingTable[nonTerminal][terminal]) {
            if (existingProduction == production) {
                return;
            }
        }
        
        if (!parsingTable[nonTerminal][terminal].empty()) {
            isLL1 = false;
        }
        
        parsingTable[nonTerminal][terminal].push_back(production);
    }

    set<char> computeFirstOfProduction(const string& production) {
        if (production.empty() || production == "ε") {
            return {'ε'};
        }
        
        set<char> firstSet;
        bool hasEpsilon = true;
        
        for (char symbol : production) {
            if (nonTerminals.find(symbol) != nonTerminals.end()) {
                set<char> symbolFirst = firstSets[symbol];
                
                for (char c : symbolFirst) {
                    if (c != 'ε') {
                        firstSet.insert(c);
                    }
                }
                
                if (symbolFirst.find('ε') == symbolFirst.end()) {
                    hasEpsilon = false;
                    break;
                }
            } else {
                firstSet.insert(symbol);
                hasEpsilon = false;
                break;
            }
        }
        
        if (hasEpsilon) {
            firstSet.insert('ε');
        }
        
        return firstSet;
    }

public:
    PredictiveParser(const map<char, vector<string>>& g,
                    const map<char, set<char>>& first,
                    const map<char, set<char>>& follow)
        : grammar(g), firstSets(first), followSets(follow), isLL1(true) {
        
        for (const auto& rule : grammar) {
            nonTerminals.insert(rule.first);
        }
        
        for (const auto& rule : grammar) {
            for (const auto& production : rule.second) {
                for (char symbol : production) {
                    if (nonTerminals.find(symbol) == nonTerminals.end() && symbol != 'ε') {
                        terminals.insert(symbol);
                    }
                }
            }
        }
        
        terminals.insert('$');
        
        constructParsingTable();
    }

    bool parse(const string& inputString) {
        if (!isLL1) {
            cout << "Cannot parse: Grammar is not LL(1)" << endl;
            return false;
        }
        
        string input = inputString + "$";
        vector<char> stack = {'$', *nonTerminals.begin()};
        
        size_t inputIndex = 0;
        
        while (stack.back() != '$') {
            char top = stack.back();
            char currentInput = input[inputIndex];
            
            if (terminals.find(top) != terminals.end()) {
                if (top == currentInput) {
                    stack.pop_back();
                    inputIndex++;
                } else {
                    return false;
                }
            } else {
                if (!parsingTable[top][currentInput].empty()) {
                    stack.pop_back();
                    string production = parsingTable[top][currentInput][0];
                    
                    if (production != "ε") {
                        for (int i = production.size() - 1; i >= 0; i--) {
                            stack.push_back(production[i]);
                        }
                    }
                } else {
                    return false;
                }
            }
        }
        
        return inputIndex == input.size() - 1;
    }

    void printParsingTable() {
        cout << "Parsing Table:" << endl;
        
        cout << "   |";
        for (char t : terminals) {
            cout << " " << t << " |";
        }
        cout << endl;
        
        string separator = "---+";
        for (size_t i = 0; i < terminals.size(); i++) {
            separator += "---+";
        }
        cout << separator << endl;
        
        for (char nt : nonTerminals) {
            cout << " " << nt << " |";
            for (char t : terminals) {
                if (parsingTable[nt][t].empty()) {
                    cout << "   |";
                } else {
                    cout << " " << parsingTable[nt][t][0] << " |";
                }
            }
            cout << endl;
        }
        
        cout << "\nIs LL(1): " << (isLL1 ? "Yes" : "No") << endl;
    }

    bool isLL1Grammar() const {
        return isLL1;
    }
};

int main() {
    map<char, vector<string>> grammar = {
        {'S', {"AB"}},
        {'A', {"aA", "ε"}},
        {'B', {"bB", "ε"}}
    };
    
    map<char, set<char>> firstSets = {
        {'S', {'a', 'b', 'ε'}},
        {'A', {'a', 'ε'}},
        {'B', {'b', 'ε'}}
    };
    
    map<char, set<char>> followSets = {
        {'S', {'$'}},
        {'A', {'b', '$'}},
        {'B', {'$'}}
    };
    
    PredictiveParser parser(grammar, firstSets, followSets);
    parser.printParsingTable();
    
    vector<string> testStrings = {
        "ab",
        "aabb",
        "",
        "ba"
    };
    
    for (const auto& str : testStrings) {
        bool valid = parser.parse(str);
        cout << "String \"" << str << "\": " << (valid ? "Valid" : "Invalid") << " string" << endl;
    }
    
    return 0;
}
