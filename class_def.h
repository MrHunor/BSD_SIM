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
	Uint8 level;
	Uint32 LastFrameSwitchResting;
	Uint32 LastFrameSwitchWalking;
	Uint32 LastFrameSwitchFighting;
	Uint32 walkingCooldown; //used for chuuyas pathfiniding
	Uint32 fightingCooldown;
};

class DebugClass
{
public:
	/*Time for printing the message in ms*/
	Uint16 interval;
	bool Variant; //0 = normal; 1 = cls
	/*Debug console out true or false*/
	bool state;
	Uint32 gametime;
	/*This value is a Uint8 so you must cast it to a int before outputting it via cout*/
	Uint8 fps;
	float ramUsage;
	double CPULoad;
	//Debug Variables for gamestatus 1
	Uint16 dazaiCords[2];
	Uint8 dazaiHealth;
	Uint16 chuuyaCords[2];
	Uint8 chuuyahealth;
	bool chuuyaAggresiv;
	Uint16 disBetweenDAndC;
	Uint8 dazaiAbility;
	Uint8 chuuyaAbility;
	//Debug Variables for gamestatus 2
	Uint16 gunHolderCords[2];
	Uint16 crossairCords[2];
	Uint16 enemyCords[2];
	Uint8 enemyHealth;
	Uint8 gunHolderHealth;
};