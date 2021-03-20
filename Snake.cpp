
#include <iostream>
#include <stdio.h>
#include <Windows.h> 
#include <conio.h>
#include <time.h>
#include <vector>

// kbhit() - нажатие клавиши

//CONSOLE_SCREEN_BUFFER_INFO buf;
//coord.x = buf.dwCursorPosition.x;

#define size 20

using namespace std;
HANDLE nConsole = GetStdHandle(STD_OUTPUT_HANDLE);


int field[size][size]; // 0 - пустое поле, 2 - голова, 1 - хвост, 3 фрукт, 6 - жизнь
COORD tail[size * size]; // хвост
enum direction { STOP, UP, DOWN, LEFT, RIGHT };

// события
bool gameOver;
int eat = 0;
int gain_life = 0;
int loose_life = 0;
int loose_score = 0;
int LOOS = 0;
//
int life = 0; // кол-во жизней
int life_counter = 0; // счетчик жизней
int life_x, life_y;
//
int score = 0;
int stail = 0; // длина хвоста
direction dir;
int x, y; // координаты головы
int fruit_x, fruit_y; // координаты фрукта
int speed;

void setup() {
	dir = STOP;
	gameOver = false;
	speed = 50;
	x = 0;
	y = 0;

	life_x = life_y = -1;

	fruit_x = rand() % size;
	fruit_y = rand() % size;

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			field[i][j] = 0;
		}
	}
	field[x][y] = 2;

	do {
		fruit_x = rand() % size;
		fruit_y = rand() % size;
	} while (field[fruit_x][fruit_y]);
	field[fruit_x][fruit_y] = 3;

}

void Draw() {
	SetConsoleCursorPosition(nConsole, { 0, 0 });
	cout << "score : ";
	if (eat) {
		SetConsoleTextAttribute(nConsole, 14);
		eat--;
		cout << score;
		SetConsoleTextAttribute(nConsole, 15);
		cout << " +10" << endl;
	}
	else if (loose_score) {
		SetConsoleTextAttribute(nConsole, 12);
		loose_score--;
		cout << score;
		SetConsoleTextAttribute(nConsole, 15);
		cout << " -" << LOOS << endl;
	}
	else cout << score << "                   " << endl;	

	cout << "life : ";
	if (gain_life) {
		SetConsoleTextAttribute(nConsole, 10);
		gain_life--;
		cout << life << endl;;
		SetConsoleTextAttribute(nConsole, 15);
	}
	else if (loose_life) {
		SetConsoleTextAttribute(nConsole, 12);
		loose_life--;
		cout << life << endl;
		SetConsoleTextAttribute(nConsole, 15);
	}
	else cout << life << "                 " << endl;

	for (int i = 0; i < size + 2; i++) {
		_putch('.');
	}
	_putch('\n');
	for (int i = 0; i < size; i++) {
		_putch('.');
		for (int j = 0; j < size; j++) {
			switch (field[i][j]) {
			case 0:
				cout << ' ';
				break;
			case 1:
				SetConsoleTextAttribute(nConsole, 10);
				cout << 'o';
				SetConsoleTextAttribute(nConsole, 15);
				break;
			case 5:
				SetConsoleTextAttribute(nConsole, 2);
				cout << 'o';
				SetConsoleTextAttribute(nConsole, 15);
				break;
			case 2:
				SetConsoleTextAttribute(nConsole, 10);
				cout << 'Q';
				SetConsoleTextAttribute(nConsole, 15);
				break;
			case 3:
				SetConsoleTextAttribute(nConsole, 14);
				cout << '$';
				SetConsoleTextAttribute(nConsole, 15);
				break;
			case 4:
				SetConsoleTextAttribute(nConsole, 12);
				cout << 'X';
				SetConsoleTextAttribute(nConsole, 15);
				break;
			case 6:
				SetConsoleTextAttribute(nConsole, 13);
				cout << '@';
				SetConsoleTextAttribute(nConsole, 15);
				break;

			}
		}
		puts(".");
	}
	for (int i = 0; i < size + 2; i++) {
		_putch('.');
	}
	cout << "\n< Q - exit, WASD - control >\n";
}

void Input() {
	if (_kbhit()) {
		switch (_getch()) {
		case 'w':
			if (dir != DOWN || !stail) dir = UP;
			break;
		case 'a':
			if (dir != RIGHT || !stail) dir = LEFT;
			break;
		case 's':
			if (dir != UP || !stail) dir = DOWN;
			break;
		case 'd':
			if (dir != LEFT || !stail) dir = RIGHT;
			break;
		case 'q':
			gameOver = true;
			break;
		default:
			break;
		}
	}
}

void Life() {
	if (life_counter < 9) {
		life_counter++;
	}
	else if(life_counter == 9){
		do {
			life_x = rand() % size;
			life_y = rand() % size;
		} while (field[life_x][life_y]);
		field[life_x][life_y] = 6;
		life_counter++;
	}
}

void Crop() { // самокусь с обрезанием
	int i;
	for (i = 0; i < stail; i++) {
		if (tail[i].X == x && tail[i].Y == y) break;
	}

	for (int j = i-1; j < stail; j++) {
		field[tail[j].X][tail[j].Y] = 0;
	}
	score -= 10 * (stail - i);
	LOOS = 10*(stail - i);
	stail -= stail - i;
}

void Logic() {
	COORD prevtail = { x, y };

	switch (dir)
	{
	case STOP:
		break;
	case LEFT:
		field[x][y] = 0;
		y -= 1;
		if (y >= size) y = 0; else if (y < 0) y = size - 1;
		field[x][y] = 2;
		break;
	case RIGHT:
		field[x][y] = 0;
		y += 1;
		if (y >= size) y = 0; else if (y < 0) y = size - 1;
		field[x][y] = 2;
		break;
	case UP:
		field[x][y] = 0;
		x -= 1;
		if (x >= size) x = 0; else if (x < 0) x = size - 1;
		field[x][y] = 2;
		break;
	case DOWN:
		field[x][y] = 0;
		x += 1;
		if (x >= size) x = 0; else if (x < 0) x = size - 1;
		field[x][y] = 2;
		break;
	} // движение головы

	for (int i = 0; i < stail; i++) { // движение хвоста
		COORD prevtail2 = tail[i];
		tail[i] = prevtail;
		prevtail = prevtail2;
		if (stail - i < 4) { // последние сегменты хвоста будут темнее
			field[tail[i].X][tail[i].Y] = 5;
		}
		else {
			field[tail[i].X][tail[i].Y] = 1;
		}
		field[prevtail.X][prevtail.Y] = 0;
	}


	if (x == fruit_x && y == fruit_y) { // поiв фрукт
		Life(); // + жизнь
		eat = 5;
		score += 10;
		tail[stail] = prevtail;
		field[tail[stail].X][tail[stail].Y] = 5;
		stail++;
		if (stail == size * size - 1) { // победа
			Draw();
			gameOver = true;
			return;
		}

		do {
			fruit_x = rand() % size;
			fruit_y = rand() % size;
		} while (field[fruit_x][fruit_y]);
		field[fruit_x][fruit_y] = 3;
	}

	if (x == life_x && y == life_y) { // съел жизнь
		life_counter = 0;
		life++;
		life_x = life_y = -1;
		gain_life = 5;
	}

	if (field[x][y] == 1 || field[x][y] == 5) { // самокусь
		if (life) { // - жизнь
			life--;
			loose_life = 5;
			loose_score = 5;
			Crop();
			return;
		}
		else { // смерть
			for (int i = stail - 1; i >= 0; i--) {
				field[tail[i].X][tail[i].Y] = 0;
				field[x][y] = 4;
				Draw();
				Sleep(speed * 0.75);
			}
			gameOver = true;
			return;
		}
	}
}

int main()
{
	double stime = time(0);
	srand(stime);
	setup();


	while (!gameOver) {
		Draw();
		Input();
		Logic();
		Sleep(speed);
	}

	return 0;
}