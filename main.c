#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>

typedef struct Coords
{
	int x;
	int y;
} coords;

typedef struct Vector
{
    coords position;
    int direction; // [1, 2, 3, 4] => [left, up, right, down]
} vector;

void draw(int width, int height, vector snake, coords fruit, int score);
int random(int min, int max);
int isGameover(int width, int height, vector snake);
void delay(int ms);
void move(vector *snake);
void changeDir(vector *snake);
int isSnakeAteFruit(vector snake, coords fruit, int *score);
void spawnFruit(int width, int height, coords *fruit);

int main(void)
{
    int gameover = 0;
    int width = 20;
    int height = 20;
    int score = 0;

    vector snake;
    coords fruit;

    snake.position.x = random(3, width - 3);
    snake.position.y = random(3, height - 3);
    snake.direction = random(1, 4);
    fruit.x = random(1, width - 1);
    fruit.y = random(1, height - 1);

    while(!gameover)
    {
        delay(50);
        if(kbhit()) changeDir(&snake);
        move(&snake);
        if(isSnakeAteFruit(snake, fruit, &score)) spawnFruit(width, height, &fruit);
        gameover = isGameover(width, height, snake);
        draw(width, height, snake, fruit, score);
    }

    system("cls");
    printf("GAME OVER\n");
    printf("SCORE: %i\n", score);

    return 0;
}

void draw(int width, int height, vector snake, coords fruit, int score)
{
    int x, y;

    system("cls");
    printf("%i", snake.direction);
	printf("\t\t\tScore: %i\n", score);

    for(y = 0; y <= height; y++)
    {
        for(x = 0; x <= width; x++)
        {
            if(y == 0 || y == height)
			{
				printf("Z");
			}
			else if (x == 0 || x == width)
			{
				printf("H");
			}
			else if(x == snake.position.x && y == snake.position.y)
			{
				printf("#");
			}
			else if(x == fruit.x && y == fruit.y)
			{
				printf("$");
			}
			else
			{
				printf(" ");
			}
        }
        printf("\n");
    }
}

int random(int min, int max)
{
    srand(time(NULL));
	return rand() % max + min;
}

int isGameover(int width, int height, vector snake)
{
    if(snake.position.x == 1 || snake.position.x == width || snake.position.y == 1 || snake.position.y == height) return 1;
    return 0;
}

void delay(int ms)
{
	clock_t start_time = clock();

	while(clock() < start_time + ms);
}

void move(vector *snake)
{
    switch(snake -> direction)
	{
		case 1:
			snake -> position.x -= 1;
			break;
		case 2:
			snake -> position.y += 1;
			break;
		case 3:
			snake -> position.x += 1;
			break;
		case 4:
			snake -> position.y -= 1;
			break;
	}
}

void changeDir(vector *snake)
{
	switch(getch())
	{
		case 'a':
			if(snake -> direction != 3) snake -> direction = 1;
			break;
		case 's':
			if(snake -> direction != 4) snake -> direction = 2;
			break;
		case 'd':
			if(snake -> direction != 1) snake -> direction = 3;
			break;
		case 'w':
			if(snake -> direction != 2) snake -> direction = 4;
			break;
    }
}

int isSnakeAteFruit(vector snake, coords fruit, int *score)
{
    if(snake.position.x == fruit.x && snake.position.y == fruit.y) 
    {
        ++*score;
        return 1;
    }
    return 0;
}

void spawnFruit(int width, int height, coords *fruit)
{
    fruit -> x = random(1, width - 1);
    fruit -> y = random(1, height - 1);
}