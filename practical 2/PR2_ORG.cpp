//check for abbabab

#include <iostream>
using namespace std;


const int MAX_STATES = 5;  
const int MAX_SYMBOLS = 2; 


int transitionTable[MAX_STATES][MAX_SYMBOLS];


bool DFA(int initialState, int acceptingState, const string &input) {
    int currentState = initialState;

    for (char ch : input) {
        
        int symbolIndex = (ch == 'a') ? 0 : (ch == 'b') ? 1 : -1;
        if (symbolIndex == -1) {
            return false; 
        }

        currentState = transitionTable[currentState][symbolIndex];
    }

    return currentState == acceptingState;
}

int main() {
    
    int initialState = 1;
    int acceptingState = 2;

 
    // Transition table is 1-indexed to match the problem description
    transitionTable[1][0] = 2; 
    transitionTable[1][1] = 3; 
    transitionTable[2][0] = 1; 
    transitionTable[2][1] = 4; 
    transitionTable[3][0] = 4; 
    transitionTable[3][1] = 1; 
    transitionTable[4][0] = 3; 
    transitionTable[4][1] = 2; 

    
    string input;
    cout << "Enter the input string (using 'a' and 'b'): ";
    cin >> input;

    if (DFA(initialState, acceptingState, input)) {
        cout << "Accepted\n";
    } else {
        cout << "Rejected\n";
    }

    return 0;
}
