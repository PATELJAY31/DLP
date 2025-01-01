#include <iostream>
using namespace std;

const int MAX_STATES = 5;  
const int MAX_SYMBOLS = 3; 


int transitionTable[MAX_STATES][MAX_SYMBOLS];


bool DFA(int initialState, int acceptingStates[], int numAcceptingStates, const string &input) {
    int currentState = initialState;

    for (char ch : input) {
        int symbolIndex;
        if (ch == 'a') symbolIndex = 0;
        else if (ch == 'b') symbolIndex = 1;
        else if (ch == 'c') symbolIndex = 2;
        else return false; 

        currentState = transitionTable[currentState][symbolIndex];
    }

    
    for (int i = 0; i < numAcceptingStates; i++) {
        if (currentState == acceptingStates[i]) {
            return true;
        }
    }
    return false;
}

int main() {
   
    int initialState = 0;
    int acceptingStates[] = {1, 2, 3}; 

    
    transitionTable[0][0] = 1; 
    transitionTable[0][1] = 2; 
    transitionTable[0][2] = 3; 

    transitionTable[1][0] = 1; 
    transitionTable[1][1] = 4; 
    transitionTable[1][2] = 4; 

    transitionTable[2][0] = 4; 
    transitionTable[2][1] = 2; 
    transitionTable[2][2] = 4; 

    transitionTable[3][0] = 4; 
    transitionTable[3][1] = 4; 
    transitionTable[3][2] = 3; 

    transitionTable[4][0] = 4; 
    transitionTable[4][1] = 4; 
    transitionTable[4][2] = 4; 

    
    string input;
    cout << "Enter the input string (using 'a', 'b', and 'c'): ";
    cin >> input;

    if (DFA(initialState, acceptingStates, 3, input)) {
        cout << "Accepted\n";
    } else {
        cout << "Rejected\n";
    }

    return 0;
}
