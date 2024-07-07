# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <ctype.h>
# include <stdbool.h>

# define ROWS 3
# define COLS 3
# define word_file "words.txt"
struct Player {
    char name[100];
    int score;
};


char grid[ROWS][COLS];

int score_for_word(const char *word, int found) {
    int score = 0;
    for (int i = 0; i < strlen(word); i++) {
        switch (word[i]) {
            case 'A':
            case 'E':
            case 'I':
            case 'O':
            case 'U':
            case 'L':
            case 'N':
            case 'R':
            case 'S':
            case 'T':
                score += 1;
                break;
            case 'D':
            case 'G':
                score += 2;
                break;
            case 'B':
            case 'C':
            case 'M':
            case 'P':
                score += 3;
                break;
            case 'F':
            case 'H':
            case 'V':
            case 'W':
            case 'Y':
                score += 4;
                break;
            case 'K':
                score += 5;
                break;
            case 'J':
            case 'X':
                score += 8;
                break;
            case 'Q':
            case 'Z':
                score += 10;
                break;
            default:
                break;
        }
    }
    score *= found;
    printf("Score for word %s is %d (found %d times)\n", word, score, found);
    return score;
}

int check_word(const char *users_word) {
    FILE *file = fopen(word_file, "r");
    char word[100];
    int found = 0;
    char rev_word[100];

    if (!file) {
        printf("File can't be opened\n");
        return 1;
    }
    for (int i = 0; i < strlen(users_word); i++)
    {
        rev_word[i] = users_word[strlen(users_word) - i - 1];
    }
    rev_word[strlen(users_word)] = '\0';
    printf("Reversed word: %s\n", rev_word);
    while (fgets(word, sizeof(word), file)) {
        word[strcspn(word, "\n")] = 0;
        if (strcmp(users_word, word) == 0) {
            printf("Word %s has been found\n", word);
            found++;
            continue;
        }
        if (strcmp(rev_word, word) == 0){
            printf("Word %s has been found\n", word);
            found++;
            continue;
        }
    }

    fclose(file);
    return found;
}

int ask_user_for_word(char *users_word) {
    int found = 0;

    while (!found) {
        printf("Please enter a word: ");
        scanf("%99s", users_word);
        for (int i = 0; i < strlen(users_word); i++)
        {
            users_word[i] = toupper(users_word[i]);
        }
        
        found = check_word(users_word);
        if (!found) {
            printf("Please enter a valid word\n");
        } else {
            printf("Word %s is correct\n", users_word);
            printf("We found it %d times\n", found);
        }
    }
    return found;
}

void make_empty_board(){

    for(int i = 0; i < ROWS; i++){
        for(int j = 0; j < COLS; j++){
            grid[i][j] = '_';
            printf("%c ", grid[i][j]);
        }
        printf("\n");
    }
}

bool is_grid_full(){
    for (int i = 0; i < ROWS; i++){
        for (int j = 0; j < COLS; j++){
            if (grid[i][j] == '_' || grid[i][j] == '_'){
                return false;
        }
        }
    }
    return true;
}

int game_over(){
    return 0;
}

bool can_place_word(const char *word, int row, int col, char direction) {
    int length = strlen(word);
    if (direction == 'h') {
        if (col + length > COLS) return false;  // Check horizontal bounds
        for (int i = 0; i < length; i++) {
            if (grid[row][col + i] != '_' && grid[row][col + i] != word[i]) return false;  // Check for clash
        }
    } else if (direction == 'v') {
        if (row + length > ROWS) return false;  // Check vertical bounds
        for (int i = 0; i < length; i++) {
            if (grid[row + i][col] != '_' && grid[row + i][col] != word[i]) return false;  // Check for clash
        }
    }
    return true;
}


void add_word(char *users_word){

    int row, col;
    char direction;

    printf("Enter the row: ");
    scanf("%d", &row);
    printf("Enter the column: ");
    scanf("%d", &col);
    printf("Enter the direction: ");
    scanf(" %c", &direction);
    printf("Users word: %s\n", users_word);
    printf("Your word %s will be placed at row %d and column %d in direction %c.\n", users_word, row, col, direction);
    
    row--; // Adjust for zero-indexed array
    col--; // Adjust for zero-indexed array

    if (can_place_word(users_word, row, col, direction)) {
        for (int i = 0; i < strlen(users_word); i++) {
            if (direction == 'h') {
                grid[row][col + i] = users_word[i];  // Place horizontally
            } else if (direction == 'v') {
                grid[row + i][col] = users_word[i];  // Place vertically
            }
        }
    } else {
        printf("Cannot place word %s at (%d,%d) in direction %c\n", users_word, row + 1, col + 1, direction);
        add_word(users_word);  // Ask again if placement failed
        return;
    }

    for(int i = 0; i < ROWS; i++){
        for(int j = 0; j < COLS; j++){
            printf("%c ", grid[i][j]);
        }
        printf("\n");
    }

}

int main() {
    char users_word[100];
    bool player_one_turn = true;

    printf("Welcome to the game of scrabble\n");
    struct Player player1, player2;
    player1.score = 0;
    player2.score = 0;
    strcpy(player1.name, "Sid");
    strcpy(player2.name, "Computer");
    printf("%s vs %s\n", player1.name, player2.name);
    printf("Score is %d - %d\n", player1.score, player2.score);
    make_empty_board();
    while (!is_grid_full()) {
        int found = ask_user_for_word(users_word);
        if (check_word(users_word)) {  // If the word is valid
            add_word(users_word);
            int word_score = score_for_word(users_word, found);
            if (player_one_turn) {
                player1.score += word_score;
                printf("%s's new score: %d\n", player1.name, player1.score);
            } else {
                player2.score += word_score;
                printf("%s's new score: %d\n", player2.name, player2.score);
            }
            player_one_turn = !player_one_turn;  // Toggle turn
        }
    }
    printf("Final score: %s: %d, %s: %d\n", player1.name, player1.score, player2.name, player2.score);
    printf("Game over\n");
    game_over();
    return 0;
}
