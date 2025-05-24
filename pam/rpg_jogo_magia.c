#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <locale.h>

#ifdef _WIN32
    #include <windows.h>
    #define CLEAR "cls"
    void esperar(int ms) { Sleep(ms); }
#else
    #define CLEAR "clear"
    void esperar(int ms) { usleep(ms * 1000); }
#endif

#define NUM_ESQUELETOS 5

typedef struct {
    int vida;
    int poder;
} inimigo_stats;

typedef struct {
    int vida;
    int poder;
    int mana;
    int magia;
} jogador;

jogador p1;
inimigo_stats esqueletos[NUM_ESQUELETOS];

void mostrarCena(const char* efeito) {
    const char* mago[] = {
        "    /\\   \"V\"    ",
        "   /__\\   I     ",
        "  //..\\\\  I     ",
        "  \\].`[/  I     ",
        "  /l\\/j\\  (]    ",
        " /. ~~ ,\\/I     ",
        " \\\\L__j^\\/I     ",
        "  \\/--v}  I     ",
        "  |    |  I     ",
        "  |    |  I     ",
        "  |    l  I     ",
        "_/j  L l\\_      ",
        "                "
    };

    const char* esqueleto[] = {
        "      .-.        ",
        "     (o.o)       ",
        "      |=|        ",
        "     __|__       ",
        "   //.=|=.\\\\     ",
        "  // .=|=. \\\\    ",
        "  \\\\ .=|=. //    ",
        "   \\\\(_=_)//     ",
        "    (:| |:)       ",
        "     || ||        ",
        "     () ()        ",
        "     || ||        ",
        "     || ||        ",
        "   ==' '==        "
    };

    for (int i = 0; i < 11; i++) {
        printf("%s", mago[i]);
        if (efeito && i == 5)
            printf("%25s", efeito);
        else
            printf("%25s", " ");
        printf("%s\n", esqueleto[i]);
    }
}

void animAtaqueFisico() {
    const char* frames[] = { "->", "=>", "==>", "===>", "====>", "=====>", "======>" };
    for (int i = 0; i < 7; i++) {
        mostrarCena(frames[i]);
        esperar(80);
    }
}

void animBolaDeFogo() {
    const char* frames[] = { "( )", "(*)", "(#)", "(*)", "(#)" };
    for (int i = 0; i < 5; i++) {
        mostrarCena(frames[i]);
        esperar(100);
    }
}

void mostrarStatus() {
    printf("\nVoc�: Vida = %d  Mana = %d\n", p1.vida, p1.mana);
    for (int i = 0; i < NUM_ESQUELETOS; i++) {
        if (esqueletos[i].vida > 0)
            printf("Esqueleto %d: %d HP\n", i + 1, esqueletos[i].vida);
        else
            printf("Esqueleto %d: [MORTO]\n", i + 1);
    }
}

void mostrarMenu() {
    printf("\n+-----------------------------+\n");
    printf("|        ESCOLHA A��O        |\n");
    printf("+-----------------------------+\n");
    printf(" (1) Ataque\n");
    printf(" (2) Bola de fogo\n\n");
    printf("Op��o: ");
}

int todosMortos() {
    for (int i = 0; i < NUM_ESQUELETOS; i++) {
        if (esqueletos[i].vida > 0)
            return 0;
    }
    return 1;
}

int main() {

    setlocale(LC_ALL, "Portuguese");
    int opcao, alvo;
    srand(time(NULL));

    p1.vida = 1000;
    p1.poder = 50;
    p1.mana = 100;
    p1.magia = 100;

    for (int i = 0; i < NUM_ESQUELETOS; i++) {
        esqueletos[i].vida = 100 + rand() % 100;
        esqueletos[i].poder = 25 + rand() % 15;
    }

    while (p1.vida > 0 && !todosMortos()) {
        mostrarCena(NULL);
        mostrarStatus();
        mostrarMenu();

        scanf("%d", &opcao);
        getchar();

        printf("Escolha o esqueleto para atacar (1-%d): ", NUM_ESQUELETOS);
        scanf("%d", &alvo);
        getchar();
        alvo--;

        if (alvo < 0 || alvo >= NUM_ESQUELETOS || esqueletos[alvo].vida <= 0) {
            printf("\nAlvo inv�lido ou j� destru�do!\n");
            esperar(1500);
            continue;
        }

        if (opcao == 1) {
            animAtaqueFisico();
            int dano = p1.poder + rand() % 20;
            esqueletos[alvo].vida -= dano;
            if (esqueletos[alvo].vida < 0) esqueletos[alvo].vida = 0;
            printf("\nVoc� atacou o Esqueleto %d causando %d de dano!\n", alvo + 1, dano);
        }
        else if (opcao == 2) {
            if (p1.mana >= 40) {
                animBolaDeFogo();
                int dano = p1.magia * (rand() % 3 + 1);
                esqueletos[alvo].vida -= dano;
                p1.mana -= 40;
                if (esqueletos[alvo].vida < 0) esqueletos[alvo].vida = 0;
                printf("\nVoc� lan�ou bola de fogo no Esqueleto %d causando %d de dano!\n", alvo + 1, dano);
            } else {
                printf("\nMana insuficiente!\n");
                esperar(1500);
                continue;
            }
        } else {
            printf("\nOp��o inv�lida!\n");
            esperar(1500);
            continue;
        }

        esperar(1500);

        for (int i = 0; i < NUM_ESQUELETOS; i++) {
            if (esqueletos[i].vida > 0) {
                int dano = esqueletos[i].poder * (rand() % 2 + 1);
                p1.vida -= dano;
                printf("Esqueleto %d atacou voc� e causou %d de dano!\n", i + 1, dano);
            }
        }

        if (p1.vida < 0) p1.vida = 0;

        p1.mana += 15;
        if (p1.mana > 100) p1.mana = 100;

        printf("\nPressione ENTER para continuar...");
        getchar();
    }

    mostrarCena(NULL);

    if (p1.vida > 0)
        printf("\nVoc� venceu! Todos os esqueletos viraram p�.\n");
    else
        printf("\nVoc� caiu... os esqueletos venceram.\n");

    return 0;
}
