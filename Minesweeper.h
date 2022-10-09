#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

// estrutura responsável por armazenar informações acerca de cada espaço da matriz.
typedef struct Cell {
    int mineCount;
    char ch;
    bool uncovered;
    bool bomb;
} Cell;

// definindo uma estrutura para as dimensões do jogo.
typedef struct Board {
    int height, width;
    Cell** cells;
} Board;

void generateBoard(Board*);
void printBoard(Board*);
void randomizeBombs(Board*, int);
void adjBombs(Board*);
void dig(Board*, int, int);
void revealBombs(Board*, int, int);
void checkVictory(Board*, int);
void startGame();
void selectDifficulty(int*, int*, int*);