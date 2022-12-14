#include "stdafx.h"
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>

#include <SFML/Graphics.hpp>

#define WIDTH 10
#define HEIGHT 20
#define r <<24 |
#define g <<16 |
#define b <<8 |
using namespace std;
using namespace sf;

class Point
{
public:
	int x, y;
	Point():x(0), y(0) {}
	Point (int xi, int yi) { x = xi; y = yi; }
};

//vector<Point> holes;

int type, nextType;
bool vertical, demoVertical, demoTempVertical;
int dasDir, pieceDrops = 0;
bool game = true, demo, timelapse = false;
Point body[4], nextBody[4], demoBody[4], demoTempBody[4];
Point pos, demoPos, demoTempPos;
int field[WIDTH][HEIGHT];
int points[4] = { 40, 100, 300, 1200 };
int lineBatches[4] = {};
int pieces[7] = {};
int lines = 0, score = 0, startLv;
bool softDropAllowed = false;
clock_t dasDelay = CLOCKS_PER_SEC / 60 * 6, dasStartDelay = CLOCKS_PER_SEC / 60 * 16, lastDas = clock(), dasStart;
clock_t dropDelay, lastDrop = clock(), softDropDelay = CLOCKS_PER_SEC / 60, lastSoftDrop = clock(), softDropStartDelay = CLOCKS_PER_SEC / 60 * 3, softDropStart = clock();
clock_t lastFrame = clock(), frameDelay = CLOCKS_PER_SEC / 144.0;
clock_t lastDemoTap = clock(), demoTapDelay = CLOCKS_PER_SEC / 60 * 1;

int squareSize;
int nextFieldHeight = 4;
int windowWidth, windowHeight;
RenderWindow window;
RectangleShape square;
RectangleShape rw, cl;
int bodyColor, nextBodyColor;
int colors[10][3] =
{
	{ 252 r 252 g 252 b 255 ,  60 r 188 g 252 b 255, 0 r 88 g 248 b 255},
	{ 252 r 252 g 252 b 255 ,  184 r 248 g 24 b 255 ,  0 r 168 g 0 b 255 },
	{ 252 r 252 g 252 b 255 ,  248 r 120 g 248 b 255 ,  216 r 0 g 204 b 255 },
	{ 252 r 252 g 252 b 255 ,  88 r 216 g 84 b 255 ,  0 r 88 g 248 b 255 },
	{ 252 r 252 g 252 b 255 ,  88 r 248 g 152 b 255 ,  228 r 0 g 88 b 255 },

	{ 252 r 252 g 252 b 255 ,  104 r 136 g 252 b 255 ,  88 r 248 g 152 b 255 },
	{ 252 r 252 g 252 b 255 ,  124 r 124 g 124 b 255 ,  248 r 56 g 0 b 255 },
	{ 252 r 252 g 252 b 255 ,  168 r 0 g 32 b 255 ,  104 r 68 g 252 b 255 },
	{ 252 r 252 g 252 b 255 ,  248 r 56 g 0 b 255 ,  0 r 88 g 248 b 255 },
	{ 252 r 252 g 252 b 255 ,  252 r 160 g 68 b 255 ,  248 r 56 g 0 b 255 }
};
int typeColor[7] = {0, 0, 0, 1, 2, 2, 1};
//				0	1	2	3	4	5	6	7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29
int fpd[30] = {48, 43, 38, 33, 28, 23, 18, 13, 8, 6, 5, 5, 5, 4, 4, 4, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1};

enum Piece
{
	I = 0, O, T, L, J, S, Z, PIECES
};
char piecenames[PIECES] = { 'I', 'O', 'T', 'L', 'J', 'S', 'Z' };

void rotate(bool dir)
{
	int temp;
	Point p = Point(0, 0);
	if (dir)
	{
		for (int i = 0; i < 4; i++)
		{
			p = body[i];
			if (p.x * p.y > 0)
				p.y *= -1;
			else
			{
				if (p.x * p.y < 0)
					p.x *= -1;
				else if (p.y == 0)
					p.x *= -1;
				temp = p.x;
				p.x = p.y;
				p.y = temp;
			}
			if (pos.x + p.x < 0 || pos.x + p.x >= WIDTH || pos.y + p.y < 0)
				return;
			else if (pos.y + p.y < HEIGHT)
				if (field[pos.x + p.x][pos.y + p.y] != -1)
					return;
		}
	}
	else
	{
		for (int i = 0; i < 4; i++)
		{
			p = body[i];
			if (p.x * p.y > 0)
				p.x *= -1;
			else
			{
				if (p.x * p.y < 0)
					p.y *= -1;
				else if (p.x == 0)
					p.y *= -1;
				temp = p.x;
				p.x = p.y;
				p.y = temp;
			}
			if (pos.x + p.x < 0 || pos.x + p.x >= WIDTH || pos.y + p.y < 0)
				return;
			else if(pos.y + p.y < HEIGHT) 
				if(field[pos.x + p.x][pos.y + p.y] != -1)
					return;
		}
	}
	if(dir)
	{
		for (int i = 0; i < 4; i++)
		{
			if (body[i].x * body[i].y > 0)
			{
				body[i].y *= -1;
				continue;
			}
			else if (body[i].x * body[i].y < 0)
				body[i].x *= -1;
			else if (body[i].y == 0)
				body[i].x *= -1;
			temp = body[i].x;
			body[i].x = body[i].y;
			body[i].y = temp;
		}
	}
	else
	{
		for (int i = 0; i < 4; i++)
		{
			if (body[i].x * body[i].y > 0)
			{
				body[i].x *= -1;
				continue;
			}
			else if (body[i].x * body[i].y < 0)
				body[i].y *= -1;
			else if (body[i].x == 0)
				body[i].y *= -1;
			temp = body[i].x;
			body[i].x = body[i].y;
			body[i].y = temp;
		}
	}
	vertical = !vertical;
}
void rotate0(bool dir, bool isNext = false)
{
	int temp;
	Point p = Point(0, 0);
	Point* Body = (isNext ? demoTempBody : demoBody);
	if (dir)
	{
		for (int i = 0; i < 4; i++)
		{
			if (Body[i].x * Body[i].y > 0)
			{
				Body[i].y *= -1;
				continue;
			}
			else if (Body[i].x * Body[i].y < 0)
				Body[i].x *= -1;
			else if (Body[i].y == 0)
				Body[i].x *= -1;
			temp = Body[i].x;
			Body[i].x = Body[i].y;
			Body[i].y = temp;
		}
	}
	else
	{
		for (int i = 0; i < 4; i++)
		{
			if (Body[i].x * Body[i].y > 0)
			{
				Body[i].x *= -1;
				continue;
			}
			else if (Body[i].x * Body[i].y < 0)
				Body[i].y *= -1;
			else if (Body[i].x == 0)
				Body[i].y *= -1;
			temp = Body[i].x;
			Body[i].x = Body[i].y;
			Body[i].y = temp;
		}
	}
	(isNext ? demoTempVertical : demoVertical) = !demoVertical;
}

int getLv()
{
	return ((lines / 10) > startLv) ? lines / 10 : startLv;
}
void printScore()
{
	system("cls");
	printf("Score: %d Lines: %d Lvl: %d\n", score, lines, getLv());
	for (int i = 0; i < 4; i++)
		printf("%d: %d\n", i + 1, lineBatches[i]);
	for (int i = 0; i < PIECES; i++)
		printf("%c: %d\n", piecenames[i], pieces[i]);
}
void endGame()
{
	printScore();
	game = false;
}
void adjustPiece();
void generate()
{
	pos.x = WIDTH / 2;
	pos.y = HEIGHT - 1;
	type = nextType;
	pieces[type]++;
	vertical = type == I;
	for (int i = 0; i < 4; i++)
		body[i] = nextBody[i];
	bodyColor = nextBodyColor;
	switch (nextType = rand() % PIECES)
	{
	case I://I
		nextBody[0] = Point(0, 0);
		nextBody[1] = Point(1, 0);
		nextBody[2] = Point(-1, 0);
		nextBody[3] = Point(-2, 0);
		break;
	case O://O
		nextBody[0] = Point(0, 0);
		nextBody[1] = Point(0, -1);
		nextBody[2] = Point(-1, 0);
		nextBody[3] = Point(-1, -1);
		break;
	case T://T
		nextBody[0] = Point(0, 0);
		nextBody[1] = Point(-1, 0);
		nextBody[2] = Point(1, 0);
		nextBody[3] = Point(0, -1);
		break;
	case L://L
		nextBody[0] = Point(0, 0);
		nextBody[1] = Point(-1, 0);
		nextBody[2] = Point(1, 0);
		nextBody[3] = Point(-1, -1);
		break;
	case J://J
		nextBody[0] = Point(0, 0);
		nextBody[1] = Point(-1, 0);
		nextBody[2] = Point(1, 0);
		nextBody[3] = Point(1, -1);
		break;
	case S://S
		nextBody[0] = Point(0, 0);
		nextBody[1] = Point(1, 0);
		nextBody[2] = Point(0, -1);
		nextBody[3] = Point(-1, -1);
		break;
	case Z://Z
		nextBody[0] = Point(0, 0);
		nextBody[1] = Point(-1, 0);
		nextBody[2] = Point(0, -1);
		nextBody[3] = Point(1, -1);
		break;
	}
	for (int i = 0; i < 4; i++)
		if (pos.y + body[i].y < HEIGHT)
			if (field[pos.x + body[i].x][pos.y + body[i].y] != -1)
				endGame();
	nextBodyColor = typeColor[nextType];
	if (demo)
		adjustPiece();
}
/*
void hold()
{
	Point temp;
	int tmp;
	if (held)
		return;
	pos.x = WIDTH / 2;
	pos.y = HEIGHT - 1;
	for (int i = 0; i < 4; i++)
	{
		temp = body[i];
		body[i] = heldBody[i];
		heldBody[i] = temp;
	}

	tmp = type;
	type = heldType;
	heldType = tmp;

	tmp = bodyColor;
	bodyColor = heldBodyColor;
	heldBodyColor = tmp;
	held = true;
}
*/
void adjustDelay()
{
	dropDelay = CLOCKS_PER_SEC / 60 * fpd[(getLv() < 30) ? getLv(): 29];
}
void checkScore()
{
	int newLines = 0;
	bool line;
	for (int j = HEIGHT - 1; j >= 0; j--)
	{
		line = true;
		for (int i = 0; i < WIDTH; i++)
			if (field[i][j] == -1)
			{
				line = false;
				break;
			}
		if (line)
		{
			for (int i = 0; i < WIDTH; i++)
			{
				for (int l = j; l < HEIGHT - 1; l++)
					field[i][l] = field[i][l + 1];
				field[i][HEIGHT - 1] = -1;
			}
			newLines++;
		}
		if (newLines > 3)
			break;
	}
	if (newLines)
	{
		score += (getLv() + 1) * points[newLines - 1];
		lineBatches[newLines - 1]++;
		lines += newLines;
		adjustDelay();
	}
}
void set(bool val)
{
	for (int i = 0; i < 4; i++)
		if(pos.x + body[i].x >= 0 && pos.x + body[i].x < WIDTH && pos.y + body[i].y >= 0 && pos.y + body[i].y < HEIGHT)
			field[pos.x + body[i].x][pos.y + body[i].y] = bodyColor * val;
	checkScore();
	if(!timelapse)
		printScore();
	pieceDrops = 0;
}
bool isDropped()
{
	for (int i = 0; i < 4; i++)
		if (pos.y + body[i].y == 0)
			return true;
		else if(pos.y + body[i].y - 1 < HEIGHT)
			if(field[pos.x + body[i].x][pos.y + body[i].y - 1] != -1)
				return true;
	return false;
}
void drop()
{
	if (isDropped())
	{
		set(true);
		generate();
	}
	else
	{
		pos.y--;
		pieceDrops ++;
	}
}
void drop0(bool isNext = false)
{
	Point* Body = (isNext ? demoTempBody : demoBody);
	Point& Pos = (isNext ? demoTempPos : demoPos);
	for(int i = 0; i < HEIGHT; i++)
	{
		for (int i = 0; i < 4; i++)
			if (Pos.y + Body[i].y == 0)
				return;
			else if (Pos.y + Body[i].y < HEIGHT)
			{
				if (field[Pos.x + Body[i].x][Pos.y + Body[i].y - 1] != -1)
					return;
				if (isNext)
					for(int i1 = 0; i1 < 4; i1++)
					if (demoPos.x + demoBody[i].x == demoTempPos.x + demoTempBody[i1].x && demoPos.y + demoBody[i].y == demoTempPos.y + demoTempBody[i1].y - 1)
						return;
			}
		Pos.y--;
	}
}
void move(bool dir)
{
	if (dir)
	{
		for (int i = 0; i < 4; i++)
			if (pos.x + body[i].x == WIDTH - 1)
				return;
			else if(pos.y + body[i].y < HEIGHT)
				if(field[pos.x + body[i].x + 1][pos.y + body[i].y] != -1)
				return;
		pos.x++;
	}
	else
	{
		for (int i = 0; i < 4; i++)
			if (pos.x + body[i].x == 0)
				return;
			else if (pos.y + body[i].y < HEIGHT)
				if (field[pos.x + body[i].x - 1][pos.y + body[i].y] != -1)
					return;
		pos.x--;
	}
}
void move0(int val, bool isNext = false)
{
	Point* Body = (isNext ? demoTempBody : demoBody);
	Point& Pos = (isNext ? demoTempPos : demoPos);
	if (val > 0)
	{
		for (int j = 0; j < val; j++)
		{
			for (int i = 0; i < 4; i++)
				if (Pos.x + Body[i].x == WIDTH - 1)
					return;
				else if (Pos.y + Body[i].y < HEIGHT)
					if (field[Pos.x + Body[i].x + 1][Pos.y + Body[i].y] != -1)
						return;
			Pos.x++;
		}
	}
	else
	{
		for (int j = 0; j < -val; j++)
		{
			for (int i = 0; i < 4; i++)
				if (Pos.x + Body[i].x == 0)
					return;
				else if (Pos.y + Body[i].y < HEIGHT)
					if (field[Pos.x + Body[i].x - 1][Pos.y + Body[i].y] != -1)
						return;
			Pos.x--;
		}
	}
}

void print()
{
	int colorScheme = getLv() % 10;
	window.clear();
	for(int i = 0; i < WIDTH; i++)	//field
		for(int j = HEIGHT - 1; j >= 0; j--)
			if (field[i][j] != -1)
			{
				square.setFillColor(Color(colors[colorScheme][field[i][j]]));
				square.setPosition(squareSize * (i + 1), squareSize * (HEIGHT - j));
				square.setOutlineThickness(squareSize * -0.1 * !field[i][j]);
				square.setOutlineColor(Color(!field[i][j] * colors[colorScheme][2]));
				window.draw(square);
			}
	for (int i = 0; i < 4; i++)	//falling piece
	{
		square.setFillColor(Color(colors[colorScheme][bodyColor]));
		square.setPosition(squareSize * (body[i].x + pos.x + 1), squareSize * (HEIGHT - (body[i].y + pos.y)));
		square.setOutlineThickness(squareSize * -0.1 * !bodyColor);
		square.setOutlineColor(Color(!bodyColor * colors[colorScheme][2]));
		window.draw(square);
	}
	/*for (int i = 0; i < 4; i++)	//demo
	{
		square.setFillColor(Color::Red);
		square.setPosition(squareSize * (demoBody[i].x + demoPos.x + 1), squareSize * (HEIGHT - (demoBody[i].y + demoPos.y)));
		window.draw(square);
	}
	for (int i = 0; i < 4; i++)	//demonext
	{
		square.setFillColor(Color::Blue);
		square.setPosition(squareSize * (demoTempBody[i].x + demoTempPos.x + 1), squareSize * (HEIGHT - (demoTempBody[i].y + demoTempPos.y)));
		window.draw(square);
	}*/
	for (int i = 0; i < 4; i++)	// preview box
	{
		square.setFillColor(Color(colors[colorScheme][nextBodyColor]));
		square.setPosition(squareSize * (nextBody[i].x + (WIDTH + 1) / 2.0), squareSize * (HEIGHT + nextFieldHeight / 2.0 + 1 - nextBody[i].y));
		square.setOutlineThickness(squareSize * -0.1 * !nextBodyColor);
		square.setOutlineColor(Color(!nextBodyColor * colors[colorScheme][2]));
		window.draw(square);
	}
	cl.setPosition(0, 0);
	window.draw(cl);
	cl.setPosition(windowWidth - squareSize, 0);
	window.draw(cl);
	rw.setPosition(0, 0);
	window.draw(rw);
	rw.setPosition(0, windowHeight - squareSize);
	window.draw(rw);
	rw.setPosition(0, windowHeight - squareSize * (nextFieldHeight + 2));
	window.draw(rw);
	window.display();
}
Event e;
bool isAnyPresssed()
{
	return e.key.code == Keyboard::D || e.key.code == Keyboard::A || e.key.code == Keyboard::S || e.key.code == Keyboard::W || e.key.code == Keyboard::Space || e.key.code == Keyboard::LControl;
}
bool isDemoBody(int x, int y)
{
	for (int i = 0; i < 4; i++)
		if (demoPos.x + demoBody[i].x == x && demoPos.y + demoBody[i].y == y ||
			demoTempPos.x + demoTempBody[i].x == x && demoTempPos.y + demoTempBody[i].y == y)
			return true;
	return false;
}
//
float value()
{
	//holes.clear();
	bool temp1, temp2;
	int temp3;
	int minHeight = HEIGHT, maxHeight = -1, hls = 0, emptyColomns = 0;
	int colomns[WIDTH];
	int lines[4], linesCount = 0;

	for (int j = 0; j < HEIGHT; j++)
	{
		temp1 = true;
		for (int i = 0; i < WIDTH; i++)
		{
			temp1 = temp1 && (field[i][j] != -1 || isDemoBody(i, j));
		}
		if (temp1)
		{
			lines[linesCount] = j;
			linesCount++;
		}
	}

	for (int i = 0; i < WIDTH; i++)
	{
		colomns[i] = 0;
		temp1 = false;
		for (int j = HEIGHT - 1; j >= 0; j--)
		{
			temp2 = false;
			for (int k = 0; k < linesCount; k++)
				if (j == lines[k])
					temp2 = true;
			if (temp2)
				continue;
			if (field[i][j] != -1 || isDemoBody(i, j))
			{
				colomns[i] = j + 1;
				if (j + 1 > maxHeight)
					maxHeight = j + 1;
				if (j + 1 < minHeight)
					minHeight = j + 1;
				temp1 = true;
				break;
			}
		}
		if (!temp1)
		{
			colomns[i] = 0;
			minHeight = 0;
		}
	}

	for (int i = 0; i < WIDTH; i++)
	{
		temp3 = min(((i == 0) ? HEIGHT : colomns[i - 1]) - colomns[i],
			((i == WIDTH - 1) ? HEIGHT : colomns[i + 1]) - colomns[i]);
		if (temp3 > 1)
			emptyColomns += temp3;
	}

	for(int i = 0; i < WIDTH; i++)
		for (int j = 0; j < maxHeight; j++)
		{
			temp1 = false;
			for (int t = 0; t < linesCount; t++)
				if (j == lines[t])
					temp1 = true;
			if (temp1)
				continue;
			if (field[i][j] != -1 || isDemoBody(i, j))
				for (int k = j - 1; k >= 0; k--)
				{
					temp1 = false;
					for (int t = 0; t < linesCount; t++)
						if (k == lines[t])
							temp1 = true;
					if (temp1)
						continue;
					if (field[i][k] == -1 && !isDemoBody(i, k))
					{
						hls++;
						//holes.push_back(Point(i, k));
					}
					else
						break;
				}
		}
	return (hls) * 10.0 + (maxHeight - minHeight) * 3.0 + demoPos.y + emptyColomns * 2.0 - linesCount * 4;
}
int mov, rot;
void adjustPiece()
{
	int rotations = 2, nextRotations = 2;
	float minVal = 1000000, val;

	demoTempPos = Point(WIDTH + 10, HEIGHT + 10);
	for (int i = 0; i < 4; i++)
		demoBody[i] = body[i];
	demoVertical = type == I;

	if (type == O)
		rotations = 1;
	else if (type == J || type == L || type == T)
		rotations = 4;
	if (nextType == O)
		nextRotations = 1;
	else if (nextType == J || nextType == L || nextType == T)
		nextRotations = 4;

	for (int i = 0; i < rotations; i++)//rotations
	{
		for (int j = -WIDTH / 2; j < WIDTH / 2 + 1; j++)
		{
			demoPos.x = WIDTH / 2;
			demoPos.y = HEIGHT - 1;
			move0(j);
			drop0();
			//
			for (int i1 = 0; i1 < 4; i1++)
				demoTempBody[i1] = nextBody[i1];
			demoTempVertical = nextType == I;
			for (int i1 = 0; i1 < nextRotations; i1++)//rotations
			{
				for (int j1 = -WIDTH / 2; j1 < WIDTH / 2 + 1; j1++)
				{
					demoTempPos.x = WIDTH / 2;
					demoTempPos.y = HEIGHT - 1;
					move0(j1, true);
					drop0(true);
					val = value();
					//print();
					if (val < minVal)
					{
						minVal = val;
						mov = j;
						rot = i;
						//printf("%f\n", val);
					}
					/*Event ev;
					do
						window.pollEvent(ev);
					while (ev.type != Event::MouseWheelMoved);*/
				}
				if (nextType == J || nextType == L || nextType == T)
					rotate0(true, true);
				else
					rotate0(demoTempVertical, true);
			}
		}
		if (type == J || type == L || type == T)
			rotate0(true);
		else
			rotate0(demoVertical);
	}
}
//
int d()
{
	
	squareSize = VideoMode::getDesktopMode().height / (HEIGHT + nextFieldHeight + 3) / 10 * 10;
	windowWidth = squareSize * (WIDTH + 2), windowHeight = squareSize * (HEIGHT + nextFieldHeight + 3);
	square.setSize(Vector2f(squareSize * 0.9, squareSize * 0.9));
	rw.setSize(Vector2f(windowWidth, squareSize));
	cl.setSize(Vector2f(squareSize, windowHeight));
	window.create(VideoMode(windowWidth, windowHeight), "Tetriz");

	square.setFillColor(Color::Red);
	cl.setFillColor(Color::Black);
	rw.setFillColor(Color::Black);
	cl.setOutlineThickness(-1);
	rw.setOutlineThickness(-1);
	cl.setOutlineColor(Color::White);
	rw.setOutlineColor(Color::White);

	window.setVerticalSyncEnabled(false);
	window.setKeyRepeatEnabled(false);
	window.clear();
	time_t seed;
	ofstream out;
	ifstream in;
	char filename[50];

	while (window.isOpen())
	{
		for (int i = 0; i < WIDTH; i++)
			for (int j = 0; j < HEIGHT; j++)
				field[i][j] = -1;
		seed = unsigned(time(0));
		window.setVisible(false);
		printf("Hold F1 to start demo,\nF2 to start demo from file\nStart with level: ");
		scanf_s("%d", &startLv);
		if (Keyboard::isKeyPressed(Keyboard::F3))
			timelapse = true;
		if (Keyboard::isKeyPressed(Keyboard::F1))
		{
			demo = true;
			do
			{
				printf("Enter save file name: ");
				cin >> filename;
				out.open(filename, ios::binary);
			} while (!out.is_open());
			out.write((char *)&seed, sizeof(seed));
			//out.write((char *)&startLv, sizeof(startLv));
			out.close();
		}
		else if (Keyboard::isKeyPressed(Keyboard::F2))
		{
			demo = true;
			do
			{
				printf("Enter save file name: ");
				cin >> filename;
				in.open(filename, ios::binary);
			} while (!in.is_open());
			in.read((char*)&seed, sizeof(seed));
			//in.read((char*)&startLv, sizeof(startLv));
			in.close();
		}
		srand(seed);
		adjustDelay();
		window.setVisible(true);
		window.requestFocus();
		generate(); 
		generate();

		while (game)
		{
			if (window.pollEvent(e))
			{
				if (e.type == Event::Closed)
				{
					game = false;
					window.close();
				}
				//
				if (!demo)
				{
					if (e.type == Event::KeyPressed)
					{
						if (!softDropAllowed)
						{
							switch (e.key.code)
							{
							case Keyboard::Num0:
								demo = true;
								break;
							case Keyboard::Q:
								window.close();
								return 0;
								break;
							case Keyboard::W:
								if (type == O)
									break;
								if (type == J || type == L || type == T)
									rotate(true);
								else
									rotate(vertical);
								break;
							case Keyboard::S:
								if (type == O)
									break;
								if (type == J || type == L || type == T)
									rotate(false);
								else
									rotate(vertical);
								break;
							case Keyboard::A:
								move(false);
								if (dasDir == 0)
								{
									dasStart = clock();
									dasDir = -1;
								}
								break;
							case Keyboard::D:
								move(true);
								if (dasDir == 0)
								{
									dasStart = clock();
									dasDir = 1;
								}
								break;
							case Keyboard::Space:
								softDropStart = clock();
								softDropAllowed = true;
								break;
							}
						}
						print();
					}
					if (e.type == Event::KeyReleased)
						switch (e.key.code)
						{
						case Keyboard::Space:
							softDropAllowed = false;
							break;
						case Keyboard::A:
							dasDir = 0;
							break;
						case Keyboard::D:
							dasDir = 0;
							break;
						}
				}
			}
			if (clock() - lastFrame > frameDelay)
			{
				lastFrame = clock();
				print();
			}
			if (demo)
			{
				if (demoTapDelay == 0 || timelapse)
				{
					for (int i = 0; i < rot; i++)
						if (type != O && rot > 0)
						{
							if (type == J || type == L || type == T)
								for (int i = 0; i < rot; i++)
									rotate(true);
							else
								rotate(vertical);
							rot = -1;
						}
					for(int i = 0; i < WIDTH / 2; i++)
					if (abs(pos.x - WIDTH / 2) < abs(mov))
						move(mov > 0);
				}
				else if (clock() - lastDemoTap > demoTapDelay)
				{
					if (type != O && rot > 0)
					{
						if (type == J || type == L || type == T)
							for (int i = 0; i < rot; i++)
								rotate(true);
						else
							rotate(vertical);
						rot = -1;
					}
					if (abs(pos.x - WIDTH / 2) < abs(mov))
						move(mov > 0);
					lastDemoTap = clock();
				}
			}
			if (clock() - dasStart > dasStartDelay && clock() - lastDas > dasDelay)
			{
				if (dasDir == -1 && Keyboard::isKeyPressed(Keyboard::A))
				{
					move(false);
				}
				else if (dasDir == 1 && Keyboard::isKeyPressed(Keyboard::D))
				{
					move(true);
				}
				lastDas = clock();
			}
			if (softDropAllowed && Keyboard::isKeyPressed(Keyboard::Space) && clock() - softDropStart > softDropStartDelay
				&& (clock() - lastSoftDrop > softDropDelay) && !isAnyPresssed())
			{
				score++;
				drop();
				lastSoftDrop = clock();
			}
			else if (timelapse || lastDrop + dropDelay < clock())
			{
				drop();
				lastDrop = clock();
			}
		}
		print();
		while (!game)
		{
			if (window.pollEvent(e))
			{
				if (e.type == Event::KeyPressed)
				{
					switch (e.key.code)
					{
					case Keyboard::Q:
						return 0;
					case Keyboard::R:
						game = true;
						demo = false;
						timelapse = false;
						for (int i = 0; i < WIDTH; i++)
							for (int j = 0; j < HEIGHT; j++)
								field[i][j] = -1;
						for (int i = 0; i < 4; i++)
							lineBatches[i] = 0;
						for (int i = 0; i < PIECES; i++)
							pieces[i] = 0;
						score = lines = 0;
						system("cls");
						break;
					}
				}
			}
		}
		
	}
}
