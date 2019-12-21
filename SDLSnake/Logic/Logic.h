#pragma once
#include "../GlobalDef.h"
#include "../Screen/Screen.h"
#include <list>
#include <utility>
#include <mutex>
#include <random>

using namespace SDL2pp;

class Game;

class Logic
{
	Game* game;
	Screen* screen;
	std::list<std::pair<int, int>>* SnakeBody;
	std::pair<int, int> SnakeHead, Pellet, CellToBeCleared;
	int direction;
	bool GameOver;
	SDL_TimerID AdvanceTimer;
	std::mutex mtx;
	std::mt19937 mt;
	std::uniform_int_distribution<int> xGen;
	std::uniform_int_distribution<int> yGen;

public:
	Logic(Game* g, Screen* scr);
	~Logic();
	void NewGame();
	void EventHandling();
	void Draw();
	void AdvanceSnake();
	void GeneratePellet();
	bool PelletCollected();
	bool Collision();
	void Process();
};