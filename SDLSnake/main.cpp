#define SDL_MAIN_HANDLED

#include <iostream>
#include "Game/Game.h"
#include "GlobalDef.h"

int main(int argc, char* argv[])
{
	std::cout << "Set Snake Speed (ms/step): ";
	std::cin >> AdvanceInterval;
	SDL_SetMainReady();
	Game game;
	return game.Run();
}