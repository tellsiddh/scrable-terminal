#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

void createBoard(char board[3][3]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            board[i][j] = ' ';
        }
    }
}

void printBoard(char board[3][3]) {
    printf("  1 2 3\n");
    for (int i = 0; i < 3; i++) {
        printf("%d %c|%c|%c\n", i + 1, board[i][0], board[i][1], board[i][2]);
        if (i < 2) {
            printf("  -----\n");
        }
    }
}

void playerMove(char board[3][3]) {
    int row, col;
    while (1) {
        printf("Player, enter row and column (1-3): ");
        if (scanf("%d %d", &row, &col) != 2) {
            while (getchar() != '\n'); // clear input buffer
            printf("Invalid input. Please enter two numbers.\n");
            continue;
        }
        if (row >= 1 && row <= 3 && col >= 1 && col <= 3 && board[row - 1][col - 1] == ' ') {
            board[row - 1][col - 1] = 'X';
            break;
        } else {
            printf("Invalid move. Try again.\n");
        }
    }
}

int checkWin(char board[3][3]) {
    for (int i = 0; i < 3; i++) {
        if ((board[i][0] == board[i][1] && board[i][1] == board[i][2] && board[i][0] != ' ') ||
            (board[0][i] == board[1][i] && board[1][i] == board[2][i] && board[0][i] != ' ')) {
            return (board[i][0] == 'X' || board[0][i] == 'X') ? 1 : -1;
        }
    }
    if ((board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] != ' ') ||
        (board[0][2] == board[1][1] && board[1][1] == board[2][0] && board[0][2] != ' ')) {
        return (board[0][0] == 'X' || board[0][2] == 'X') ? 1 : -1;
    }
    return 0;
}

int checkDraw(char board[3][3]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == ' ') {
                return 0;
            }
        }
    }
    return 1;
}

int minimax(char board[3][3], int depth, int isMaximizing) {
    int score = checkWin(board);
    if (score != 0) {
        return score;
    }
    if (checkDraw(board)) {
        return 0;
    }

    int bestScore = isMaximizing ? INT_MIN : INT_MAX;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == ' ') {
                board[i][j] = isMaximizing ? 'O' : 'X';
                int currentScore = minimax(board, depth + 1, !isMaximizing);
                board[i][j] = ' ';
                if (isMaximizing) {
                    bestScore = (currentScore > bestScore) ? currentScore : bestScore;
                } else {
                    bestScore = (currentScore < bestScore) ? currentScore : bestScore;
                }
            }
        }
    }
    return bestScore;
}

void computerMove(char board[3][3]) {
    int bestScore = INT_MIN;
    int moveRow = -1;
    int moveCol = -1;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == ' ') {
                board[i][j] = 'O';
                int currentScore = minimax(board, 0, 0);
                board[i][j] = ' ';
                if (currentScore > bestScore) {
                    bestScore = currentScore;
                    moveRow = i;
                    moveCol = j;
                }
            }
        }
    }
    board[moveRow][moveCol] = 'O';
    printf("Computer played at %d %d\n", moveRow + 1, moveCol + 1);
}

int main() {
    char board[3][3];
    int player = 1;

    printf("Welcome to Tic Tac Toe game\n");
    printf("Player: X\n");
    printf("Computer: O\n");

    createBoard(board);
    printBoard(board);

    while (1) {
        if (player == 1) {
            playerMove(board);
        } else {
            computerMove(board);
        }
        printBoard(board);

        int result = checkWin(board);
        if (result == 1) {
            printf("Player wins!\n");
            break;
        } else if (result == -1) {
            printf("Computer wins!\n");
            break;
        } else if (checkDraw(board)) {
            printf("It's a draw!\n");
            break;
        }

        player = (player == 1) ? 2 : 1;
    }

    return 0;
}
