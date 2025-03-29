#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <string>
#include <algorithm>

using namespace std;

class Grammar {
private:
    map<char, vector<string>> productions;
    set<char> terminals;
    set<char> nonTerminals;
    map<char, set<char>> first;
    map<char, set<char>> follow;

public:
    Grammar() {
        productions['S'] = {"ABC", "D"};
        productions['A'] = {"a", "e"};
        productions['B'] = {"b", "e"};
        productions['C'] = {"(S)", "c"};
        productions['D'] = {"AC"};

        terminals = {'a', 'b', 'c', '(', ')', 'e'};
        nonTerminals = {'S', 'A', 'B', 'C', 'D'};

        for (char nt : nonTerminals) {
            first[nt] = set<char>();
            follow[nt] = set<char>();
        }

        follow['S'].insert('$');
        
        computeFirstSets();
        computeFollowSets();
    }

    set<char> firstOfString(const string& str) {
        set<char> result;
        if (str.empty() || str == "e") {
            result.insert('e');
            return result;
        }

        bool allDeriveEpsilon = true;
        for (size_t i = 0; i < str.length(); i++) {
            char symbol = str[i];
            
            if (terminals.find(symbol) != terminals.end()) {
                result.insert(symbol);
                allDeriveEpsilon = false;
                break;
            } else {
                for (char terminal : first[symbol]) {
                    if (terminal != 'e') {
                        result.insert(terminal);
                    }
                }
                
                if (first[symbol].find('e') == first[symbol].end()) {
                    allDeriveEpsilon = false;
                    break;
                }
            }
        }
        
        if (allDeriveEpsilon) {
            result.insert('e');
        }
        
        return result;
    }

    void computeFirstSets() {
        for (char terminal : terminals) {
            if (terminal != 'e') {
                first[terminal] = {terminal};
            }
        }

        bool changed = true;
        while (changed) {
            changed = false;
            
            for (char nt : nonTerminals) {
                for (const string& production : productions[nt]) {
                    if (production[0] == 'e') {
                        if (first[nt].find('e') == first[nt].end()) {
                            first[nt].insert('e');
                            changed = true;
                        }
                        continue;
                    }

                    bool allDeriveEpsilon = true;
                    for (size_t i = 0; i < production.length(); i++) {
                        char symbol = production[i];
                        
                        if (terminals.find(symbol) != terminals.end()) {
                            if (first[nt].find(symbol) == first[nt].end()) {
                                first[nt].insert(symbol);
                                changed = true;
                            }
                            allDeriveEpsilon = false;
                            break;
                        } else {
                            for (char terminal : first[symbol]) {
                                if (terminal != 'e' && first[nt].find(terminal) == first[nt].end()) {
                                    first[nt].insert(terminal);
                                    changed = true;
                                }
                            }
                            
                            if (first[symbol].find('e') == first[symbol].end()) {
                                allDeriveEpsilon = false;
                                break;
                            }
                        }
                    }
                    
                    if (allDeriveEpsilon && first[nt].find('e') == first[nt].end()) {
                        first[nt].insert('e');
                        changed = true;
                    }
                }
            }
        }
    }

    void computeFollowSets() {
        bool changed = true;
        while (changed) {
            changed = false;
            
            for (char nt : nonTerminals) {
                for (auto& entry : productions) {
                    char lhs = entry.first;
                    for (const string& production : entry.second) {
                        for (size_t i = 0; i < production.length(); i++) {
                            if (production[i] == nt) {
                                if (i == production.length() - 1) {
                                    for (char terminal : follow[lhs]) {
                                        if (follow[nt].find(terminal) == follow[nt].end()) {
                                            follow[nt].insert(terminal);
                                            changed = true;
                                        }
                                    }
                                } else {
                                    string beta = production.substr(i + 1);
                                    set<char> firstBeta = firstOfString(beta);
                                    
                                    for (char terminal : firstBeta) {
                                        if (terminal != 'e' && follow[nt].find(terminal) == follow[nt].end()) {
                                            follow[nt].insert(terminal);
                                            changed = true;
                                        }
                                    }
                                    
                                    if (firstBeta.find('e') != firstBeta.end()) {
                                        for (char terminal : follow[lhs]) {
                                            if (follow[nt].find(terminal) == follow[nt].end()) {
                                                follow[nt].insert(terminal);
                                                changed = true;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    void printSets() {
        vector<char> sortedNT(nonTerminals.begin(), nonTerminals.end());
        sort(sortedNT.begin(), sortedNT.end());
        
        for (char nt : sortedNT) {
            cout << "First(" << nt << ") = {";
            vector<char> sortedFirst(first[nt].begin(), first[nt].end());
            sort(sortedFirst.begin(), sortedFirst.end());
            
            for (size_t i = 0; i < sortedFirst.size(); i++) {
                if (sortedFirst[i] == 'e') {
                    cout << "ε";
                } else {
                    cout << sortedFirst[i];
                }
                if (i < sortedFirst.size() - 1) {
                    cout << ", ";
                }
            }
            cout << "}" << endl;
        }
        
        for (char nt : sortedNT) {
            cout << "Follow(" << nt << ") = {";
            vector<char> sortedFollow(follow[nt].begin(), follow[nt].end());
            sort(sortedFollow.begin(), sortedFollow.end());
            
            for (size_t i = 0; i < sortedFollow.size(); i++) {
                cout << sortedFollow[i];
                if (i < sortedFollow.size() - 1) {
                    cout << ", ";
                }
            }
            cout << "}" << endl;
        }
    }
};

int main() {
    Grammar grammar;
    grammar.printSets();
    return 0;
}