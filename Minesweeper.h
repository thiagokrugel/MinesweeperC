#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

// construir a struct do campo
typedef struct Campo{
    int number_of_mines;
    char ch;
    bool uncovered;
    bool bomb;
}Campo;

// criando as dimensões do campo
typedef struct Board{
    int altura, largura;
    Campo **cells;
}Board;

void criaCampo(Board*);
void printarCampo(Board*);
void aleatorizaBomba(Board*, int);
void bombasAdjacentes(Board*);
void escavar(Board*, int, int);
void reveal_automatically(Board*, int, int);
void checarVitoria(Board*, int);
void play_game();
