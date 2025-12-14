#include <iostream>
#include <conio.h>
#include <windows.h>
#include <ctime>
#include <cstdlib>   
using namespace std;

#define H 20
#define W 20

char board[H][W] = {};

char currentBlock[4][4];
char blocks[][4][4] = {
    {{' ','I',' ',' '},
     {' ','I',' ',' '},
     {' ','I',' ',' '},
     {' ','I',' ',' '}},
    {{' ',' ',' ',' '},
     {' ','O','O',' '},
     {' ','O','O',' '},
     {' ',' ',' ',' '}},
    {{' ',' ',' ',' '},
     {' ','T',' ',' '},
     {'T','T','T',' '},
     {' ',' ',' ',' '}},
    {{' ',' ',' ',' '},
     {' ','S','S',' '},
     {'S','S',' ',' '},
     {' ',' ',' ',' '}},
    {{' ',' ',' ',' '},
     {'Z','Z',' ',' '},
     {' ','Z','Z',' '},
     {' ',' ',' ',' '}},
    {{' ',' ',' ',' '},
     {'J',' ',' ',' '},
     {'J','J','J',' '},
     {' ',' ',' ',' '}},
    {{' ',' ',' ',' '},
     {' ',' ','L',' '},
     {'L','L','L',' '},
     {' ',' ',' ',' '}}
};

int x = 4, y = 0, b = 0;
int score = 0;
int dropDelay = 200;    
void hideCursor() {
    CONSOLE_CURSOR_INFO ci;
    ci.dwSize = 1;
    ci.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &ci);
}     
void gotoxy(int x, int y) {
    COORD c = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}
int menu() {
    while (true) {
        system("cls");
        cout << "====== TETRIS ======\n";
        cout << "1. Start Game\n";
        cout << "2. Speed: Slow\n";
        cout << "3. Speed: Normal\n";
        cout << "4. Speed: Fast\n";
        cout << "5. Exit\n";
        cout << "====================\n";
        cout << "Press 1-5";

        char c = _getch();
        if (c >= '1' && c <= '5') return c - '0';
    }
}
void loadBlock(int id) {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            currentBlock[i][j] = blocks[id][i][j];
}

inline bool inPlayable(int tx, int ty) {
    return (tx >= 1 && tx <= W - 2 && ty >= 0 && ty <= H - 2);
}

void boardDelBlock() {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (currentBlock[i][j] != ' ')
                board[y + i][x + j] = ' ';
}

void block2Board() {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (currentBlock[i][j] != ' ')
                board[y + i][x + j] = currentBlock[i][j];
}

bool canMove(int dx, int dy) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (currentBlock[i][j] != ' ') {
                int tx = x + j + dx;
                int ty = y + i + dy;

                if (!inPlayable(tx, ty)) return false;

                if (ty >= 0 && board[ty][tx] != ' ') return false;
            }
        }
    }
    return true;
}

void rotateBlock() {
    char t[4][4];
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            t[j][3 - i] = currentBlock[i][j];

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            currentBlock[i][j] = t[i][j];
}

void initBoard() {
    for (int i = 0; i < H; i++)
        for (int j = 0; j < W; j++)
            if ((i == H - 1) || (j == 0) || (j == W - 1)) board[i][j] = '#';
            else board[i][j] = ' ';
}

void draw() {
    gotoxy(0, 0);
    cout << "Score: " << score << "\n\n";
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++)
            cout << (board[i][j] == '#' ? "##" : board[i][j] != ' ' ? "[]" : "  ");
        cout << "\n";
    }
    cout << "\nA/D: Move  W: Rotate  S: Down  Q: Quit";
}

void removeLine() {
    for (int i = H - 2; i >= 0; i--) {
        bool full = true;
        for (int j = 1; j <= W - 2; j++)
            if (board[i][j] == ' ') full = false;

        if (full) {
            score += 100;
            for (int r = i; r > 0; r--)
                for (int c = 1; c <= W - 2; c++)
                    board[r][c] = board[r - 1][c];

            for (int c = 1; c <= W - 2; c++)
                board[0][c] = ' ';
            i++;
        }
    }
}

int main() {
    srand(time(0));
    hideCursor();

    while (true) {
        int c = menu();
        if (c == 1) break;
        if (c == 2) dropDelay = 400;
        if (c == 3) dropDelay = 200;
        if (c == 4) dropDelay = 100;
        if (c == 5) return 0;
    }

    system("cls");
    initBoard();

    b = rand() % 7;
    loadBlock(b);
    x = W / 2 - 2;
    y = 0;

    while (true) {
        boardDelBlock();

        if (_kbhit()) {
            char k = _getch();
            if ((k == 'a' || k == 'A') && canMove(-1, 0)) x--;
            if ((k == 'd' || k == 'D') && canMove(1, 0)) x++;
            if ((k == 's' || k == 'S') && canMove(0, 1)) y++;
            if (k == 'w' || k == 'W') rotateBlock();
            if (k == 'q' || k == 'Q') break;
        }

        if (canMove(0, 1)) y++;
        else {
            block2Board();
            removeLine();

            b = rand() % 7;
            loadBlock(b);
            x = W / 2 - 2;
            y = 0;

            if (!canMove(0, 0)) {
                draw();
                cout << "\nGAME OVER!";
                _getch();
                break;
            }
        }

        block2Board();
        draw();
        Sleep(dropDelay);
    }
    return 0;
}

