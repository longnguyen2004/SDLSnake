#pragma once
#include <SDL2pp/SDL2pp.hh>
#include "../Screen/Screen.h"
#include "../Logic/Logic.h"

class Game
{
    SDL* sdl;
    Screen* screen;
    Logic* logic;
    bool Running;

public:
    Game();
    ~Game();
    int Run();
    void SetRunning(bool r);
};