#define _CRT_SECURE_NO_WARNINGS
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"
#include "conio.h"
#include "windows.h"

#define WIDTH 65
#define HEIGHT 25

typedef struct {

	int x, y;
	int width;

}TRacket;

char map[HEIGHT][WIDTH + 1];	// '\0' for each line for HEIGHT
TRacket racket;

void create_racket()
{
	racket.width = 7;

	racket.x = (WIDTH - racket.width) / 2;
	racket.y = HEIGHT - 1;
}

void put_racket()
{
	for (int i = racket.x; i < racket.x + racket.width; i++)
		map[racket.y][i] = '@';
}

void create_map()
{
	for (int i = 0; i < WIDTH; i++)
		map[0][i] = '#';

	map[0][WIDTH] = '\0';

	strncpy(map[1], map[0], WIDTH + 1);
	for (int i = 1; i < WIDTH - 1; i++)
		map[1][i] = ' ';

	for (int i = 2; i < HEIGHT; i++)
		strncpy(map[i], map[1], WIDTH + 1);

}

void show_map()
{
	for (int i = 0; i < HEIGHT; i++)
	{
		printf("%s", map[i]);
		if (i < HEIGHT - 1)
			printf("\n");
	}
}

void move_racket(int x)
{
	racket.x = x;

	if (racket.x < 1)
		racket.x = 1;


	if (racket.x + racket.width >= WIDTH)	// 200 + 7 = 207 >= 65 (by default)
		racket.x = WIDTH - 1 - racket.width;

}

void set_cursor(int x, int y)
{
	COORD coord;

	coord.X = x;
	coord.Y = y;

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void hide_cursor()
{
	CONSOLE_CURSOR_INFO ci = { 100, FALSE };
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &ci);
}

int main(int argc, char* argv[])
{
	hide_cursor();
	create_racket();

	do
	{
		set_cursor(0, 0);
		create_map();
		put_racket();

		show_map();

		if (GetKeyState('A') < 0)
			move_racket(racket.x - 1);
		else if (GetKeyState('D') < 0)
			move_racket(racket.x + 1);

		Sleep(1);

	} while (GetKeyState(VK_ESCAPE) >= 0);

	printf("\n\n");

	return 0;
}
