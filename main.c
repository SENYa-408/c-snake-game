#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

int getch();
int kbhit();
int delay();
int randInt();
void draw();
int move();
void input();
void checkFruit();

struct coords 
{
	int x;
	int y;
	int direction; // [1, 2, 3, 4] => [left, up, right, down]
};

struct coords snake;
struct coords fruit;
int width = 81, height = 41, score = 0;

int getch(void)
{
    struct termios oldattr, newattr;
    int ch;
    tcgetattr( STDIN_FILENO, &oldattr );
    newattr = oldattr;
    newattr.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
    return ch;
}

int kbhit(void)
{
  struct termios oldt, newt;
  int ch;
  int oldf;

  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

  ch = getchar();

  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);

  if(ch != EOF)
  {
    ungetc(ch, stdin);
    return 1;
  }

  return 0;
}

int delay(int milseconds)
{
	clock_t start_time = clock();

	while(clock() < start_time + milseconds);
}

int randInt(int minInt, int maxInt)
{
	srand(time(NULL));
	return rand() % maxInt + minInt;
}

void draw(int width, int height, struct coords *snake, struct coords fruit, int score)
{
	system("clear");
	printf("%i", snake -> direction);
	printf("\t\t\tScore: %i\n", score);

	for(int y = 1; y <= height; y++)
	{
		for(int x = 1; x <= width; x++)
		{
			if(y == 1 || y == height)
			{
				printf("Z");
			}
			else if (x == 1 || x == width)
			{
				printf("H");
			}
			else if(x == snake -> x && y == snake -> y)
			{
				printf("S");
			}
			else if(x == fruit.x && y == fruit.y)
			{
				printf("F");
			}
			else
			{
				printf(" ");
			}
		}
		puts("");
	}
}

int move(int width, int height, struct coords *snake, struct coords fruit, int score)
{
	switch(snake -> direction)
	{
		case 1:
			snake -> x -= 1;
			break;
		case 2:
			snake -> y += 1;
			break;
		case 3:
			snake -> x += 1;
			break;
		case 4:
			snake -> y -= 1;
			break;
	}

	if(snake -> y < 2 || snake -> x < 2 || snake -> y > height - 1 || snake -> x > width - 1) return 0;

	draw(width, height, snake, fruit, score);
	return 1;
}

void input()
{
	if(kbhit())
	{
		switch(getch())
		{
			case 'a':
				if(snake.direction != 3) snake.direction = 1;
				break;
			case 's':
				if(snake.direction != 4) snake.direction = 2;
				break;
			case 'd':
				if(snake.direction != 1) snake.direction = 3;
				break;
			case 'w':
				if(snake.direction != 2) snake.direction = 4;
				break;
		}
	}
}

void checkFruit(struct coords snake, struct coords *fruit, int *score)
{
	if(snake.x == fruit -> x && snake.y == fruit -> y)
	{
		++*score;
		fruit -> x = randInt(2, width - 2);
		fruit -> y = randInt(2, height - 2);
	}
}

int main()
{	
	int gameover;
	snake.x = randInt(2, width - 2);
	snake.y = randInt(2, height - 2);
	snake.direction = randInt(1, 4);
	fruit.x = randInt(2, width - 2);
	fruit.y = randInt(2, height - 2);

	draw(width, height, &snake, fruit, score);

	while(1)
	{
		delay(500);
		gameover = move(width, height, &snake, fruit, score);
		if(!gameover) break;
		input();
		checkFruit(snake, &fruit, &score);
	}

	system("clear");
	printf("You lost\nYour score: %i\n", score);

	return 0;
}
