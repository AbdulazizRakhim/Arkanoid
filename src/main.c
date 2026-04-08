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

#define WALL 177 // replace from 176 to 178 for polishing wall
#define RACKET 205
#define BALL 111

typedef struct TRacket{

	int	x, y;
	int	width;

}TRacket;

typedef struct TBall{
	
	float x, y;
	int ix, iy;
	float angle;
	float speed;

}TBall;

unsigned char map[HEIGHT][WIDTH + 1];	// '\0' for each line for HEIGHT
TRacket racket;
TBall ball;
TBall copy_ball;
int hit_counter = 0;
int max_counter = 0;
int level = 1;
int level_needed = 6;

void move_ball(float x, float y);
void show_preview(int level);

int is_inside(int x, int y)
{
	return (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT);
}

unsigned char get_map(int x, int y)
{
	if (x < 0 || x >= WIDTH)
		return WALL;

	if (y < 0)
		return WALL;

	if (y >= HEIGHT)
		return ' ';

	return map[y][x];
}

void create_ball()
{
	move_ball(2, 2);
	ball.angle = -1;
	ball.speed = 0.6;
}

void put_ball()
{
	if(is_inside(ball.ix,ball.iy)) // safe borders
		map[ball.iy][ball.ix] = BALL;
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
	// keep the radians in circle
	if (ball.angle < 0)
		ball.angle = ball.angle + M_PI * 2;
	if (ball.angle > M_PI * 2)
		ball.angle = ball.angle - M_PI * 2;

	copy_ball = ball;

	move_ball(ball.x + cos(ball.angle) * ball.speed,
		      ball.y + sin(ball.angle) * ball.speed);
	
	// changing direction based on quadrants and radians

	if ( (get_map(ball.ix, ball.iy) == WALL) || (get_map(ball.ix, ball.iy) == RACKET))
	{

		if (get_map(ball.ix, ball.iy) == RACKET)
		{
			hit_counter++;
			if (hit_counter >= level_needed)
			{
				level++;
				hit_counter = 0;
				printf("\a");
				show_preview(level);
			}
		}

		// each frame might have different positions  
		if ((ball.ix != copy_ball.ix) && (ball.iy != copy_ball.iy))
		{
			unsigned char probe_x = get_map(ball.ix, copy_ball.iy); 
			unsigned char probe_y = get_map(copy_ball.ix, ball.iy);

			// if ball gets to corner take the ball back 
			if ((probe_x == WALL || probe_x == RACKET) && (probe_y == WALL || probe_y == RACKET))
				copy_ball.angle = copy_ball.angle + M_PI;
			
			// if ball's x position enters to wall, change its angle
			else if (probe_x == WALL || probe_x == RACKET)
				copy_ball.angle = (2 * M_PI - copy_ball.angle) + M_PI;
			
			// if there is nothing just fly to up or down 
			else if (probe_y == WALL || probe_y == RACKET)
				copy_ball.angle = (2 * M_PI - copy_ball.angle);
			
			// if ball quits the field, take it back to paddle (safely) 
			else
				copy_ball.angle = copy_ball.angle + M_PI; 				
		
		}
		else if (ball.iy == copy_ball.iy) // if both .iy positions stay same, change opposite 
			copy_ball.angle = (2 * M_PI - copy_ball.angle) + M_PI;
		else 
			copy_ball.angle = (2 * M_PI - copy_ball.angle); // changing only vertical position
		
		// keep the radians in circle again (safely)
		if (ball.angle < 0)
			ball.angle = ball.angle + M_PI * 2;
		if (ball.angle > M_PI * 2)
			ball.angle = ball.angle - M_PI * 2;

		ball = copy_ball; // update its coordination and move the ball
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
		map[racket.y][i] = RACKET; 
}

void create_map(int lvl)
{
	
	for (int i = 0; i < WIDTH; i++)
		map[0][i] = WALL; 

	map[0][WIDTH] = '\0';

	strncpy(map[1], map[0], WIDTH + 1);
	for (int i = 1; i < WIDTH - 1; i++)
		map[1][i] = ' ';	// use '.' for deep analysis

	for (int i = 2; i < HEIGHT; i++)
		strncpy(map[i], map[1], WIDTH + 1);

	if (lvl == 1)
		for (int i = 20; i < 50; i++)
			map[10][i] = WALL;
	else if (lvl == 2)
	{
		for (int i = 4; i < 10; i += 5)
			for (int j = 20; j < 50; j++)
				map[i][j] = WALL;
	}
	else if (lvl == 3)
	{
		for (int i = 10; i < 60; i += 9)
			for (int j = 3; j <= 15; j++)
				map[j][i] = WALL;
	}
	else
		level = 1;

}

void show_map(BOOL debug_overlay)
{

	for (int i = 0; i < HEIGHT; i++)
	{
		printf("%s", map[i]);

		if (i == 3)
			printf("    Hits  %-5d", hit_counter);
		if (i == 5)
			printf("    Score %-5d", max_counter);

		if (debug_overlay)
		{
			if (i == 8)
				printf("    | Angle: %-8.2f", ball.angle);
			else if (i == 10)
				printf("    | cos ( %-5.2f): %-8.3f", ball.angle, cos(ball.angle));
			else if (i == 12)
				printf("    | sin ( %-5.2f): %-8.3f", ball.angle, sin(ball.angle));
			else if (i == 14)
				printf("    | Ball position: [X = %-2d, Y = %-2d]", ball.ix, ball.iy);
			else if (i == 16)
				printf("    | Ball copy:     [X = %-2d, Y = %-2d]", copy_ball.ix, copy_ball.iy);
		}
		else
		{
			if (i == 8 || i == 10 || i == 12 || i == 14 || i == 16)
				printf("%-55s", "");
		}

		if (i < HEIGHT - 1)
			printf("\n");
	}

	printf("\n\n\n\nPress X to Toggle Debug Overlay: ");
}

void move_racket(int x)
{
	racket.x = x;

	if (racket.x < 1)
		racket.x = 1;


	if (racket.x + racket.width >= WIDTH)	// 200 + 7 = 207 >= 65 (by default)
		racket.x = WIDTH - 1 - racket.width;

}

//winapi32 functions

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

//

void show_preview(int level)
{
	system("cls");
	hide_cursor();
	if (level >= 5)
	{
		level = 1;
		printf("\n\n\n\n\n\n\n\n\t\t\t\t\t\t\n");
		printf("\n\n\t\t\t\t\t\t   END OF THE GAME\n");
		
		Beep(400, 800);
		Sleep(2000); // 2 sec 
		system("cls");
	}
	else if (level > 1)
		printf("\n\n\n\n\n\n\n\n\t\t\t\t\t\t\n");
	if (level == 1)
	{
		printf("\n\n\n\n\n\n\n\n\t\t\t\t\t\tArkanoid game\n");
		Beep(300, 500);
	}

	printf("\n\n\t\t\t\t\t\t   LEVEL %d", level);
	

	
	Sleep(2000); // 2 sec 
	system("cls");
}

int main(int argc, char* argv[])
{
	show_preview(level);
	SetConsoleOutputCP(437);

	BOOL run = FALSE;
	BOOL debug_overlay = FALSE;
	BOOL was_pressed = FALSE;

	hide_cursor();
	create_racket();
	create_ball();

	do
	{
		set_cursor(0, 0);

		if (run)
			flying_ball();

		if (ball.iy > HEIGHT - 1)
		{
			run = FALSE;

			 if (hit_counter > max_counter)
				max_counter = hit_counter;

			hit_counter = 0;
		}

		BOOL is_pressed = (GetKeyState('X') < 0);

		if (is_pressed && !was_pressed)
			debug_overlay = !debug_overlay;
		
		was_pressed = is_pressed;

		create_map(level);

		put_racket();
		put_ball();

		show_map(debug_overlay);

		if (GetKeyState('A') < 0)
			move_racket(racket.x - 1);
		else if (GetKeyState('D') < 0)
			move_racket(racket.x + 1);
		else if (GetKeyState('W') < 0)
			run = TRUE;

		if(!run)
		move_ball(racket.x + racket.width / 2, racket.y - 1); // place the ball on the center of racket
		//			29	   +     7		  / 2,    24    - 1
		
		Sleep(10);

	} while (GetKeyState(VK_ESCAPE) >= 0);

	printf("\n\n");

	return 0;
}
