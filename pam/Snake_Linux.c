#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <termios.h>
#include <unistd.h>
#include <ctype.h>  // Necessário para toupper()

#define V 21
#define H 75
#define N 100


typedef struct
{
	int x, y; // initial position
	int movX, movY;
	char imagen;
}
snake;

typedef struct
{
	int x, y;
}
fruit;

snake snk[N]; // max size
fruit frt;
int eaten = 0;
int vic = 0;
int vezes_jogadas = 0;

// Global variable to store original terminal settings
struct termios original_termios;

void begin(int *size, char map[V][H]);
void intro(char map[V][H]);
void intro_data(char map[V][H], int *size);
void loop(char map[V][H], int size);
void input_Snake(char map[V][H], int *size, int *dead);
void update(char map[V][H], int size);
void intro_data2(char map[V][H], int size);
void victory();

void gotoxy(int x, int y);
int kbhit(void); // Declaration for custom kbhit
int getch_linux(void); // Declaration for custom getch

void show(char map[V][H]);

// Function to reset terminal settings on exit
void reset_terminal_settings()
{
    tcsetattr(STDIN_FILENO, TCSANOW, &original_termios);
}

// Initialized in a initial position
void begin(int *size, char map[V][H])
{
	int i;
	// snake head
	snk[0].x = 32;
	snk[0].y = 10; // V
	// snake body
	*size = 4;

	srand(time(NULL));

	frt.x = rand() % (H - 2) + 1; // automatic position of fruit
	frt.y = rand() % (V - 2) + 1;

	for (i = 0; i < *size; i++)
	{
		snk[i].movX = 1;
		snk[i].movY = 0;
	} // snake only move on X

	intro(map);
	intro_data(map, size);
}

// Show everything
void show(char map[V][H])
{
	int i, j;
	for (i = 0; i < V; i++)
	{
		for (j = 0; j < H; j++)
		{
			printf("%c", map[i][j]);
		}
		printf("\n");
	}
}

// The camp (map).
void intro(char map[V][H])
{
	int i, j;

	for (i = 0; i < V; i++)
	{
		for (j = 0; j < H; j++)
		{
			if (i == 0 || i == V - 1)
			{
				map[i][j] = '-';
			}
			else if (j == 0 || j == H - 1)
			{
				map[i][j] = '|';
			}
			else
			{
				map[i][j] = ' ';
			}
		}
	}
}

// Introduce every data inIT
void intro_data(char map[V][H], int *size)
{
	int i;

	for (i = 1; i < *size; i++)
	{
		snk[i].x = snk[i - 1].x - 1;
		snk[i].y = snk[i - 1].y;

		snk[i].imagen = '#'; // body
	}
	snk[0].imagen = 'O'; // head

	// Introduce snake into our camp

	for (i = 0; i < *size; i++)
	{
		map[snk[i].y][snk[i].x] = snk[i].imagen;
	}

	map[frt.y][frt.x] = 'X'; // fruit
}

void input_Snake(char map[V][H], int *size, int *dead)
{
	int i;
	char key;

	// Only two ways to die, collision with map or body, every part of the snake is an diferent structure

	// CHECK GAME CONDITIONS

	if (snk[0].x == 0 || snk[0].x == H - 1 || snk[0].y == 0 || snk[0].y == V - 1)
	{ // 0 es la cabeza de la serpiente, solo evaluaremos cuando la cabeza choque.
		*dead = 1;
	}

	for (i = 1; i < *size && *dead == 0; i++)
	{
		if (snk[0].x == snk[i].x && snk[0].y == snk[i].y)
		{
			*dead = 1;
		}
	}

	// CHECK FRUIT, IF HEAD GET CLOSE, EAT IT!

	if (snk[0].x == frt.x && snk[0].y == frt.y)
	{
		*size += 1;

		snk[*size - 1].imagen = '#';

		eaten++;

		// Regenerated fruit position in a random position

		frt.x = rand() % (H - 2) + 1;
		frt.y = rand() % (V - 2) + 1;
	}

	// IF DEAD IS ZERO WE CAN KEEP GOING

	if (*dead == 0)
	{
		if (kbhit() == 1)
		{
			key = getch_linux(); // Use custom getch_linux

            // Aplica toupper() para que as teclas 'w', 'a', 's', 'd' funcionem
            // tanto em maiúsculas quanto em minúsculas
            key = toupper(key);

			if (key == 'S' && snk[0].movY != -1) // Agora comparamos com 'S' maiúsculo
			{
				snk[0].movX = 0;
				snk[0].movY = 1;
			}

			if (key == 'W' && snk[0].movY != 1) // Agora comparamos com 'W' maiúsculo
			{
				snk[0].movX = 0;
				snk[0].movY = -1;
			}

			if (key == 'A' && snk[0].movX != 1) // Agora comparamos com 'A' maiúsculo
			{
				snk[0].movX = -1;
				snk[0].movY = 0;
			}

			if (key == 'D' && snk[0].movX != -1) // Agora comparamos com 'D' maiúsculo
			{
				snk[0].movX = 1;
				snk[0].movY = 0;
			}
		}
	}
}

void loop(char map[V][H], int size)
{
	int dead;
	dead = 0;

	do
	{
		gotoxy(0, 0);
		show(map);
		input_Snake(map, &size, &dead);
		update(map, size); // automatic

		usleep(100000); // Replaced Sleep(10) with usleep(100000) for 100ms on Linux (10ms was too fast)

		if(eaten >= 2){
            victory(); //The user won the game
            dead = 1;
		}

	} while (dead == 0);
}

void update(char map[V][H], int size)
{
	// Everytime this function is called, the map is overwriting

	intro(map);

	intro_data2(map, size);
}

void intro_data2(char map[V][H], int size)
{
	int i;

	// Body following the head

	for (i = size - 1; i > 0; i--)
	{ // 0 is the head. so we going decresing until extremities
		snk[i].x = snk[i - 1].x;
		snk[i].y = snk[i - 1].y;
	}

	snk[0].x += snk[0].movX; // plus movemenrts
	snk[0].y += snk[0].movY;

	// now introduce the values in the camp

	for (i = 0; i < size; i++)
	{
		map[snk[i].y][snk[i].x] = snk[i].imagen;
	}

	map[frt.y][frt.x] = 'X';
}

void victory()
{
    vic = 1;
    gotoxy(0, 0); // Move o cursor para o início
    system("clear"); // Replaced "cls" with "clear" for Linux
    printf("\n\n\n  PARABENS! VOCÊ VENCEU!\n");
    printf("  Você comeu %d frutas! O jogo terminou.\n", eaten);
}

// Linux equivalent for gotoxy
void gotoxy(int x, int y)
{
    printf("\033[%d;%dH", y + 1, x + 1); // ANSI escape codes for cursor positioning
    fflush(stdout); // Ensure the output is immediately written to the console
}

// Linux equivalent for kbhit
int kbhit(void)
{
    struct timeval tv = { 0L, 0L };
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    return select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv);
}

// Linux equivalent for getch
int getch_linux(void)
{
    int r;
    unsigned char c;
    if ((r = read(STDIN_FILENO, &c, sizeof(c))) < 0) {
        return r;
    }
    return c;
}


int PlaySnake()
{
    setlocale(LC_ALL, "Portuguese");
    char ans;

    struct termios new_termios;
    tcgetattr(STDIN_FILENO, &original_termios);
    atexit(reset_terminal_settings);
    new_termios = original_termios;
    new_termios.c_lflag &= ~(ICANON | ECHO);
    new_termios.c_cc[VMIN] = 1;
    new_termios.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);

    do{
        int size;
        char map[V][H];
        eaten = 0;

        system("clear"); // Replaced "cls" with "clear" for Linux
        begin(&size, map);
        show(map);
        printf("\nPressione qualquer tecla para iniciar..."); // Custom "press any key" message
        getch_linux(); // Use custom getch
        loop(map, size);

        if(vic != 1)
        {
            vezes_jogadas += 1;
            if(vezes_jogadas <= 3)
            {
                do
                {
                    printf("\n\n  Jogar novamente? [s/n] ");
                    scanf(" %c", &ans);
                    ans = toupper(ans); // Converte para maiúscula para a comparação abaixo
                }
                while(ans != 'S' && ans != 'N');
            }
            else
            {
                system("clear");
                printf("\n\n                            --- PERDEU! ---                            \n\n");
            }
        }
        else
        {
            return 1;
        }

    }while(ans == 'S' && vezes_jogadas <= 3); // Compara com 'S' maiúsculo

    return 0;
}
