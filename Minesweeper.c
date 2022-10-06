#include "Minesweeper.h"

bool lose = false;
bool win = false;

void criaCampo(Board *ptr)
{
    int i, j;
    ptr->cells = (Campo**)malloc((ptr->altura+2)*sizeof(Campo*));

    for(i = 0; i <= ptr->altura+1; ++i)
        ptr->cells[i] = (Campo*)malloc((ptr->largura+2)*sizeof(Campo));
  
    for(i = 0; i <= ptr->altura+1; ++i){
        for(j = 0; j <= ptr->largura+1; ++j){
            if (i==0 || i==ptr->altura+1 || j==0 || j == ptr->largura+1)
            {
                ptr->cells[i][j].bomb = false;
                ptr->cells[i][j].uncovered = true;
            }

            else
            {
                ptr->cells[i][j].ch = '-';  // criando o campo com '-'
                ptr->cells[i][j].number_of_mines = 0; // inicializar as bombas
                ptr->cells[i][j].bomb = false;
                ptr->cells[i][j].uncovered = false;
            }
        }
    }

     return;
}

void printarCampo(Board *ptr)
{
    system("clear");

    printf("  ");
    int i, j;
    for(i = 1; i <= ptr->largura; ++i) // loop in order print each row 
        printf("%d ", i); // numbers which shows rows
    printf("\n");
    // printar coordenadas
    for(i = 1; i <= ptr->altura; ++i){
        for(j = 0; j<= ptr->largura; ++j){
          if(j == 0) printf("%d ", i);
          else printf("%c ", ptr->cells[i][j].ch);
        }
        printf("\n");
    }
   return;
}

void aleatorizaBomba(Board *ptr, int mines)
{
    int random_row, random_col, num_of_mine = 0;
    while(num_of_mine < mines)
    {
        random_row = rand()%ptr->altura+1; // generating random number
        random_col = rand()%ptr->largura+1;  // generating random number

        if(ptr->cells[random_row][random_col].bomb == false && (random_row != 0 && random_col != 0)) // checking for numbers which were generated before or not
        {
            ptr->cells[random_row][random_col].bomb = true; // if not, make a new bomb
            num_of_mine++;
        }
    }
     return;
}

void bombasAdjacentes(Board *ptr){
	int i, j, m, n;
	for(i = 1; i <= ptr->altura; ++i){
        for(j = 1; j <= ptr->largura; ++j){
            if (ptr->cells[i][j].bomb == false){
                for(m = i-1; m <= i+1; ++m)
                for(n = j-1; n <= j+1; ++n)
                    if(ptr->cells[m][n].bomb == true) // contando as bombas em volta
                    ptr->cells[i][j].number_of_mines++;
            }
        }
    }
    return;
}

void escavar(Board *ptr, int a, int b)
{
    if(ptr->cells[a][b].bomb == true)
    {
        lose = true; // terminate the game "in the play game function (while loop)"
        int i, j;
        /* nested loop to uncover cells */
        for(i = 1; i <= ptr->altura; ++i)
            for(j = 1; j <= ptr->largura; ++j)
               if(ptr->cells[i][j].bomb == true) /* if there is a bomb */
                  ptr->cells[i][j].ch = '*'; // showing all bombs
               else
                  ptr->cells[i][j].ch = ptr->cells[i][j].number_of_mines + '0'; // troca a casa escavada pelo número de bombas adjacentes
        printarCampo(ptr);
        printf("\nFIM DE JOGO\n");

         return;
    }

   ptr->cells[a][b].ch = ptr->cells[a][b].number_of_mines + '0';
    if(ptr->cells[a][b].number_of_mines == 0)
       reveal_automatically(ptr,a,b); // se o número de bombas adjacentes for 0, revela os adjacentes
    else
       ptr->cells[a][b].uncovered = true;

    return;
}

void reveal_automatically(Board *ptr, int a, int b)
{
    int i, j;

    if(ptr->cells[a][b].uncovered == false)
    {
        ptr->cells[a][b].uncovered = true;
        /* nested loop to reveal automatically */
        for(i = a-1; i <= a+1; ++i)
            for(j = b-1; j <= b+1; ++j)
               if(ptr->cells[i][j].uncovered == false)
                 escavar(ptr,i,j); //calling function in order to uncover cells
    }
    return;
}

void checarVitoria(Board *ptr, int mines)
{
    int i, j, counter = 0;


    for(i = 1; i <= ptr->altura; ++i)
        for(j = 1; j <= ptr->largura; ++j)
           if(ptr->cells[i][j].bomb==false && ptr->cells[i][j].ch != '-' && ptr->cells[i][j].ch != '!')
             counter++;

    if (counter == (ptr->altura*ptr->largura) - mines)
    {
        win = true;
        for(i = 1; i <= ptr->altura; ++i)
            for(j = 1; j <= ptr->largura; ++j)
               if(ptr->cells[i][j].bomb == true)
                  ptr->cells[i][j].ch = '*'; // mostrar as bombas
               else
                  ptr->cells[i][j].ch = ptr->cells[i][j].number_of_mines + '0'; // mostrar as adjacentes

        printarCampo(ptr);
        printf("\nVOCÊ VENCEU\n");
          return;
    }
}


void play_game()
{

    srand(time(NULL)); // set seed for rand() in the "bombplacing_randomly" function

    printf("\t\t\t\t ***WELCOME TO MINEWSWEEPER GAME*** \n");
    int x, y, row, column, mines;
    char op;

    printf("Insira o tamanho do campo (separe os números com um espaço):\n");
    scanf("%d%d", &x, &y);

    printf("Insira o número de bombas que estarão no campo: ");
    scanf("%d", &mines);

    Board *ptr=(Board*)malloc(sizeof(Board)); // alocando memória para o campo
    ptr->altura = x;
    ptr->largura = y;

    criaCampo(ptr);
    aleatorizaBomba(ptr,mines);
    bombasAdjacentes(ptr);

    do
    {
        printarCampo(ptr);

        printf("Você quer escavar ou marcar com bandeira? 'e' ou 'b' ");
        scanf(" %c", &op);
        fflush(stdin);

        printf("Insira a linha desejada: ");
        scanf("%d", &row);
        printf("Insira a coluna desejada: ");
        scanf("%d", &column);

        if(op == 'e') // e - escavar
            escavar(ptr, row, column);
        if(op == 'b') // b - bandeira
            ptr->cells[row][column].ch = '!';
        if (!lose) check_for_win(ptr, mines);
    } while(!lose && !win);

    for(int i = 0; i <= ptr->altura + 1; i++)
      free(ptr->cells[i]); // limpar cada célula do campo
    free(ptr->cells); // limpar o array de ponteiros
    return;
}
