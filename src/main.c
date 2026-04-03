#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"
#include "conio.h"
#include "windows.h"

#define WIDTH 65
#define HEIGHT 25

typedef struct {

	int	x;
	int	y;
	int	width;

}TRacket;

typedef struct {
	
	float x;
	float y;
	int ix;
	int iy;
	float angle;
	float speed;
}TBall;

char map[HEIGHT][WIDTH + 1];	// '\0' for each line for HEIGHT
TRacket racket;
TBall ball;
int hit_counter = 0;
int max_counter = 0;

void move_ball(float x, float y);

void create_ball()
{
	move_ball(2, 2);
	ball.angle = -1;
	ball.speed = 0.5;
}

void put_ball()
{
	map[ball.iy][ball.ix] = '*';
}

void move_ball(float x, float y)
{
	ball.x = x;
	ball.y = y;
	ball.ix = (int)round(ball.x);
	ball.iy = (int)round(ball.y);
}

void flying_ball()
{
	if (ball.angle < 0)
		ball.angle = ball.angle + M_PI * 2;
	else if (ball.angle > M_PI * 2)
		ball.angle = ball.angle - M_PI * 2;

	TBall copy_ball = ball;

	move_ball(ball.x + cos(ball.angle) * ball.speed,
		      ball.y + sin(ball.angle) * ball.speed);
	
	if ((map[ball.iy][ball.ix] == '#') || (map[ball.iy][ball.ix] == '@'))
	{
		if (map[ball.iy][ball.ix] == '@')
			hit_counter++; 

		if ((ball.ix != copy_ball.ix) && (ball.iy != copy_ball.iy))
		{
			if (map[copy_ball.iy][copy_ball.ix] == map[ball.iy][ball.ix])
				copy_ball.angle = copy_ball.angle + M_PI;
			else
			{
				if(map[copy_ball.iy][ball.ix] == '#')
					copy_ball.angle = (2 * M_PI - copy_ball.angle) + M_PI;
				else
					copy_ball.angle = (2 * M_PI - copy_ball.angle);
			}
		}
		else if (ball.iy == copy_ball.iy)
			copy_ball.angle = (2 * M_PI - copy_ball.angle) + M_PI;
		else
			copy_ball.angle = (2 * M_PI - copy_ball.angle);
		
		ball = copy_ball;
		flying_ball();
	}

}

void create_racket()
{
	racket.width = 7;

	racket.x = (WIDTH - racket.width) / 2; // (65 - 7) / 2 = 29 (by default)
	racket.y = HEIGHT - 1; // 24 (by default)
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
		map[1][i] = ' ';	// use '.' for deep analysis

	for (int i = 2; i < HEIGHT; i++)
		strncpy(map[i], map[1], WIDTH + 1);

}

void show_map()
{
	for (int i = 0; i < HEIGHT; i++)
	{
		printf("%s", map[i]);

		if (i == 3)
			printf("    Hits  %-5d", hit_counter);
		else if (i == 5)
			printf("    Score %-5d", max_counter);
		else if (i == 8)
			printf("    | Angle: %-8.2f", ball.angle);
		else if (i == 10)
			printf("    | cos (%-3.2f): %-8.3f", ball.angle, cos(ball.angle));
		else if (i == 12)
			printf("    | sin (%-3.2f): %-8.3f", ball.angle, sin(ball.angle));
		else if (i == 14)
			printf("    | Ball position: [X = %-2d, Y = %-2d]", ball.ix, ball.iy);
		
		if (i < HEIGHT - 1)
			printf("\n");
	}

	printf("\n\n\n\nPress X for Debug Overlay: ");
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
	BOOL run = FALSE;

	hide_cursor();
	create_racket();
	create_ball();

	do
	{
		set_cursor(0, 0);

		if (run)
			flying_ball();

		if (ball.iy > HEIGHT)
		{
			run = FALSE;

			if (hit_counter > max_counter)
				max_counter = hit_counter;

			hit_counter = 0;
		}

		create_map();

		put_racket();
		put_ball();

		show_map();

		if (GetKeyState('A') < 0)
			move_racket(racket.x - 1);
		else if (GetKeyState('D') < 0)
			move_racket(racket.x + 1);
		else if (GetKeyState('W') < 0)
			run = TRUE;

		if(!run)
		move_ball(racket.x + racket.width / 2, racket.y - 1); // place the ball on the center of racket
		//			29	   +     7		  / 2,    24    - 1
		
		Sleep(1);

	} while (GetKeyState(VK_ESCAPE) >= 0);

	printf("\n\n");

	return 0;
}
