#ifndef CAMPO_MINADO_H_INCLUDED
#define CAMPO_MINADO_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define BOMBA 208
#define QTD_BOMBA 30
#define META 10

unsigned char mapa[7][7] = {
    {'#', '#', '#', '#', '#', '#', '#'},
    {'#', '#', '#', '#', '#', '#', '#'},
    {'#', '#', '#', '#', '#', '#', '#'},
    {'#', '#', '#', '#', '#', '#', '#'},
    {'#', '#', '#', '#', '#', '#', '#'},
    {'#', '#', '#', '#', '#', '#', '#'},
    {'#', '#', '#', '#', '#', '#', '#'}
};


unsigned char mapa_bombas[7][7] =
{
    {' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' '}
};

int mapa_revelado[7][7] = {0};

int jogadas = 0;
int vida = 3;
int liberado = 0;
char mensagem[20];

void contar_pontos()
{
    liberado = 0;

    for(int i = 0; i < 7; i++)
    {
        for(int j = 0; j < 7; j++)
        {
            liberado = liberado + mapa_revelado[i][j];
        }
    }
}

void colocar_bombas()
{
    srand(time(NULL));

    int i = 0;
    while(i < QTD_BOMBA)
    {
        int x = rand() %7;
        int y = rand() %7;

        if(mapa_bombas[x][y] != BOMBA)
        {
            mapa_bombas[x][y] = BOMBA;
            i++;
        }
    }
}

void colocar_numero(int x, int y)
{
    int contador = 0;

    for(int i = x-1; i <= x+1; i++)
    {
        for(int j = y-1; j <= y+1; j++)
        {
            if(i == x && j == y)
            {
                continue;
            }

            if((i >= 0 && i <= 6) && (j >= 0 && j <= 6))
            {
                if(mapa_bombas[i][j] == BOMBA)
                {
                    contador++;
                }
            }
        }
    }

    if(contador == 0)
    {
        mapa[x][y] = ' ';
    }
    else
    {
        mapa[x][y] = '0' + contador;
    }
}

void revelar_campo(int x, int y)
{
    if((jogadas > 1 && mapa_bombas[x][y] == BOMBA) || mapa_revelado[x][y] == 1)
    {
        return;
    }

    mapa_revelado[x][y] = 1;

    colocar_numero(x, y);

    for(int i = x-1; i <= x+1; i++)
    {
        for(int j = y-1; j <= y+1; j++)
        {
            if(i == x && j == y)
            {
                continue;
            }

            if((i == x-1 && j == y-1) || (i == x+1 && j == y-1) || (i == x-1 && j == y+1) || (i == x+1 && j == y+1))
            {
                continue;
            }

            if((i >= 0 && i <= 6) && (j >= 0 && j <= 6))
            {
                if(mapa_bombas[i][j] != BOMBA)
                {
                    revelar_campo(i, j);
                }
            }
        }
    }
}

int validar(int x, int y)
{
    if(x > 6 || y > 6 || x < 0 || y < 0)
    {
        strcpy(mensagem, "FORA DO CAMPO!");
        return 0;
    }

    if(jogadas == 1)
    {
        mapa_bombas[x][y] = ' ';
        return 1;
    }

    if(mapa_bombas[x][y] == BOMBA)
    {
        strcpy(mensagem, "BOMBA!");
        return 0;
    }

    if(mapa_revelado[x][y] == 1)
    {
        strcpy(mensagem, "JA FOI REVELADO");
        return 1;
    }

    return 1;
}

void jogar()
{
    int x, y;
    printf("Insira as coordenadas\n");
    printf("X: "), scanf("%i", &x);
    printf("Y: "), scanf("%i", &y);

    x = x - 1;
    y = y - 1;

    jogadas++;

    if(validar(x, y))
    {
        revelar_campo(x, y);
        strcpy(mensagem,"");
        contar_pontos();
    }
    else
    {
        vida--;
        mapa[x][y] = mapa_bombas[x][y];
    }
}

void colocar_bandeira()
{
    int x, y;
    printf("Insira as coordenadas\n");
    printf("X: "), scanf("%i", &x);
    printf("Y: "), scanf("%i", &y);

    x = x - 1;
    y = y - 1;

    if(mapa[x][y] == '#')
    {
        mapa[x][y] = '>';
        strcpy(mensagem, "");
    }
}

void exibir()
{
    system("cls");
    printf("OBS.: Ao sair, se tiver liberado pelo menos %i quadrados, vence.\n", META);
    printf("\n%s\n", mensagem);
    printf("------------------------------------------------------------------------------\n");
    printf(" VIDA: %i/3                      CAMPO MINADO                   LIBERADO: %i/%i\n", vida, liberado,49-QTD_BOMBA);
    printf("------------------------------------------------------------------------------\n");

    printf("                      Y +---+---+---+---+---+---+---+\n");
    for(int i = 6; i >= 0; i--)  // Começa da linha 6 para 0
    {
        printf("                      %i |", i+1);
        for(int j = 0; j <= 6; j++)  // Loop de colunas da 0 até a 6
        {
            printf(" %c |", mapa[j][i]);
        }
        printf("\n                        +---+---+---+---+---+---+---+");
        printf("\n");
    }
    printf("                      0   1   2   3   4   5   6   7 X\n");
    printf("------------------------------------------------------------------------------\n");
}

int PlayCampo_Minado()
{
    colocar_bombas();
    int op = 0;

    do
    {
        exibir();

        printf("CAVAR            [1]\n");
        printf("COLOCAR BANDEIRA [2]\n");
        printf("SAIR             [3]\n");

        printf("Opcao: ");
        scanf("%i", &op);

        switch(op)
        {
            case 1:
                jogar();
                break;
            case 2:
                colocar_bandeira();
                break;

            case 3:
                break;
            default:
                strcpy(mensagem, "OPCAO INVALIDA");
        }

    }
    while(op != 3 && vida > 0 && liberado < 49-QTD_BOMBA);

    if(liberado >= META)
    {
        strcpy(mensagem, "GANHOU!");
        exibir();
        return 1;
    }
    else
    {
        strcpy(mensagem, "PERDEU!");
        exibir();
        return 0;
    }

    return 0;
}


#endif // CAMPO_MINADO_H_INCLUDED
