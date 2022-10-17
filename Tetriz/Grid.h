#pragma once
#include "stdafx.h"
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>

#include <SFML/Graphics.hpp>
using namespace sf;
using namespace std;

#define WIDTH 10
#define HEIGHT 20
#define r <<24 |
#define g <<16 |
#define b <<8 |
#define ULL unsigned long long
#define LL long long
//<GLOBAL VARS>
const static int TetroNumber = 4;
const static enum Piece
{
	I = 0, O, T, L, J, S, Z, PIECES
};
const static clock_t 
/*dasDelay = CLOCKS_PER_SEC / 60 * 6,
dasStartDelay = CLOCKS_PER_SEC / 60 * 16,*/
frameDelay = CLOCKS_PER_SEC / 144.0,
tickDelay = CLOCKS_PER_SEC / 60.0,
demoTapDelay = CLOCKS_PER_SEC / 60 * 1;
const static int sidebarSize = 6;
//							0	1	2	3	4	5	6	7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29
const static int tpd[30] = { 48, 43, 38, 33, 28, 23, 18, 13, 8, 6, 5, 5, 5, 4, 4, 4, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1 };

const static int colors[10][3] =
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
const static int typeColors[7] = { 0, 0, 0, 1, 2, 2, 1 };

const static enum TetrominosObjects
{
	CURRENT = 0, NEXT, HELD, TOTAL_TETROMINOS_OBJECTS
}; 
const static enum AI_TetrominosObjects
{
	AICURRENT = 0, AINEXT, AIHELD, TOTAL_AI_TETROMINOS_OBJECTS
};
const static char piecenames[PIECES] = { 'I', 'O', 'T', 'L', 'J', 'S', 'Z' };
//</GLOBALVARS>
class Tetromino
{
public:
	int type, color = 0, moved = 0, rotated = 0;
	vector<Vector2i> body;
	Vector2i pos;
	bool vertical = false, solid;
	void addRotation(bool dir)
	{
		if (dir)
			rotated++;
		else
			rotated--;
		rotated = (rotated + maxRotations()) % maxRotations();
	}
	void addMove(int offset)
	{
		moved += offset;
	}
	void defaultPos()
	{
		pos.y = HEIGHT - 1;
		pos.x = WIDTH / 2;
		solid = false;
		moved = 0;
	}
	Tetromino(const Tetromino& t)
	{
		copy(t);
	}
	Tetromino(const Tetromino&& t)
	{
		copy(t);
	}
	void operator=(Tetromino& right)
	{
		this->copy(right);
	}
	void operator=(Tetromino&& right)
	{
		this->copy(right);
	}
	void copy(const Tetromino& t)
	{
		type = t.type;
		color = t.color;
		moved = t.moved;
		rotated = t.rotated;
		vertical = t.vertical;
		solid = t.solid;
		pos = t.pos;
		body = t.body;
	}
	Tetromino (int newType = -1)
	{
		body.assign(TetroNumber, Vector2i());
		solid = false;
		if (newType == -1)
			type = PIECES;
		else
		{
			type = newType;
		}
		color = typeColors[type];
		defaultPos();
		vertical = type == I;
		switch (type)
		{
		case I://I
			body[0] = Vector2i(0, 0);
			body[1] = Vector2i(1, 0);
			body[2] = Vector2i(-1, 0);
			body[3] = Vector2i(-2, 0);
			break;
		case O://O
			body[0] = Vector2i(0, 0);
			body[1] = Vector2i(0, -1);
			body[2] = Vector2i(-1, 0);
			body[3] = Vector2i(-1, -1);
			break;
		case T://T
			body[0] = Vector2i(0, 0);
			body[1] = Vector2i(-1, 0);
			body[2] = Vector2i(1, 0);
			body[3] = Vector2i(0, -1);
			break;
		case L://L
			body[0] = Vector2i(0, 0);
			body[1] = Vector2i(-1, 0);
			body[2] = Vector2i(1, 0);
			body[3] = Vector2i(-1, -1);
			break;
		case J://J
			body[0] = Vector2i(0, 0);
			body[1] = Vector2i(-1, 0);
			body[2] = Vector2i(1, 0);
			body[3] = Vector2i(1, -1);
			break;
		case S://S
			body[0] = Vector2i(0, 0);
			body[1] = Vector2i(1, 0);
			body[2] = Vector2i(0, -1);
			body[3] = Vector2i(-1, -1);
			break;
		case Z://Z
			body[0] = Vector2i(0, 0);
			body[1] = Vector2i(-1, 0);
			body[2] = Vector2i(0, -1);
			body[3] = Vector2i(1, -1);
			break;
		}
	}
	bool contains(Vector2i point)
	{
		for (int i = 0; i < TetroNumber; i++)
			if (body[i] + pos == point)
				return true;
		return
			false;
	}
	bool containsX(int x)
	{
		for (int i = 0; i < TetroNumber; i++)
			if (body[i].x + pos.x == x)
				return true;
		return false;
	}
	bool containsY(int y)
	{
		for (int i = 0; i < TetroNumber; i++)
			if (body[i].y + pos.y == y)
				return true;
		return false;
	}
	int maxRotations()
	{
		if (type == Z || type == S || type == I)
			return 2;
		else if (type == O)
			return 1;
		return 4;
	}
	static void swap(Tetromino& first, Tetromino& second)
	{
		std::swap(first.type, second.type);
		std::swap(first.color, second.color);
		std::swap(first.moved, second.moved);
		std::swap(first.rotated, second.rotated);
		std::swap(first.vertical, second.vertical);
		std::swap(first.solid, second.solid);
		std::swap(first.pos, second.pos);
		first.body.swap(second.body);
	}
};

class Player
{
public:
	enum Attributes
	{
		HOLES = 0,
		PILLARS,
		LINES_BURN,
		BUMPINESS,
		BLOCKS_ABOVE_HOLES,
		COL_10_BLOCKS,
		CRITICAL_STACK_HEIGHT_PERCENT,
		TETRIS,
		TETRIS_READY,
		ATTRIBUTES
	};
	//vector<float> attributes = { 31.5689, 0.471205, 12.3467, 4.83297, 24.5056, 12.8792, 43.8853, 996.243, 8.88312 };
	vector<float> attributes = { 2.1548, 0.6483, 2.7595, 0.4041, 0.3602, 2.435, 50, 3.0836, 0.9374 };
	
	const static ULL MAX_ATTRIBUTE_VAL = 100;

	ULL
		SCORE = 0, burns = 0;
	float 
		TTR = 0;
	const static int cutOffPercent = 10;
	Player() {}
	Player(float holes, float pillars, float linesBurn, float bumpiness, float blocksAboveHoles, float colomn_10_blocks, float criticalStackHeightPercent, float tetris, float tetrisReady)
	{
		attributes[HOLES] = holes;
		attributes[PILLARS] = pillars;
		attributes[LINES_BURN] = linesBurn;
		attributes[BUMPINESS] = bumpiness;
		attributes[BLOCKS_ABOVE_HOLES] = blocksAboveHoles;
		attributes[COL_10_BLOCKS] = colomn_10_blocks;
		attributes[CRITICAL_STACK_HEIGHT_PERCENT] = criticalStackHeightPercent;
		attributes[TETRIS] = tetris;
		attributes[TETRIS_READY] = tetrisReady;
	}
	float cost(LL holes, LL pillars, LL linesCleared, LL bumpiness, LL blocksAboveHoles, LL colomn_10_blocks, LL maxStackHeight, int tetrisReady, int tetrisFound)
	{
		return
		attributes[HOLES] * holes
		+ attributes[PILLARS] * pillars
		+ attributes[LINES_BURN] * linesCleared * (!tetrisFound && !tetrisReady) * ((attributes[CRITICAL_STACK_HEIGHT_PERCENT] / 100.0 * HEIGHT < maxStackHeight) ? -1 : 1)
		+ attributes[BUMPINESS] * bumpiness
		+ attributes[BLOCKS_ABOVE_HOLES] * blocksAboveHoles
		+ attributes[COL_10_BLOCKS] * colomn_10_blocks * (holes == 0)
		- attributes[TETRIS] * tetrisFound
		- attributes[TETRIS_READY] * (tetrisReady > 0)
		;
	}
	static float fitness(ULL score, float ttr)
	{
		return score * ttr;//score / lines * ttr?
	}
	static void resetPlayersStats(vector<Player>& players)
	{
		for (int player = 0; player < players.size(); player++)
		{
			players[player].SCORE = 0;
			players[player].TTR = 0;
		}
	}
	float fitness()
	{
		return fitness(SCORE, TTR);
	}
	static float generateRandomFloat()
	{
		return float((rand() % (2 * 10000)) - 10000) / 10000;
	}
	static void adjustAttributes(vector<Player> &players)
	{
		int best = players.size() * cutOffPercent / 100;
		for (int player = players.size() / 2; player < players.size(); player++)
		{
			for (int attribute = 0; attribute < ATTRIBUTES; attribute++)
			{
				players[player].attributes[attribute] += generateRandomFloat();
				if (players[player].attributes[attribute] < 0)
					players[player].attributes[attribute] = 0;
				if (players[player].attributes[attribute] > MAX_ATTRIBUTE_VAL)
					players[player].attributes[attribute] = MAX_ATTRIBUTE_VAL;
			}
		}
	}
	static void sortPlayers(vector<Player>& players)
	{
		float max;
		int maxIndex;
		for (int i = 0; i < players.size(); i++)
		{
			max = 0;
			maxIndex = i;
			for (int j = i; j < players.size(); j++)
			{
				if (players[j].fitness() > max)
				{
					max = players[j].fitness();
					maxIndex = j;
				}
			}
			swap(players[i], players[maxIndex]);
		}
		int parents = players.size() * cutOffPercent / 100;
		//auto newPlayers = players;
		for (int i = parents; i < players.size(); i++)
			players[i] = players[i % parents];//newPlayers[i].mergePlayers(players[i % parents], players[(i + 1) % parents]);
		//players = newPlayers;
	}
	static void randomizePlayers(vector<Player>& players)
	{
		for (int i = 0; i < players.size(); i++)
		{
			for (int j = 0; j < Player::ATTRIBUTES; j++)
				players[i].attributes[j] = (rand() % 1000000) / 10000.0;
		}
	}
	void mergePlayers(Player& one, Player& two)
	{
		for (int i = 0; i < ATTRIBUTES; i++)
			attributes[i] = ((rand() % 2) ? one : two).attributes[i];
	}
};

class Grid
{
public:
	Player *player;
	Tetromino tetrominos[TOTAL_TETROMINOS_OBJECTS], aiTetrominos[TOTAL_AI_TETROMINOS_OBJECTS];
	Tetromino &current = tetrominos[CURRENT], &next = tetrominos[NEXT];
	Tetromino held;
	bool game = true, timelapse = false, isAI = true, holdUsed = false;

	int field[WIDTH][HEIGHT] = {};
	int points[TetroNumber] = { 40, 100, 300, 1200 };
	ULL lineBatches[TetroNumber] = {};
	ULL pieces[7] = {};
	ULL lines = 0, score = 0;
	int startLv, ticksPerDrop, ticks = 0;
	bool adjustable = false;

	clock_t lastDas = 0, dasStart = 0;
	clock_t lastFrame = 0, lastTick = 0;
	clock_t lastDemoTap = 0;

	int squareSize;
	int windowWidth, windowHeight;

	RenderWindow window;
	RectangleShape square;
	RectangleShape rw, cl;
	Font font;
	Text text;

	//
	Grid()
	{
		squareSize = VideoMode::getDesktopMode().height / (HEIGHT + 2) / 10 * 10;
		windowWidth = squareSize * (WIDTH + 2 + sidebarSize), windowHeight = squareSize * (HEIGHT + 2);
		square.setSize(Vector2f(squareSize * 0.9, squareSize * 0.9));
		rw.setSize(Vector2f(windowWidth, squareSize));
		cl.setSize(Vector2f(squareSize, windowHeight));
		window.create(VideoMode(windowWidth, windowHeight), "T");
		for (int i = 0; i < WIDTH; i++)
			for (int j = 0; j < HEIGHT; j++)
				field[i][j] = -1;
		adjustDelay();
		generate();
		generate();
		//game = true;
		//timelapse = false;
		if (!font.loadFromFile("arial.ttf"))
			exit(-9999);
		text.setCharacterSize(24);
		text.setFont(font);
		text.setFillColor(Color::Black);
		text.setPosition(windowWidth / 2, squareSize / 2);
	}
	void handleEvent()
	{
		Event ev;
		while(window.pollEvent(ev))
			if (ev.type == Event::KeyPressed)
			{
				switch (ev.key.code)
				{
				case Keyboard::A:
					move(-1);
					break;
				case Keyboard::D:
					move(1);
					break;
				case Keyboard::W:
					rotate(true);
					break;
				case Keyboard::S:
					rotate(false);
					break;
				case Keyboard::Num0:
					isAI = !isAI;
					break;
				case Keyboard::T:
					toggleTimelapse();
					break;
				case Keyboard::LControl:
					hold();
					break;
				}
				print();
			}
	}

	void toggleTimelapse() { timelapse = !timelapse; }
	
	bool noCollisions(Tetromino& t)
	{
		bool result = true;
		for (int i = 0; i < TetroNumber; i++)
		{
			if (t.pos.x + t.body[i].x < 0 || t.pos.x + t.body[i].x >= WIDTH || t.pos.y + t.body[i].y < 0)
				result = false;
			else if (t.pos.y + t.body[i].y < HEIGHT)
				if (isSolid(t.pos.x + t.body[i].x, t.pos.y + t.body[i].y))
					result = false;
		}
		return result;
	}

	void rotateTetromino(bool dir, Tetromino& t)
	{
		for (int i = 0; i < TetroNumber; i++)
		{
			if (t.body[i].x * t.body[i].y > 0)
			{
				(dir ? t.body[i].y : t.body[i].x) *= -1;
				continue;
			}
			else if (t.body[i].x * t.body[i].y < 0)
				(dir ? t.body[i].x : t.body[i].y) *= -1;
			else if ((dir ? t.body[i].y : t.body[i].x) == 0)
				(dir ? t.body[i].x : t.body[i].y) *= -1;
			swap(t.body[i].x, t.body[i].y);
		}
	}

	void hold()
	{
		if (holdUsed)
			return;
		holdUsed = true;
		swap(current, held);
		if (current.type == PIECES)
			generate();
		current.defaultPos();
	}

	void rotate(bool dir, Tetromino* ptr = nullptr)
	{
		bool currentPiece = false;
		if (ptr == nullptr)
		{
			ptr = &current;
			currentPiece = true;
		}
		Tetromino& t = *ptr;
		if (t.type == O)
			return;
		if(t.type == Z || t.type == S || t.type == I)
			dir = t.vertical;
		rotateTetromino(dir, t);
		if (!noCollisions(t))
		{
			rotateTetromino(!dir, t);
		}
		else
		{
			t.vertical = !t.vertical;
			t.addRotation(dir);
		}
	}

	void move(int offset, Tetromino* ptr = nullptr)
	{
		if (ptr == nullptr)
			ptr = &current;
		Tetromino& t = *ptr;
		for (int i = 0, step = (offset > 0 ? 1 : -1); i < abs(offset); i++)
		{
			t.pos.x += step;
			if (!noCollisions(t))
			{
				t.pos.x -= step;
				return;
			}
			else
				t.addMove(step);
		}
	}

	int getLv() { return ((lines / 10) > startLv) ? lines / 10 : startLv; }

	void printScore()
	{
		string message;
		message = "Score: " + to_string(score) + " Lines: " + to_string(lines) + " Lvl: " + to_string(getLv()) + " TTR: " + to_string(int(getTTR() * 100)) + '\n';
		text.setString(message);
		text.setOrigin(text.getGlobalBounds().width / 2, text.getGlobalBounds().height / 2);
	}

	void endGame()
	{
		printScore();
		game = false;
	}

	void generate()
	{
		current = next;
		pieces[current.type]++;
		next = Tetromino(rand() % PIECES);
		adjustable = true;
	}

	void adjustDelay() { ticksPerDrop = tpd[(getLv() < 30) ? getLv() : 29]; }

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
			if (newLines == TetroNumber)
				break;
		}
		if (newLines)
		{
			score += (getLv() + 1) * points[newLines - 1];
			lineBatches[newLines - 1]++;
			lines += newLines;
			if (newLines != TetroNumber)
				player->burns += newLines;
			else
				player->burns = 0;
			if (player->burns > 200)
				endGame();
			adjustDelay();
		}
	}

	void set(bool val)
	{
		for (int i = 0; i < TetroNumber; i++)
			if (current.pos.x + current.body[i].x >= 0 && current.pos.x + current.body[i].x < WIDTH && current.pos.y + current.body[i].y >= 0 && current.pos.y + current.body[i].y < HEIGHT)
				field[current.pos.x + current.body[i].x][current.pos.y + current.body[i].y] = current.color * val;
		checkScore();
		printScore();
		holdUsed = false;
	}

	void print()
	{
		if (clock() - lastFrame < frameDelay)
			return;
		lastFrame = clock();
		int colorScheme = getLv() % 10;
		window.clear();
		for (int i = 0; i < WIDTH; i++)	//field
			for (int j = HEIGHT - 1; j >= 0; j--)
				if (field[i][j] != -1)
				{
					square.setFillColor(Color(colors[colorScheme][field[i][j]]));
					square.setPosition(squareSize * (i + 1), squareSize * (HEIGHT - j));
					square.setOutlineThickness(squareSize * -0.1 * !field[i][j]);
					square.setOutlineColor(Color(!field[i][j] * colors[colorScheme][2]));
					window.draw(square);
				}
		for (int i = 0; i < 4; i++)	//current
		{
			square.setFillColor(Color(colors[colorScheme][current.color]));
			square.setPosition(squareSize * (current.body[i].x + current.pos.x + 1), squareSize * (HEIGHT - (current.body[i].y + current.pos.y)));
			square.setOutlineThickness(squareSize * -0.1 * !current.color);
			square.setOutlineColor(Color(!current.color * colors[colorScheme][2]));
			window.draw(square);
		}
		for (int i = 0; i < TOTAL_AI_TETROMINOS_OBJECTS; i++)
		{
			for (int j = 0; j < TetroNumber; j++)
			{
				if (i == 0)
					square.setOutlineColor(Color::Red);
				if (i == 1)
					square.setOutlineColor(Color::Green);
				if (i == 2)
					square.setOutlineColor(Color::Blue);
				square.setFillColor(Color(64, 64, 64));
				if (j == 0)
					square.setFillColor(Color::Black);
				square.setOutlineThickness(squareSize * -0.1);
				square.setPosition(squareSize * (aiTetrominos[i].body[j].x + aiTetrominos[i].pos.x + 1), squareSize * (HEIGHT - (aiTetrominos[i].body[j].y + aiTetrominos[i].pos.y)));
				window.draw(square);
				square.setOutlineColor(Color::Black);
			}
		}
		for (int i = 0; i < 4; i++)	// next
		{
			square.setFillColor(Color(colors[colorScheme][next.color]));
			square.setPosition(squareSize * (next.body[i].x + WIDTH + 1 + (sidebarSize / 2.0)), squareSize * (HEIGHT - sidebarSize - next.body[i].y));
			square.setOutlineThickness(squareSize * -0.1 * !next.color);
			square.setOutlineColor(Color(!next.color * colors[colorScheme][2]));
			window.draw(square);
		}
		if(held.type < PIECES)
			for (int i = 0; i < 4; i++)	// held
			{
				square.setFillColor(Color(colors[colorScheme][held.color]));
				square.setPosition(squareSize * (held.body[i].x + WIDTH + 1 + (sidebarSize / 2.0)), squareSize * (sidebarSize - held.body[i].y));
				square.setOutlineThickness(squareSize * -0.1 * !held.color);
				square.setOutlineColor(Color(!held.color * colors[colorScheme][2]));
				window.draw(square);
			}
		cl.setPosition(0, 0);
		window.draw(cl);
		cl.setPosition(windowWidth - squareSize, 0);
		window.draw(cl);
		cl.setPosition(squareSize * (WIDTH + 1), 0);
		window.draw(cl);
		rw.setPosition(0, 0);
		window.draw(rw);
		rw.setPosition(0, windowHeight - squareSize);
		window.draw(rw);
		window.draw(text);
		window.display();
	}

	bool isDropped(Tetromino& t)
	{
		for (int i = 0; i < 4; i++)
			if (t.pos.y + t.body[i].y == 0)
				return true;
			else if (t.pos.y + t.body[i].y - 1 < HEIGHT)
				if (isSolid(t.pos.x + t.body[i].x, t.pos.y + t.body[i].y - 1))
				{
					return true;
				}
		return false;
	}

	bool drop(Tetromino* ptr = nullptr)
	{
		bool currentPiece = false;
		if (ptr == nullptr)
		{
			ptr = &current;
			currentPiece = true;
		}
		Tetromino& t = *ptr;
		if (isDropped(t))
		{
			if (currentPiece)
			{
				set(true);
				generate();
				for (int i = 0; i < 4; i++)
					if (current.pos.y + current.body[i].y < HEIGHT)
						if (field[current.pos.x + current.body[i].x][current.pos.y + current.body[i].y] != -1)
							endGame();
			}
			else
			{
				t.solid = true;
			}
			return false;
		}
		if (isAI && adjustable && currentPiece)
			calculateMove();
		t.pos.y--;
		return true;
	}

	bool isSolid(int x, int y)
	{
		if (field[x][y] != -1)
			return true;
		for (int i = 0; i < TOTAL_AI_TETROMINOS_OBJECTS; i++)
			if (aiTetrominos[i].solid && aiTetrominos[i].contains(Vector2i(x, y)))
				return true;
		return false;
	}

	void tick()
	{
		ticks = (ticks + 1) % ticksPerDrop;
		if (!ticks)
		{
			drop();
			print();
		}
	}

	float getTTR()
	{
		if (lines == 0)
			return 0;
		return float(lineBatches[3] * 4) / float(lines);
	}

	void play(Player* pl, int startLvl = 18)
	{
		player = pl;
		player->burns = 0;
		startLv = startLvl;
		score = 0;
		lines = 0;
		for (int i = 0; i < WIDTH; i++)
			for (int j = 0; j < HEIGHT; j++)
				field[i][j] = -1;
		for (int i = 0; i < PIECES; i++)
			pieces[i] = 0;
		for (int i = 0; i < TetroNumber; i++)
			lineBatches[i] = 0;
		adjustDelay();
		generate();
		generate();
		game = true;
		Event ev;
		while (game)
		{
			handleEvent();
			if (timelapse)
			{
				while(drop());
				print();
			}
			if (clock() - lastTick > tickDelay)
			{
				lastTick = clock();
				tick();
				//if(!timelapse)
					print();
			}
		}
		float ttr = getTTR();
		
		if (player->fitness() > 0)
		{
			if (player->fitness() > Player::fitness(score, ttr))
			{
				player->SCORE = score;
				player->TTR = ttr;
			}
		}
		else
		{
			player->SCORE = score;
			player->TTR = ttr;
		}
	}

	bool contains(vector<int> v, int val)
	{
		for (int i = 0; i < v.size(); i++)
			if (v[i] == val)
				return true;
		return false;
	}

	float costFunction(Tetromino& first, Tetromino& second)
	{
		Tetromino& aiCurrent = first, & aiNext = second;

		int temp, clearedCurrent = 0, clearedNext = 0;
		int MAX_STACK_HEIGHT = -1, MIN_STACK_HEIGHT = HEIGHT, HOLES = 0, PILLARS = 0, CLEARED_LINES, BUMPINESS = 0, BLOCKS_ABOVE_HOLES = 0, COL_10_BLOCKS = 0, TETRIS_FOUND = 0, TETRIS_READY = 0;
		vector<int> colomns(WIDTH);
		vector<int> clearedLines;
		int x, y;

		for (y = 0; y < HEIGHT; y++)
		{
			for (x = 0; x < WIDTH; x++)
			{
				if (!isSolid(x, y))
				{
					x = -1;
					break;
				}
			}
			if (x != -1)
				clearedLines.push_back(y);
		}
		CLEARED_LINES = clearedLines.size();

		for (x = 0; x < WIDTH; x++)
		{
			for (y = HEIGHT - 1; y >= 0; y--)
			{
				if (contains(clearedLines, y))
					continue;
				if (isSolid(x, y))
					break;
			}
			colomns[x] = y + 1;
		}
		for (y = 0; y < colomns.back(); y++)
			if (!contains(clearedLines, y) && isSolid(WIDTH - 1, y))
				COL_10_BLOCKS++;
		for (x = 0; x < WIDTH; x++)
		{
			if (colomns[x] > MAX_STACK_HEIGHT)
				MAX_STACK_HEIGHT = colomns[x];
			if (colomns[x] < MIN_STACK_HEIGHT)
				MIN_STACK_HEIGHT = colomns[x];
		}
		for (x = 1; x < WIDTH - 1; x++)//(COL_10_BLOCKS == 0)
			BUMPINESS += abs(colomns[x] - colomns[x - 1]);

		for (x = WIDTH - 1; x >= 0; x--)
		{
			if (!isSolid(x, MIN_STACK_HEIGHT))
			{
				temp = x;
				break;
			}
		}

		x = 0, y = MIN_STACK_HEIGHT;
		while (y < HEIGHT && isSolid(x, y) ^ (x == temp))
		{
			x++;
			if (x == WIDTH)
			{
				x = 0;
				y++;
			}
		}
		TETRIS_READY = (y - MIN_STACK_HEIGHT) / TetroNumber;
		
		if (CLEARED_LINES >= TetroNumber && (aiCurrent.type == I || aiNext.type == I))
		{
			for (int i = 0; i < clearedLines.size(); i++)
			{
				if (aiCurrent.containsY(clearedLines[i]))
					clearedCurrent++;
				if (aiNext.containsY(clearedLines[i]))
					clearedNext++;
			}
			if (clearedCurrent == TetroNumber)
			{
				TETRIS_FOUND++;
			}
			if (clearedNext == TetroNumber)
			{
				TETRIS_FOUND++;
			}
		}
		for (int x = 0; x < WIDTH - 1; x++)//(COL_10_BLOCKS == 0)
		{
			temp = min(((x == 0) ? HEIGHT : colomns[x - 1]) - colomns[x],
				((x == WIDTH - 1) ? HEIGHT : colomns[x + 1]) - colomns[x]);
			if (temp > 2)
				PILLARS += temp;
		}

		for (int x = 0; x < WIDTH; x++)
		{
			y = 0;
			while (contains(clearedLines, y) || isSolid(x, y))
				y++;
			for (; y < colomns[x]; y++)
			{
				if (contains(clearedLines, y))
					continue;
				if (isSolid(x, y))
					BLOCKS_ABOVE_HOLES++;
				else
					HOLES++;
			}
		}
		//if(TETRIS_FOUND)
		//	cout << "tetris found " << TETRIS_FOUND << endl;
		//if(TETRIS_READY)
		//	cout << "tetris ready " << TETRIS_READY << endl;
		////if (TETRIS_READY)
		//
		//cout << "max height " << MAX_STACK_HEIGHT << endl;
		//cout << "col 10 blocks " << COL_10_BLOCKS << endl;
		//cout << "cleared lines " << CLEARED_LINES << endl;
		//cout << "pillars " << PILLARS << endl;
		//cout << "holes " << HOLES << " blocks above " << BLOCKS_ABOVE_HOLES << endl;
		//cout << "bump " << BUMPINESS << endl << endl;
		/*for (int i = 0; i < WIDTH; i++)
			cout << colomns[i] << ' ';
		cout << endl;*/
		return player->cost(HOLES, PILLARS, CLEARED_LINES, BUMPINESS, BLOCKS_ABOVE_HOLES, COL_10_BLOCKS, MAX_STACK_HEIGHT, TETRIS_READY, TETRIS_FOUND);
	}

	bool calculateSequnce(float &minCost, Tetromino& bestPlace, Tetromino& t1, Tetromino& t2)
	{
		float newCost;
		bool foundBetter = false;
		for (int t1Rotations = 0; t1Rotations < t1.maxRotations(); t1Rotations++)
		{
			for (int t1Moves = -WIDTH / 2; t1Moves < WIDTH / 2 + 1; t1Moves++)
			{
				t1.defaultPos();
				move(t1Moves, &t1);
				while (drop(&t1));
				for (int t2Rotations = 0; t2Rotations < t2.maxRotations(); t2Rotations++)
				{
					for (int t2Moves = -WIDTH / 2; t2Moves < WIDTH / 2 + 1; t2Moves++)
					{
						t2.defaultPos();
						move(t2Moves, &t2);
						while (drop(&t2));
						newCost = costFunction(t1, t2);
						if (minCost > newCost)
						{
							minCost = newCost;
							bestPlace = t1;
							foundBetter = true;

							/*print();
							Event ev;
							do
								window.pollEvent(ev);
							while (ev.type != Event::MouseWheelMoved);*/
						}
					}
					t2.defaultPos();
					rotate(true, &t2);
				}
			}
			t1.defaultPos();
			rotate(true, &t1);
		}
		return foundBetter;
	}

	void calculateMove()
	{
		float cost = 1000000;
		bool holdBetter = false;
		Tetromino& AIcurrent = (aiTetrominos[AICURRENT]), &AInext = (aiTetrominos[AINEXT]), &AIheld = (aiTetrominos[AIHELD]);
		Tetromino bestPlacement;
		AIcurrent = Tetromino(current.type);
		AInext = Tetromino(next.type);
		AIheld = Tetromino(held.type);
		
		if (held.type != PIECES)
		{
			calculateSequnce(cost, bestPlacement, AIcurrent, AInext);
			calculateSequnce(cost, bestPlacement, AIcurrent, AIheld);
			holdBetter += calculateSequnce(cost, bestPlacement, AIheld, AInext);
			holdBetter += calculateSequnce(cost, bestPlacement, AIheld, AIcurrent);
		}
		else
		{
			calculateSequnce(cost, bestPlacement, AIcurrent, AInext);
			holdBetter += calculateSequnce(cost, bestPlacement, AInext, AIcurrent);
		}
		//cout << " h " << holdBetter << endl;
		if (holdBetter)
			hold();
		for(int i = 0; i < bestPlacement.rotated; i++)
			rotate(true);
		if(current.moved != bestPlacement.moved)
			move(bestPlacement.moved);
		adjustable = false;
		//cout << bestPlacement.rotated << ' ' << current.rotated << "\t" << bestPlacement.moved << ' ' << current.moved  << '\t' << piecenames[current.type] << ' ' << piecenames[bestPlacement.type] << endl;
	}
};
//print();
//Event ev;
//do
//window.pollEvent(ev);
//while (ev.type != Event::MouseWheelMoved);