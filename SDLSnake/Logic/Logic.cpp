#include "Logic.h"
#include "../Game/Game.h"
#include <vector>
#include <algorithm>

int AdvanceInterval;
std::vector<std::pair<int, int>> Movement = { {-1, 0}, { 1, 0 }, { 0, -1 }, { 0, 1 } };

int WrapAround(int val, int min, int max)
{
	if (val > max) return min;
	if (val < min) return max;
	return val;
}

Uint32 AdvanceSnakeWrapper(Uint32 interval, void* this_ptr)
{
	((Logic*)this_ptr)->AdvanceSnake();
	return interval;
}

Logic::Logic(Game* g, Screen* scr) : game(g), screen(scr)
{
	xGen.param(std::uniform_int_distribution<int>::param_type(0, screen->GetGridWidth() - 1));
	yGen.param(std::uniform_int_distribution<int>::param_type(0, screen->GetGridHeight()- 1));
	mt.seed(std::random_device()());
	SnakeBody = new std::list<std::pair<int, int>>();
	NewGame();
	AdvanceTimer = SDL_AddTimer(AdvanceInterval, AdvanceSnakeWrapper, this);
}

Logic::~Logic()
{
	SDL_RemoveTimer(AdvanceTimer);
	delete SnakeBody;
}

void Logic::NewGame()
{
	screen->Clear();
	SnakeBody->clear();
	SnakeHead = std::make_pair(screen->GetGridWidth() / 2, screen->GetGridHeight() / 2);
	direction = RIGHT;
	SnakeBody->emplace_back(SnakeHead.first - 1, SnakeHead.second);
	SnakeBody->emplace_back(SnakeHead.first, SnakeHead.second);
	GeneratePellet();
}

void Logic::EventHandling()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT) game->SetRunning(false);
		else if (event.type == SDL_KEYDOWN)
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_LEFT:
				if (direction != RIGHT) direction = LEFT;
				break;
			case SDLK_RIGHT:
				if (direction != LEFT) direction = RIGHT;
				break;
			case SDLK_UP:
				if (direction != DOWN) direction = UP;
				break;
			case SDLK_DOWN:
				if (direction != UP) direction = DOWN;
				break;
			case SDLK_ESCAPE:
				game->SetRunning(false);
			}
		}
	}
}

void Logic::Draw()
{
	std::lock_guard<std::mutex> lck(mtx);
	for (const auto& i : *SnakeBody)
		screen->SetCell(i.first, i.second, SNAKE);
	screen->SetCell(Pellet.first, Pellet.second, PELLET);
	screen->SetCell(CellToBeCleared.first, CellToBeCleared.second, NOTHING);
}

void Logic::AdvanceSnake()
{
	std::lock_guard<std::mutex> lck(mtx);
	SnakeHead = std::make_pair(WrapAround(SnakeHead.first  + Movement[direction].first, 0, screen->GetGridWidth() - 1),
							   WrapAround(SnakeHead.second + Movement[direction].second, 0, screen->GetGridHeight() - 1));
	SnakeBody->push_back(SnakeHead);
	CellToBeCleared = SnakeBody->front();
	SnakeBody->pop_front();
	if (PelletCollected())
	{
		SnakeHead = std::make_pair(WrapAround(SnakeHead.first + Movement[direction].first, 0, screen->GetGridWidth() - 1),
			WrapAround(SnakeHead.second + Movement[direction].second, 0, screen->GetGridHeight() - 1));
		SnakeBody->push_back(SnakeHead);
		GeneratePellet();
	}
	if (Collision())
	{
		NewGame();
	}
}

void Logic::GeneratePellet()
{
	do
	{
		Pellet.first = xGen(mt);
		Pellet.second = yGen(mt);
	} while (std::find(SnakeBody->begin(), SnakeBody->end(), Pellet) != SnakeBody->end());
}

bool Logic::PelletCollected()
{
	return SnakeHead == Pellet;
}

bool Logic::Collision()
{
	return std::find(SnakeBody->begin(), prev(SnakeBody->end()), SnakeHead) != prev(SnakeBody->end());
}

void Logic::Process()
{
	EventHandling();
	Draw();
}
