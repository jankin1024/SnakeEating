#include <iostream>
#include <algorithm>
#include <ctime>
#include <conio.h>
#include <windows.h>
using namespace std;

// #define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME)&0x8000)?1:0)

// 定义组成蛇和食物的方块 
struct Block{
	int x;
	int y;
};

struct Snake{
	int len; // 蛇的长度 
	int dir; // 行进方向 
	int speed; // 行进速度 
	Block body[500]; // 用数组描述蛇的身体，最长为500 
	// 改变行进方向 
	void Up();
	void Down();
	void Left();
	void Right();
	// 行进一步 
	void Move();
};

Block food;

// 光标由一点直接跳转到另一点 
void gotoxy(int x,int y) {
	COORD pos;
	pos.X = y - 1;
	pos.Y = x - 1;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),pos);
}

void Snake::Up(){
	if(this->dir!=3)
		this->dir = 1;
}

void Snake::Down(){
	if(this->dir!=1)
		this->dir = 3;
}

void Snake::Right(){
	if(this->dir!=4)
		this->dir = 2;
}

void Snake::Left(){
	if(this->dir!=2)
		this->dir = 4;
}

void Snake::Move(){
	
	gotoxy(body[len - 1].x, body[len - 1].y);
	cout <<' ';
	
	for (int i = len - 1; i > 0; --i) {
		this->body[i].x = this->body[i - 1].x;
		this->body[i].y = this->body[i - 1].y;
	}
	if (this->dir == 1) {
		this->body[0].x--;
	}
	else if (this->dir == 2) {
		this->body[0].y++;
	}
	else if (this->dir == 3) {
		this->body[0].x++;
	}
	else if (this->dir == 4) {
		this->body[0].y--;
	}
	gotoxy(this->body[0].x, this->body[0].y);
	cout <<'Q';
}

void initSnake(Snake &s){
	s.body[0].x = 20;
	s.body[0].y = 30;
	s.len = 1;
	s.dir = 2;
	int ch;
	gotoxy(15, 29);
	cout << "*** Snake Speed(1-5) ***";
	gotoxy(15, 55);
//	scanf("%c", &ch);
	cin>>ch;
	if(ch<=5 || ch>0){
		s.speed = ch;
	}else{
		s.speed = 3;
	} 
	gotoxy(16, 29);
	cout << "**** BEGIN(Y/N)? ****" << endl;
	gotoxy(17, 40);
}

// 欢迎界面 
void Welcome(Snake &s){
	gotoxy(13, 29);
	cout << "**** Snake Eating ****";
	gotoxy(14, 31);
	cout << "**** WELCOME ****";
	Sleep(1750);
	initSnake(s);	
}

void Lose(int s){
	gotoxy(13, 29);
	cout << "**** Snake Eating ****";
	gotoxy(14, 31);
	cout << "**** YOU LOSE ****";
	gotoxy(15, 27);
	cout << "** your scores : "<< s << " !!! **";
	gotoxy(16, 31);
	cout << "****AGAIN(Y/N)?****" << endl;
	gotoxy(17, 40);
}

void Win(int s){
	gotoxy(13, 29);
	cout << "**** Snake Eating ****";
	gotoxy(14, 31);
	cout << "**** YOU WIN ****";
	gotoxy(15, 27);
	cout << "** your scores : "<< s << " !!! **";
	gotoxy(16, 31);
	cout << "****AGAIN(Y/N)?****" << endl;
	gotoxy(17, 40);
}

void initUI(){
	system("cls");
	for (int i = 1; i <= 80; i++)
	{
		Sleep(8);
		gotoxy(1, i);
		cout << '=';
		gotoxy(30, i);
		cout << '=';
	}	
	for(int i=2;i<=29;i++)
	{
		Sleep(10);
		gotoxy(i, 1);
		cout << '|';
		for (int j = 2; j <= 79; ++j){
			cout << ' ';
		}
		cout << '|';
	}
}

void setFood(Snake &s){
	food.x = rand() % 28 + 2;
	food.y = rand() % 78 + 2;
	int i;
	int flag = 1;
	while(flag){
		for (i = 0; i < s.len; ++i){
			if(s.body[i].x==food.x && s.body[i].y == food.y){
				break;
			}
		}
		if(i==s.len){
			flag = 0;
			gotoxy(food.x, food.y);
			cout << '#';
		}else{
			food.x = rand() % 29 + 1;
			food.y = rand() % 79 + 1;
		}
	}
	return;
}

void Eat(Snake &s){
	if(s.body[0].x==food.x && s.body[0].y==food.y){
		s.len++;
		setFood(s);
	}
}

bool isOver(Snake &s){
	if(s.body[0].x<=0 || s.body[0].x>=30 || s.body[0].y<=0 || s.body[0].y>=80){
		return true;
	}
	
	
	return false;
}

int main(int argc, char const *argv[]){
	char ch;
	srand((unsigned)time(NULL));
	Snake snake;

	do{
		initUI();
		Welcome(snake);
//		scanf("%c", &ch);
		cin>>ch;
		if(ch!='Y' && ch!='y')
			continue;
		initUI();
		setFood(snake);

		while (snake.len<=20) {
			if(kbhit()) {  // 有按键事件发生
				ch=getch();	// 获得所输入的键盘 
				switch (ch){
					case 'A':
					case 'a':
					case 75: // <-键 
						snake.Left();
						break;
					case 's':
					case 'S':
					case 80: 
						snake.Down();
						break;
					case 'd':
					case 'D':
					case 77:
						snake.Right();
						break;
					case 'w':
					case 'W':
					case 72:
						snake.Up();
						break;
				}
			}

			snake.Move();
			Eat(snake);

			if(isOver(snake)){
				break;
			}
			
			Sleep(500/snake.speed);
		}


		if(isOver(snake)){
			Lose(snake.len*10);
		}else{
			Win(snake.len*10);
		}
		cin>>ch;
		if(ch != 'Y' && ch != 'y') break;
		
	} while (ch == 'Y' || ch == 'y');
	
	system("cls");  // 清屏 

	return 0;
}



//				time_t first;
//				time(&first);
//				if(kbhit()){
//					char c;
//					time_t second;
//					time(&second);
//					c = getch();
//					if(difftime(first, second)<=0.005){
//						if(ch==c){
//							snake.speed++;
//							if(snake.speed >5)
//								snake.speed = 5;
//						}
//						else if(ch!=c){
//							snake.speed--;
//							if(snake.speed <1)
//								snake.speed = 1;
//						}
//					}
//				}

