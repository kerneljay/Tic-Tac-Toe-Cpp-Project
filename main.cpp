#include <iostream>
#include <limits>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>
#include <windows.h>  // Only include if you're on Windows

using namespace std;

char board[9] = { '1', '2', '3', '4', '5', '6', '7', '8', '9' };
char currentPlayer;
string player1, player2;
bool singlePlayer = false;



// Function to change console text color
void setTextColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hConsole == INVALID_HANDLE_VALUE) {
        cout << "Error getting console handle\n";
        return;
    }
    SetConsoleTextAttribute(hConsole, color);
}

// Display the game board with color highlights for 'X' (blue) and 'O' (red)
void displayBoard() {
    cout << '\n';
    for (int i = 0; i < 9; i++) {
        if (board[i] == 'X') {
            setTextColor(9);  // Blue for 'X'
        }
        else if (board[i] == 'O') {
            setTextColor(12);  // Red for 'O'
        }
        else {
            setTextColor(7);  // Default color for numbers
        }
        cout << " " << board[i];

        if (i == 2 || i == 5) {
            cout << '\n' << "---|---|---" << '\n';
        }
        else if (i != 8) {
            cout << " |";
        }
    }
    cout << '\n';
    setTextColor(7);  // Reset to default color
}

// Check if a player has won
bool checkWin() {
    int winConditions[8][3] = {
        {0, 1, 2}, {3, 4, 5}, {6, 7, 8}, // Rows
        {0, 3, 6}, {1, 4, 7}, {2, 5, 8}, // Columns
        {0, 4, 8}, {2, 4, 6}             // Diagonals
    };

    for (int i = 0; i < 8; i++) {
        if (board[winConditions[i][0]] == board[winConditions[i][1]] &&
            board[winConditions[i][1]] == board[winConditions[i][2]]) {
            return true;
        }
    }
    return false;
}

// Check if the board is full (draw condition)
bool checkDraw() {
    for (int i = 0; i < 9; i++) {
        if (board[i] != 'X' && board[i] != 'O') {
            return false;
        }
    }
    return true;
}

// Switch between players
void switchPlayer() {
    currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
}

// Validate the move and apply it
bool makeMove(int position) {
    if (position < 1 || position > 9 || board[position - 1] == 'X' || board[position - 1] == 'O') {
        setTextColor(14);  // Yellow for invalid move warning
        cout << "Invalid move. Try again.\n";
        setTextColor(7);   // Reset to default color
        return false;
    }
    board[position - 1] = currentPlayer;
    return true;
}

// Get valid integer input from user
int getInput() {
    int move;
    while (true) {
        setTextColor(10);  // Green for player input prompt
        cout << "Enter your move (1-9): ";
        setTextColor(7);   // Reset to default color
        if (cin >> move && move >= 1 && move <= 9) {
            break;
        }
        else {
            setTextColor(14);  // Yellow for invalid input warning
            cout << "Invalid input. Please enter a number between 1 and 9.\n";
            setTextColor(7);   // Reset to default color
            cin.clear();
        }
    }
    return move;
}

// AI's move with random delay
void aiMove() {
    srand(time(0));  // Seed the random number generator

    // Temporarily disable user input
    cin.clear();  // Clear the error state

    // Simulate AI "thinking" with a random delay
    int waitTime = rand() % 3 + 1;  // Random time between 1 and 3 seconds
    setTextColor(10);  // Green for AI thinking prompt
    cout << "AI is deciding...\n";
    setTextColor(7);   // Reset to default color
    this_thread::sleep_for(chrono::seconds(waitTime));  // Wait for the generated time

    // AI chooses a random position
    int move;
    do {
        move = rand() % 9 + 1;
    } while (board[move - 1] == 'X' || board[move - 1] == 'O');

    cout << "AI chooses position: " << move << '\n';
    board[move - 1] = currentPlayer;
}
// Initialize the game settings
void initializeGame() {
    setTextColor(10);  // Green for welcome message
    cout << "Welcome to Tic-Tac-Toe!\n";
    setTextColor(7);   // Reset to default color
    cout << "Enter Player 1's name: ";
    cin >> player1;
    cout << "Do you want to play against the AI? (y/n): ";
    char choice;
    cin >> choice;
    if (choice == 'y' || choice == 'Y') {
        singlePlayer = true;
        player2 = "AI";
    }
    else {
        cout << "Enter Player 2's name: ";
        cin >> player2;
    }

    currentPlayer = 'X';  // Player 1 always starts
}

// Reset the board
void resetBoard() {
    for (int i = 0; i < 9; i++) {
        board[i] = '1' + i;
    }
}

// Play the game
void playGame() {
    bool gameWon = false;
    bool isDraw = false;

    while (!gameWon && !isDraw) {
        displayBoard();
        if (currentPlayer == 'X') {
            setTextColor(9);  // Blue for Player 1's turn
            cout << player1 << "'s turn (" << currentPlayer << ")\n";
        }
        else {
            setTextColor(12);  // Red for AI's turn
            cout << player2 << "'s turn (" << currentPlayer << ")\n";
        }
        setTextColor(7);   // Reset to default color

        if (singlePlayer && currentPlayer == 'O') {
            aiMove();  // AI makes a move if in single-player mode
        }
        else {
            int position = getInput();
            if (!makeMove(position)) continue;
        }

        gameWon = checkWin();
        isDraw = checkDraw();

        if (!gameWon && !isDraw) {
            switchPlayer();
        }
    }

    displayBoard();

    if (gameWon) {
        setTextColor(currentPlayer == 'X' ? 9 : 12);  // Blue or Red for winner announcement
        cout << (currentPlayer == 'X' ? player1 : player2) << " wins!\n";
    }
    else if (isDraw) {
        setTextColor(14);  // Yellow for draw announcement
        cout << "It's a draw!\n";
    }
    setTextColor(7);   // Reset to default color
}

// Ask to replay the game
bool askReplay() {
    char replayChoice;
    setTextColor(10);  // Green for replay prompt
    cout << "Do you want to play again? (y/n): ";
    setTextColor(7);   // Reset to default color
    cin >> replayChoice;
    return (replayChoice == 'y' || replayChoice == 'Y');
}

int main() {
    initializeGame();

    do {
        resetBoard();
        playGame();
    } while (askReplay());

    setTextColor(10);  // Green for thanks message
    cout << "Thanks for playing!\n";
    setTextColor(7);   // Reset to default color
    return 0;
}
