#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include <limits.h>

#define SIZE 16
#define PLAYERS 2
#define NBOT 1

char board[SIZE][SIZE];
char CHIP[PLAYERS] = "XO";
int player = 0;

void INIT_SCREEN(void) {
	initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();
}

void DONE_SCREEN(void) {
    endwin();
    exit(0);
}

void init_board(void) {
    for (int i=0; i<SIZE; i++) {
        for (int j=0; j<SIZE; j++) {
            board[i][j] = ' ';
        }
    }
}

void draw_board(void) {
    move(0, 0);
    for (int i=SIZE-1; i>=0; i--) {
        for (int j=0; j<SIZE; j++) {
            printw("|%c", board[i][j]);
        }
        printw("|\n");
    }

    for (int i=0; i<SIZE; i++) {
        printw("+-");
    }
    printw("+\n");

    char letter = 'A';
    for (int i=0; i<SIZE; i++) {
        printw(" %c", letter);
        letter++;
    }
    printw("\n");
    refresh();
}

int get_col(void) {
    int key;
    while (key!=KEY_BACKSPACE && key!=127) {
        key = getch();
        if ((key >= 'A' && key < 'A' + SIZE)) {
            return key - 'A';
        } else if (key >= 'a' && key < 'a' + SIZE) {
            return key - 'a';
        }
    }
    return -1;
}

int add_coin(int col, int player) {
    for (int i=0; i<SIZE; i++) {
        if (board[i][col]==' ') {
            board[i][col]=CHIP[player];
            return (player + 1) % PLAYERS;
        }
    }
    return player;
}

int is_valid_move(int col) {
    return board[SIZE - 1][col]==' ';
}

int undo_move(int col) {
    for (int i = SIZE - 1; i >= 0; i--) {
        if (board[i][col]!=' ') {
            board[i][col]=' ';
            return 1;
        }
    }
    return 0;
}

int game_over(void) {
    int completed = 1;
    for (int i=0; i<SIZE; i++) {
        for (int j=0; j<SIZE; j++) {
            if (board[i][j]==' ') {
                completed = 0;
                continue;
            } if (j<=SIZE-4
                && board[i][j]==board[i][j+1]
                && board[i][j]==board[i][j+2]
                && board[i][j]==board[i][j+3]) {
                return board[i][j];
            } if (i<=SIZE-4
                && board[i][j]==board[i+1][j]
                && board[i][j]==board[i+2][j]
                && board[i][j]==board[i+3][j]) {
                return board[i][j];
            } if (i<=SIZE-4 && j<=SIZE-4
                && board[i][j]==board[i+1][j+1]
                && board[i][j]==board[i+2][j+2]
                && board[i][j]==board[i+3][j+3]) {
                return board[i][j];
            } if (i<=SIZE-4 && j>=4
                && board[i][j]==board[i+1][j-1]
                && board[i][j]==board[i+2][j-2]
                && board[i][j]==board[i+3][j-3]) {
                return board[i][j];
            }
        }
    }
    return completed;
}

int evaluate_board(void) {
    int score = 0;

    // Example scoring: +10 for 2 aligned chips, +50 for 3 aligned, etc.
    for (int i=0; i<SIZE; i++) {
        for (int j=0; j<SIZE; j++) {
            if (board[i][j]==CHIP[player]) score++;
            else if (board[i][j]!=CHIP[player] && board[i][j]!=' ') score--;
        }
    }
    return score;
}

int minimax(int depth, int isMaximizing, int alpha, int beta) {
    int winner = game_over();
    if (winner==CHIP[1]) return 1000;
    if (winner==CHIP[0]) return -1000;
    if (depth==0 || winner!=0) return evaluate_board(); 

    if (isMaximizing) {
        int maxEval = INT_MIN;
        for (int i=0; i<SIZE; i++) {
            if (is_valid_move(i)) {
                add_coin(i, player);
                int eval = minimax(depth - 1, 0, alpha, beta);
                undo_move(i);
                maxEval = eval>maxEval ? eval : maxEval;
                alpha = alpha>maxEval ? alpha : maxEval;
                if (beta<=alpha) break;
            }
        }
        return maxEval;
    } else {
        int minEval = INT_MAX;
        for (int i=0; i<SIZE; i++) {
            if (is_valid_move(i)) {
                add_coin(i, 0);
                int eval = minimax(depth - 1, 1, alpha, beta);
                undo_move(i);
                minEval = eval<minEval ? eval : minEval;
                beta = beta<minEval ? beta : minEval;
                if (beta<=alpha) break;
            }
        }
        return minEval;
    }
}

int find_best_move(int depth) {
    int bestMove = -1;
    int bestScore = INT_MIN;

    for (int i=0; i<SIZE; i++) {
        if (is_valid_move(i)) {
            add_coin(i, 1);
            int moveScore = minimax(depth, 0, INT_MIN, INT_MAX);
            undo_move(i);
            if (moveScore > bestScore) {
                bestScore = moveScore;
                bestMove = i;
            }
        }
    }
    return bestMove;
}

void play(void) {
    int win = 0;
    int key = 0;
    draw_board();

    while (!(win = game_over())) {
        printw("Player %d: ", player);
        refresh();

        if ((PLAYERS-player)<=NBOT) key = find_best_move(4); // Depth = 4 for performance
        else {
            if ((key = get_col()) == -1) {
                win = -1;
                draw_board();
                break;
            }
        }
        player = add_coin(key, player);
        draw_board();
    }

    if (win==-1) printw("Game stopped!\n");
    else if (win==1) printw("Game finished, no winner!\n");
    else printw("%c wins!\n", win);
    printw("(Press any key to exit!)\n");
    refresh();
}

int main(void) {
    INIT_SCREEN();
    init_board();
    play();
    getch();
    DONE_SCREEN();
}