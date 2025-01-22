#include <stdlib.h>
#include <time.h>
#include <ncurses.h>

#define SIZE 16
#define PLAYERS 2

char board[SIZE][SIZE];
char CHIP[PLAYERS] = "XO";
int player = 0;

void INIT_SCREEN() {
	initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();
}

void DONE_SCREEN() {
    endwin();
    exit(0);
}

void init_board() {
    for (int i=0; i<SIZE; i++) {
        for (int j=0; j<SIZE; j++) {
            board[i][j] = ' ';
        }
    }
}

void draw_board() {
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
}

int get_col() {
    int key;
    while (key != KEY_BACKSPACE && key != 127) {
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
            board[i][col] = CHIP[player];
            return (player+1) % PLAYERS;
        }
    }
    return player;
}

int game_over() {
    int completed = 1;
    for (int i=0; i<SIZE; i++) {
        for(int j=0; j<SIZE; j++) {
            if (board[i][j]==' ') {
                completed=0;
                continue;
            } if (j <= SIZE-4
                && board[i][j]==board[i][j+1]
                && board[i][j]==board[i][j+2]
                && board[i][j]==board[i][j+3]) {
                return board[i][j];
            } if (i <= SIZE-4
                && board[i][j]==board[i+1][j]
                && board[i][j]==board[i+2][j]
                && board[i][j]==board[i+3][j]) {
                return board[i][j];
            } if (i <= SIZE-4 && j <= SIZE-4
                && board[i][j]==board[i+1][j+1]
                && board[i][j]==board[i+2][j+2]
                && board[i][j]==board[i+3][j+3]) {
                return board[i][j];
            } if (i <= SIZE-4 && j >= 4
                && board[i][j]==board[i+1][j-1]
                && board[i][j]==board[i+2][j-2]
                && board[i][j]==board[i+3][j-3]) {
                return board[i][j];
            }
        }
    }
    return completed;
}

void play() {
    int player = 0;
    int win = 0;
    int key = 0;
    draw_board();

    while (!(win = game_over())) {
        printw("player %i : ", player);

        if ((key=get_col()) == -1) {
            win = -1;
            draw_board();
            break;
        }
        player = add_coin(key, player);
        draw_board();
    }
    if (win==-1) {
        printw("Game stopped !\n");
    } else if (win==1) {
        printw("Game finished, no winner !\n");
    } else {
        printw("%c win\n", win);
    }
    printw("(Press to end !)\n");
}

int main() {
    INIT_SCREEN();
    init_board();
    play();
    getch();
    DONE_SCREEN();
}