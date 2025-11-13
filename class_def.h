#pragma once
#include <SDL.h>
using namespace std;
class Character
{
public:
	SDL_Rect rect;
	int health;
	int ability;
	int fighting_frame;
	int walking_frame;
	int resting_frame;
	bool hit;
	bool fighting;
	bool aggressiv;
	bool walking;
};