#include "Screen.h"
#include "../GlobalDef.h"

Screen::Screen(int w, int h, int c) : width(w), height(h), cell_size(c), width_grid(w / c), height_grid(h / c)
{
	ScreenGrid = new std::vector<int>((long)width_grid * height_grid, 0);
	window = new Window("Snake",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		width, height,
		SDL_WINDOW_ALLOW_HIGHDPI);
	renderer = new Renderer(*window, -1, SDL_RENDERER_ACCELERATED);
}

Screen::~Screen()
{
	delete ScreenGrid, window, renderer;
}

void Screen::UpdateScreen()
{
	renderer->SetDrawColor(255, 255, 255);
	renderer->Clear();
	for (int i = 0; i < width_grid; ++i)
		for (int j = 0; j < height_grid; ++j)
			if (ScreenGrid->at((long)i * height_grid + j))
			{	
				Rect rect(i * cell_size, j * cell_size, cell_size, cell_size);
				renderer->SetDrawColor(0, 0, 0).DrawRect(rect);
				switch (ScreenGrid->at((long)i * height_grid + j))
				{
				case SNAKE:
					renderer->SetDrawColor(255, 0, 0).FillRect(rect);
					break;
				case PELLET:
					renderer->SetDrawColor(0, 0, 255).FillRect(rect);
					break;
				}
			}
	renderer->Present();
}

const int& Screen::GetGridWidth() const
{
	return width_grid;
}

const int& Screen::GetGridHeight() const
{
	return height_grid;
}

void Screen::Clear()
{
	for (auto& i : *ScreenGrid) i = NOTHING;
}

void Screen::SetCell(int x, int y, int val)
{
	ScreenGrid->at((long)x * height_grid + y) = val;
}

int Screen::GetCell(int x, int y)
{
	return ScreenGrid->at((long)x * height_grid + y);
}
