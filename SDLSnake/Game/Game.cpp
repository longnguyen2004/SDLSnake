#define WIDTH 640
#define HEIGHT 480
#define CELL_SIZE 10

#include "Game.h"

using namespace SDL2pp;

Game::Game()
{
	sdl = new SDL(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_EVENTS);
	screen = new Screen(WIDTH, HEIGHT, CELL_SIZE);
	logic = new Logic(this, screen);
	Running = true;
}

Game::~Game()
{
	delete logic, screen, sdl;
}

int Game::Run()
{
	while (Running)
	{
		logic->Process();
		screen->UpdateScreen();
		SDL_Delay(1);
	}
	return 0;
}

void Game::SetRunning(bool r)
{
	Running = r;
}