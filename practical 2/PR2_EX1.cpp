#include <iostream>
using namespace std;


const int MAX_STATES = 5;  
const int MAX_SYMBOLS = 2; 

int transitionTable[MAX_STATES][MAX_SYMBOLS];


bool simulateDFA(int initialState, int acceptingState, const string &input) {
    int currentState = initialState;

    for (char ch : input) {
        int symbolIndex = (ch == '0') ? 0 : (ch == '1') ? 1 : -1;
        if (symbolIndex == -1) {
            return false; // Invalid symbol
        }
        currentState = transitionTable[currentState][symbolIndex];
    }

    return currentState == acceptingState;
}

int main() {
    
    int initialState = 0;
    int acceptingState = 3;

    transitionTable[0][0] = 1; 
    transitionTable[0][1] = 0; 

    transitionTable[1][0] = 4; 
    transitionTable[1][1] = 2; 
    transitionTable[2][0] = 4; 
    transitionTable[2][1] = 3; 

    transitionTable[3][0] = 3; 
    transitionTable[3][1] = 3; 

    transitionTable[4][0] = 4; 
    transitionTable[4][1] = 4; 

   
    string input;
    cout << "Enter the input string (using '0' and '1'): ";
    cin >> input;

    if (simulateDFA(initialState, acceptingState, input)) {
        cout << "Accepted\n";
    } else {
        cout << "Rejected\n";
    }

    return 0;
}
