#include "Minesweeper.h"

// Variáveis para definir condições de finalização do jogo.
bool lose = false;
bool win = false;

// Responsável pela inicialização do campo.
void generateBoard(Board* board) {
    int i, j;
    board->cells = (Cell**) malloc((board->height+2) * sizeof(Cell*));

    for(i = 0; i <= board->height+1; ++i)
        board->cells[i] = (Cell*) malloc((board->width+2) * sizeof(Cell));
  
    for(i = 0; i <= board->height+1; ++i){
        for(j = 0; j <= board->width+1; ++j){
            if (i==0 || i==board->height+1 || j==0 || j == board->width+1) {
                board->cells[i][j].bomb = false;
                board->cells[i][j].uncovered = true;
            }
            else {
                board->cells[i][j].ch = '-';              // Valor inicial.
                board->cells[i][j].mineCount = 0;         // inicializar as bombas adjacentes como zero.
                board->cells[i][j].bomb = false;
                board->cells[i][j].uncovered = false;
            }
        }
    }

     return;
}

void printBoard(Board* board) {
    system("clear");
    printf("  ");
    int i, j;

    for(i = 1; i <= board->width; ++i)
        printf("%d ", i);
    printf("\n");

    // Printar coordenadas
    for(i = 1; i <= board->height; ++i){
        for(j = 0; j<= board->width; ++j){
            if (j == 0) 
                printf("%d ", i);
            else 
                printf("%c ", board->cells[i][j].ch);
        }
        printf("\n");
    }
   return;
}

// Definir bombas em locais aleatórios dentro do campo.
void randomizeBombs(Board* board, int mines) {
    int random_row, random_col, numMines = 0;
    while(numMines < mines) {
        random_row = rand()%board->height+1;
        random_col = rand()%board->width+1;

        if(!board->cells[random_row][random_col].bomb && (random_row != 0 && random_col != 0)) {
            board->cells[random_row][random_col].bomb = true;
            numMines++;
        }
    }
     return;
}

// Responsável por contar a quantidade de bombas adjacentes a cada espaço no campo.
void adjBombs(Board* board){
	int i, j, m, n;
	for(i = 1; i <= board->height; ++i) {
        for(j = 1; j <= board->width; ++j) {
            if (!board->cells[i][j].bomb) {
                for (m = i-1, n = j-1; m <= i+1 && n <= j+1; ++m, ++n)
                //for(m = i-1; m <= i+1; ++m)
                //for(n = j-1; n <= j+1; ++n)
                    if(board->cells[m][n].bomb) // contando as bombas em volta
                    board->cells[i][j].mineCount++;
            }
        }
    }

    return;
}

// Responsável por lidar com a função de escavação do jogo.
void dig(Board* board, int rows, int columns) {
    if(board->cells[rows][columns].bomb) {
        lose = true;
        int i, j;
        for(i = 1; i <= board->height; ++i)
            for(j = 1; j <= board->width; ++j)
                if(board->cells[i][j].bomb)
                    board->cells[i][j].ch = '*';
                else
                    board->cells[i][j].ch = board->cells[i][j].mineCount + '0'; // Troca a casa escavada pelo número de bombas adjacentes
        printBoard(board);
        printf("\nFIM DE JOGO\n");

        return;
    }

    board->cells[rows][columns].ch = board->cells[rows][columns].mineCount + '0';
    if(board->cells[rows][columns].mineCount == 0)
        revealBombs(board, rows, columns); // se o número de bombas adjacentes for 0, revela os adjacentes
    else
        board->cells[rows][columns].uncovered = true;

    return;
}

// Utilizada para revelar as bombas adjacentes, de maneira recursiva.
void revealBombs(Board* board, int rows, int columns) {
    int i, j;
    if(!board->cells[rows][columns].uncovered) {
        board->cells[rows][columns].uncovered = true;
        for(i = rows-1; i <= rows+1; ++i)
            for(j = columns-1; j <= columns+1; ++j)
                if(!board->cells[i][j].uncovered)
                    dig(board, i, j);
    }

    return;
}

void checkVictory(Board* board, int mines) {
    int i, j, counter = 0;
    for(i = 1; i <= board->height; ++i)
        for(j = 1; j <= board->width; ++j)
            if(!board->cells[i][j].bomb && board->cells[i][j].ch != '-' && board->cells[i][j].ch != '!')
                counter++;

    if(counter == (board->height * board->width) - mines) {
        win = true;
        for(i = 1; i <= board->height; ++i)
            for(j = 1; j <= board->width; ++j)
                if(board->cells[i][j].bomb)
                    board->cells[i][j].ch = '*'; // mostrar as bombas
                else
                    board->cells[i][j].ch = board->cells[i][j].mineCount + '0'; // mostrar a quantidade de bombas adjacentes
        printBoard(board);
        printf("\nVOCÊ VENCEU\n"); 
    }

    return;
}

void startGame() {
    srand(time(NULL));
    printf("\t\t\t\t ***Bem-vindo ao Campo-Minado!*** \n");
    int x, y, row, column, mines;
    char op;

    selectDifficulty(&x, &y, &mines);

    // Alocando memória para o campo.
    Board* board = (Board*) malloc(sizeof(Board)); 
    board->width = x;
    board->height = y;

    generateBoard(board);
    randomizeBombs(board, mines);
    adjBombs(board);

    do {
        printBoard(board);

        printf("Você quer escavar ou marcar com bandeira? 'e' ou 'b' ");
        scanf("%c", &op);
        fflush(stdin);

        printf("Insira a linha desejada: ");
        scanf("%d", &row);
        printf("Insira a coluna desejada: ");
        scanf("%d", &column);

        if(op == 'e') // e - escavar
            dig(board, row, column);
        if(op == 'b') // b - bandeira
            board->cells[row][column].ch = '!';

        if (!lose) 
            checkVictory(board, mines);
    } while(!lose && !win);

    for(int i = 0; i <= board->height+1; i++)
        free(board->cells[i]);          // limpar cada célula do campo
    free(board->cells);                 // limpar o array de ponteiros

    return;
}

void selectDifficulty(int* x, int* y, int* mines) {
    int choice;
    printf("1. Fácil\n2. Médio\n3. Difícil\n4. Personalizado");
    scanf("%d", choice);

    switch (choice) {
    case 1:
        *x = 8;  *y = 8;  *mines = 10;
        break;
    case 2:
        *x = 16; *y = 16; *mines = 40;
        break;
    case 3:
        *x = 30; *y = 16; *mines = 99;
        break;
    case 4:
        printf("Insira o tamanho do campo (separe os números com um espaço):\n");
        scanf("%d%d", &x, &y);

        printf("Insira o número de bombas que estarão no campo: ");
        scanf("%d", &mines);
        break;
    default:
        break;
    }

    return;
}