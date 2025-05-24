#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>
#include <time.h>

char words[10][20] = { "KERNEL", "FIRWARE", "SIGKILL", "BOOTLOADER", "SYSTEMD", "MODO REAL", "ARCH LINUX", "MAQUINA HOSPEDEIRA", "MAQUINA VIRTUAL", "USUARIO ROOT" };

char chosenWord[20];
char wordProgress[20];
char pastLetters[6]; // 6 Tentativas
int pasti = 0;

char input;
int lives = 6;

void printHangman()
{
    switch(lives)
    {
    case 6:
        printf("  +---+\n  |   |\n      |\n      |\n      |\n      |\n=========\n\n");
        break;
    case 5:
        printf("  +---+\n  |   |\n  O   |\n      |\n      |\n      |\n=========\n\n");
        break;
    case 4:
        printf("  +---+\n  |   |\n  O   |\n  |   |\n      |\n      |\n=========\n\n");
        break;
    case 3:
        printf("  +---+\n  |   |\n  O   |\n /|   |\n      |\n      |\n=========\n\n");
        break;
    case 2:
        printf("  +---+\n  |   |\n  O   |\n /|\\  |\n      |\n      |\n=========\n\n");
        break;
    case 1:
        printf("  +---+\n  |   |\n  O   |\n /|\\  |\n /    |\n      |\n=========\n\n");
        break;
    case 0:
        printf("  +---+\n  |   |\n  O   |\n /|\\  |\n / \\  |\n      |\n=========\n\n");
        break;
    }
}

void checkLetter()
{
    int foundCount = 0;
    for (int i = 0; i < strlen(chosenWord); i++)
    {
        if (chosenWord[i] == input)
        {
            wordProgress[i] = input;
            foundCount++;
        }
    }

    if (foundCount == 0)
    {
        if (strlen(pastLetters) == 0)
        {
            pastLetters[pasti] = input;
            pasti++;
        }
        else
        {
            for (int i = 0; i < strlen(pastLetters); i++)
            {
                if (pastLetters[i] == input)
                {
                    break;
                }
                else if (i + 1 == strlen(pastLetters))
                {
                    pastLetters[pasti] = input;
                    pasti++;
                }
            }
        }
        lives--;
    }
}

int PlayForca()
{
    srand(time(NULL)); // randomizar utilizando o horário atual
    int pontuacaoForca;
    strcpy(chosenWord, words[rand() % 10]); // Não esqueça de trocar o número acompanhando o rand() caso altere a matriz de palavras!
    for (int i = 0; i < strlen(chosenWord); i++)
    {
        if (chosenWord[i] == ' ')
        {
            wordProgress[i] = ' ';
        }
        else
        {
            wordProgress[i] = '_';
        }
    }

    do
    {
        system("clear");
        printf("|| JOGO DA FORCA ||\n");
        printf("Acerte a palavra para logar!\n\n");
        printHangman();
        for (int i = 0; i < strlen(chosenWord); i++)
        {
            printf("%c ", wordProgress[i]);
        }

        if (strcmp(chosenWord, wordProgress) == 0 || lives == 0)
        {
            break;
        }

        printf("\n\nLETRAS INCORRETAS: ");
        for (int i = 0; i < strlen(pastLetters); i++)
        {
            if (i + 1 == strlen(pastLetters))
            {
                printf("%c", pastLetters[i]);
            }
            else
            {
                printf("%c, ", pastLetters[i]);
            }
        }
        printf("\n\n\nDigite uma letra para testar:\n\n> ");
        scanf("%c", &input);
        fflush(stdin);
        if (isalpha(input))
        {
            input = toupper(input);
            checkLetter();
        }
    }
    while (1);

    if (lives == 0)
    {
        printf("\n\nPERDEU...\n"); // PAM_ERR_AUTH
        pontuacaoForca = 0;
    }
    else
    {
        printf("\n\nACERTOU!!!\n"); // PAM_SUCCESS
        pontuacaoForca = 1;
    }

    return pontuacaoForca;
}
