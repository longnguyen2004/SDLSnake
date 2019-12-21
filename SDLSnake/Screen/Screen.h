#pragma once
#include <SDL2pp/SDL2pp.hh>
#include <vector>

using namespace SDL2pp;

class Screen
{
	int width, height, width_grid, height_grid, cell_size;
	Window* window;
	Renderer* renderer;
	std::vector<int> *ScreenGrid;

public:
	Screen(int w, int h, int c);
	~Screen();
	const int& GetGridWidth() const;
	const int& GetGridHeight() const;
	void UpdateScreen();
	void Clear();
	void SetCell(int x, int y, int val);
	int GetCell(int x, int y);
};