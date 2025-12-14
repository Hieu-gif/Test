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
int delay = 500; 

void gotoxy(int x, int y) {
    COORD c = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
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
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (currentBlock[i][j] != ' ') {
                int by = y + i;
                int bx = x + j;
                if (by >= 0 && by < H && bx >= 0 && bx < W) {
                    if (board[by][bx] == currentBlock[i][j]) {
                        board[by][bx] = ' ';
                    }
                }
            }
        }
    }
}

void block2Board() {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (currentBlock[i][j] != ' ') {
                int by = y + i;
                int bx = x + j;
                if (by >= 0 && by < H && bx >= 0 && bx < W)
                    board[by][bx] = currentBlock[i][j];
            }
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
    bool isO = (currentBlock[1][1] == 'O' && currentBlock[1][2] == 'O' &&
        currentBlock[2][1] == 'O' && currentBlock[2][2] == 'O');
    if (isO) return;

    char temp[4][4];

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            temp[j][3 - i] = currentBlock[i][j];

    int kicks[] = { 0, -1, 1, -2, 2 };

    for (int k = 0; k < (int)(sizeof(kicks) / sizeof(kicks[0])); k++) {
        int dx = kicks[k];
        bool ok = true;

        for (int i = 0; i < 4 && ok; i++) {
            for (int j = 0; j < 4; j++) {
                if (temp[i][j] != ' ') {
                    int tx = x + j + dx;
                    int ty = y + i;
                    if (!inPlayable(tx, ty)) { ok = false; break; }
                    if (ty >= 0 && board[ty][tx] != ' ') { ok = false; break; }
                }
            }
        }

        if (ok) {
            for (int i = 0; i < 4; i++)
                for (int j = 0; j < 4; j++)
                    currentBlock[i][j] = temp[i][j];
            x += dx;
            return;
        }
    }
    return;
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
    for (int i = 0; i < H; i++, cout << endl) {
        for (int j = 0; j < W; j++) {
            if (board[i][j] == '#')
                cout << "##";
            else if (board[i][j] != ' ')
                cout << "[]";
            else
                cout << "  ";
        }
    }
    cout << "\nControls: A (Left), D (Right), S (Down), W (Rotate), Q (Quit)\n";
}

void removeLine() {
    int i = H - 2;
    while (i >= 0) {
        bool full = true;
        for (int j = 1; j <= W - 2; j++) {
            if (board[i][j] == ' ') { full = false; break; }
        }

        if (full) {
            score += 100;
            if (delay > 30)
                delay -= 10;
            for (int r = i; r > 0; r--) {
                for (int c = 1; c <= W - 2; c++) {
                    board[r][c] = board[r - 1][c];
                }
            }
            for (int c = 1; c <= W - 2; c++)
                board[0][c] = ' ';
        }
        else {
            i--;
        }
    }
}

int main()
{
    srand((unsigned)time(0));
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
    system("cls");
    initBoard();
    b = rand() % 7;
    loadBlock(b);
    x = W / 2 - 2;
    y = 0;

    block2Board();
    draw();

    while (1) {
        boardDelBlock();

        if (_kbhit()) {
            char c = _getch();
            if ((c == 'a' || c == 'A') && canMove(-1, 0)) x--;
            if ((c == 'd' || c == 'D') && canMove(1, 0)) x++;
            if ((c == 's' || c == 'S') && canMove(0, 1)) y++;
            if ((c == 'w' || c == 'W')) rotateBlock();
            if ((c == 'q' || c == 'Q')) break;
        }

        if (canMove(0, 1)) {
            y++;
        }
        else {
            block2Board();
            removeLine();

            b = rand() % 7;
            loadBlock(b);
            x = W / 2 - 2;
            y = 0;

            if (!canMove(0, 0)) {
                block2Board();
                draw();
                gotoxy(0, H + 3);
                cout << "\nGame Over! Final score: " << score << "\n";
                break;
            }
        }

        block2Board();
        draw();
        Sleep(delay);
    }
    return 0;
}

