#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define WIDTH 80
#define HEIGHT 40

typedef struct {
	int x, y;
}Position;

typedef struct {
	Position body[100];
	int length;
	char direction;
}Snake;

typedef struct {
	Position pos;
}Food;

Snake snake;
Food food;
int score = 0;


void init_game(){
	initscr();
	cbreak();
	noecho();
	curs_set(0);
	keypad(stdscr, TRUE);
	timeout(100);

	snake.length = 3;
	snake.direction= 'r';

	for(int i = 0 ; i <snake.length;i ++){
		snake.body[i].x = WIDTH/2 - i;
		snake.body[i].y = HEIGHT/2;
	}

	srand(time(NULL));
	food.pos.x = rand()%WIDTH;
	food.pos.y = rand()%HEIGHT;

}

void draw(){
	clear();

	for(int i = 0; i < snake.length; i++){
		mvprintw(snake.body[i].y, snake.body[i].x, "@");
	}

	mvprintw(food.pos.y, food.pos.x, "#");
	
	mvprintw(HEIGHT, 0, "Score:%d",score);
	refresh();
}

void update(){
	for(int i = snake.length - 1; i > 0; i--){
		snake.body[i] = snake.body[i-1];
	}

	switch(snake.direction){
		case 'u':
			snake.body[0].y--;
			break;
		case 'd':
			snake.body[0].y++;
			break;
		case 'l':
			snake.body[0].x--;
			break;
		case 'r':
			snake.body[0].x++;
			break;
	}

	if(snake.body[0].x == food.pos.x && snake.body[0].y== food.pos.y){
		snake.length++;
		score += 10;
		food.pos.x = rand()%WIDTH;
		food.pos.y = rand()%HEIGHT;
	}

	if(snake.body[0].x < 0 || snake.body[0].x >= WIDTH || snake.body[0].y < 0 || snake.body[0].y >= HEIGHT){
		endwin();
		printf("Game Over! Your score:%d", score);
		exit(0);
	}

	for(int i =1; i < snake.length; i++){
		if(snake.body[0].x == snake.body[i].x && snake.body[0].y == snake.body[i].y){
			endwin();
			printf("Game Over! Your score:%d", score);
			exit(0);
		}
	}
}

void input(){

	int ch = getch();
	switch(ch){
		case KEY_UP:
			if(snake.direction!='d')snake.direction = 'u';
			break;
		case KEY_DOWN:
			if(snake.direction != 'u')snake.direction = 'd';
			break;
		case KEY_LEFT:
			if(snake.direction != 'r') snake.direction = 'l';
			break;
		case KEY_RIGHT:
			if(snake.direction != 'l')snake.direction = 'r';
			break;
		case 'q':
			endwin();
			exit(0);
	}	
}


int main(){
	init_game();
	while(1){
		draw();
		input();
		update();
		usleep(100000);
	}
	endwin();
	return 0;
}

