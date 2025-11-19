#pragma once
#include <SDL.h>
using namespace std;
class Character
{
public:
	SDL_Rect rect;
	int health;
	int ability;
	int fightingFrame;
	int walkingFrame;
	int restingFrame;
	bool hit;
	bool fighting;
	bool aggressiv;
	bool walking;
	Uint32 LastFrameSwitchResting;
	Uint32 LastFrameSwitchWalking;
	Uint32 LastFrameSwitchFighting;
	Uint32 walkingCooldown; //used for chuuyas pathfiniding
	Uint32 fightingCooldown;
};