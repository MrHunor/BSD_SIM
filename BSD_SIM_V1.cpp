/****************************************************************************************
-------------------------------Information-----------------------------------------------
Filename:BSD_SIM_V1.cpp
Version:Pre-Release
Github Link:github.com/MrHunor/BSD_SIM

Changes Made [Date/Time/Summary of Changes Made]:
|->11-10-2025/23:50/Reused Project, installed corrosponding (old SDL2) libarys to make it work.
|->12-10-2025/00:28/Ability Bar pretty much works now, need some final cosmetic touches.
|->12-10-2025/00:45/Added Chuuya Ability bar;
|->14-10-2025/18:39/Added Chuuya ability (needs tweaks)
|->15-10-2025/19:20/Added Intro to game
|->16-10-2025/19:10/Created switch logic and created shooting animation surfaces and textures and created gamestatus console command
|->16-10-2025/20:30/Created basic shooting first person game mode (gamestatus 2)
|->07-11-2025/23:10/QOL Changes
|->08-11-2025/00:17/Added more debug and fps
|->08-11-2025/21:41/Converted std::cout to ConsoleOut
|->08-11-2025/22:02/converted all bmp to png to save space and have transparency (load via IMG_loadtexture)
|->13-11-2025/20:11/Cleaned up and Optimesed by some variables to classes (Character class in class_def.h)
|->18-11-2025/23:45/Added dialogue function and huge optimisation
|->19-11-2025/15:45/Copilot Log optimisation &moved more varibles to character class
|->19-12-2025/22:45/Resized Window from 1000² to 1024² for better performance
|->19-12-2025/23:30/Added Dialogue bar
|->26-12-2025/19:55/Added ColourCout Function and tconsolecolour
|->27-12-2025/13:52/Added Debug Class
|->28-12-2025/00:25/Small Performance upgrade, switched int to UintXX & fixed gamestatus bug
TODO:
|->1Buxfixes needed: Give Abilitybar final tweaks;
|->3Create config (that can be written to using the ingame console menu) for things like other exit animations etc.
|->4Make chuuyas ability better, buff chuuya!
|->5HEALTH BAR IMPORTANT
|->6Create a GAME!->dazai walking aroung, shooting first person at enemys etc..
|->7Add exit crash animaton
|->8resize to 1024x1024 (PoT)
|->9ADD PROPER LOGGING AND DEBUG OUPUT FOR GODS SAKE
|->12Create Startups sceen with starup progress
|->Add a enemy to the shooting game
|->maybe rethink debug Class, one the one hand cleaner and nicer to  read on the other side double declaration and useless RAM usage
|->expand the Character class to textures as well
|->think of a dynamic draw over way for gamestatus 2 so that u dont always have to draw over the hole screen when moving
|->add debug to gamestatus 2
|->add enemy to gamestatus 2
******************************************************************************************/

#include <SDL.h>
#include <SDL_image.h>
#include <Windows.h>
#include <SDL_ttf.h>
#include <iostream>
#include <mutex>

#include "Header.h"
#include "class_def.h"
#include <algorithm>

using namespace std;
std::once_flag flag;

bool ConsoleColour = true;//this has to be global so ConsoleOut can access it

int main(int argc, char* argv[]) {
	bool quit = false;
	bool hit_took = false;
	bool quit2 = false;
	bool console = false;

	Uint8 gamestatus = 0;
	int textureW = 0;
	int textureH = 0;
	Uint16 fpsCounter = 0;
	Uint16 lastFpsCount = 0;
	Uint16 fpsLimit = 30;
	string placeholder;
	string command;
	Uint32 currenttime;
	Character dazai = {};
	Character chuuya = {};
	Character gunHolder = {};
	Character enemy = {};
	dazai.health = 100;
	chuuya.health = 150;
	SDL_Event event;

	// Startup
	HWND consoleWindow = GetConsoleWindow();
	CreateLog();
	ConsoleOut("BSD_SIM_V0.9\n");
	DebugClass Debug;
	Debug.interval = 500;
	ConsoleOut("[DEBUG]>>Debug Mode, y/n?\n");
	cin >> placeholder;
	if (placeholder == "y") {
		Debug.state = true;
		ConsoleOut("[DEBUG]>>Debug Mode activated\n");
	}
	else {
		Debug.state = false;
		ConsoleOut("[DEBUG]>>Debug Mode NOT activated\n");
	}

	ConsoleOut("[SYSTEM]>>Startup started at:" + get_current_time_string() + "\n");
	Uint32 general_time = SDL_GetTicks();

	// Init SDL & other stuff
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		ConsoleOut("[SYSTEM]>>SDL Initialization Error: " + string(SDL_GetError()) + "\n");
		return 1;
	}
	TTF_Init();
	// Create Window and Renderer
	SDL_Window* window = SDL_CreateWindow("SDL2 Displaying Image", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1024, 1024, 0);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	// Create Textures from Surfaces
	SDL_Texture* backround_texture = IMG_LoadTexture(renderer, "assets\\backround.png");
	SDL_Texture* player_resting_1_texture = IMG_LoadTexture(renderer, "assets\\dazai_resting_1.png");
	SDL_Texture* player_resting_2_texture = IMG_LoadTexture(renderer, "assets\\dazai_resting_2.png");
	SDL_Texture* player_walking_1_texture = IMG_LoadTexture(renderer, "assets\\dazai_walking_1.png");
	SDL_Texture* player_walking_2_texture = IMG_LoadTexture(renderer, "assets\\dazai_walking_2.png");
	SDL_Texture* chuuya_resting_texture = IMG_LoadTexture(renderer, "assets\\chuuya_resting.png");
	SDL_Texture* chuuya_aggressiv_texture = IMG_LoadTexture(renderer, "assets\\chuuya_aggressiv.png");
	SDL_Texture* player_fighting_right_1_texture = IMG_LoadTexture(renderer, "assets\\dazai_fighting_right_1.png");
	SDL_Texture* player_fighting_right_2_texture = IMG_LoadTexture(renderer, "assets\\dazai_fighting_right_2.png");
	SDL_Texture* player_fighting_right_3_texture = IMG_LoadTexture(renderer, "assets\\dazai_fighting_right_3.png");
	SDL_Texture* player_fighting_left_1_texture = IMG_LoadTexture(renderer, "assets\\dazai_fighting_left_1.png");
	SDL_Texture* player_fighting_left_2_texture = IMG_LoadTexture(renderer, "assets\\dazai_fighting_left_2.png");
	SDL_Texture* player_fighting_left_3_texture = IMG_LoadTexture(renderer, "assets\\dazai_fighting_left_3.png");
	SDL_Texture* chuuya_fighting_right_1_texture = IMG_LoadTexture(renderer, "assets\\chuuya_fighting_right_1.png");
	SDL_Texture* chuuya_fighting_right_2_texture = IMG_LoadTexture(renderer, "assets\\chuuya_fighting_right_2.png");
	SDL_Texture* chuuya_fighting_right_3_texture = IMG_LoadTexture(renderer, "assets\\chuuya_fighting_right_3.png");
	SDL_Texture* chuuya_fighting_left_1_texture = IMG_LoadTexture(renderer, "assets\\chuuya_fighting_left_1.png");
	SDL_Texture* chuuya_fighting_left_2_texture = IMG_LoadTexture(renderer, "assets\\chuuya_fighting_left_2.png");
	SDL_Texture* chuuya_fighting_left_3_texture = IMG_LoadTexture(renderer, "assets\\chuuya_fighting_left_3.png");
	SDL_Texture* shooting1P_1_texture = IMG_LoadTexture(renderer, "assets\\Shooting1P_1.png");
	SDL_Texture* shooting1P_2_texture = IMG_LoadTexture(renderer, "assets\\Shooting1P_2.png");
	SDL_Texture* dialogue_window = IMG_LoadTexture(renderer, "assets\\dialogue_window.png");
	SDL_Texture* enemy_texture = IMG_LoadTexture(renderer, "assets\\Enemy.png");
	// Check if textures created successfully
	if (!backround_texture || !player_resting_1_texture || !player_resting_2_texture || !player_walking_1_texture || !player_walking_2_texture || !chuuya_resting_texture || !chuuya_aggressiv_texture || !player_fighting_right_1_texture || !player_fighting_right_2_texture || !player_fighting_right_3_texture || !player_fighting_left_1_texture || !player_fighting_left_2_texture || !player_fighting_left_3_texture || !chuuya_fighting_right_1_texture || !chuuya_fighting_right_2_texture || !chuuya_fighting_right_3_texture || !chuuya_fighting_left_1_texture || !chuuya_fighting_left_2_texture || !chuuya_fighting_left_3_texture || !shooting1P_1_texture || !shooting1P_2_texture) {
		ConsoleOut("[SYSTEM]>>Texture Creation Error: " + string(SDL_GetError()));
		return 1;
	}

	// Defining rects
	SDL_QueryTexture(backround_texture, NULL, NULL, &textureW, &textureH);
	SDL_Rect backround_rect = { 0, 0, textureW, textureH };
	SDL_QueryTexture(player_resting_1_texture, NULL, NULL, &textureW, &textureH);
	dazai.rect = { 100, 100, textureW, textureH };
	SDL_QueryTexture(chuuya_resting_texture, NULL, NULL, &textureW, &textureH);
	chuuya.rect = { 800, 800, textureW, textureH };
	SDL_QueryTexture(shooting1P_1_texture, NULL, NULL, &textureW, &textureH);
	// no need for a rect for the 2 frame because the dimesions are the same for both frames
	gunHolder.rect = { 620, 676, textureW, textureH };
	SDL_QueryTexture(dialogue_window, NULL, NULL, &textureW, &textureH);
	SDL_Rect dialogue_window_rect = { 250, 820, textureW, textureH };

	SDL_Point chuuyaRestingCenter = { chuuya.rect.w / 2, chuuya.rect.h / 2 };
	SDL_Point playerRestingCenter = { dazai.rect.w / 2, dazai.rect.h / 2 };

	// Timing Variables
	dazai.LastFrameSwitchResting = SDL_GetTicks();
	dazai.LastFrameSwitchWalking = SDL_GetTicks();
	chuuya.walkingCooldown = SDL_GetTicks();
	chuuya.LastFrameSwitchFighting = SDL_GetTicks();
	chuuya.fightingCooldown = SDL_GetTicks();
	dazai.LastFrameSwitchFighting = SDL_GetTicks();
	Uint32 Debug_time = SDL_GetTicks();
	Uint32 cleanuptime = SDL_GetTicks();
	Uint32 abilitlycountdown = SDL_GetTicks();
	Uint32 shootingcooldown = SDL_GetTicks();
	Uint32 fpsCounterTimer = SDL_GetTicks();
	Uint32 fpsLimitTimer = SDL_GetTicks();
	currenttime = SDL_GetTicks();

	ConsoleOut("[SYSTEM]>>Startup finished after:" + to_string(currenttime - general_time) + "ms  at:" + get_current_time_string() + "\n");
	if (!Debug.state) Intro(renderer);
	ConsoleOut("[SYSTEM]>>Program loop started\n");
	gamestatus = 1;  // for testing purposes

	// Main Game Loop
	quit = false;
	while (!quit) {
		currenttime = SDL_GetTicks();
		switch (gamestatus) {
		case 0:  // console

			quit2 = false;
			SetForegroundWindow(consoleWindow);
			ConsoleOut("[CONSOLE]>>");
			while (!quit2) {
				cin >> command;
				if (command == "exit") {
					if (gamestatus != 0) {
						ConsoleOut("[CONSOLE]>>Quitted Console\n");
						quit2 = true;
					}
					else {
						ConsoleOut("[CONSOLE]>>You cannot exit the console while in console mode! Change gamestatus first via the gamestatus command.\n");
						ConsoleOut("[CONSOLE]>>");
					}
				}
				else if (command == "help") {
					ConsoleOut("[CONSOLE]>>Commands:\n");
					ConsoleOut("[CONSOLE]>>exit: exits the console\n");
					ConsoleOut("[CONSOLE]>>help: shows this message\n");
					ConsoleOut("[CONSOLE]>>showlog: shows the log\n");
					ConsoleOut("[CONSOLE]>>log: put message into log\n");
					ConsoleOut("[CONSOLE]>>gamestatus: set gamestatus\n");
					ConsoleOut("[CONSOLE]>>fps: set fps limit\n");
					ConsoleOut("[CONSOLE]>>dialogue: test the dialogue function\n");
					ConsoleOut("[CONSOLE]>>tconsolecolour: toggle ConsoleColour\n");
					ConsoleOut("[CONSOLE]>>tdebug: toggle Debug\n");
					ConsoleOut("[CONSOLE]>>debugintervall: Set Debug interval in ms (the time at which the message is printed to the console), max. value = 65535\n");
					ConsoleOut("[CONSOLE]>>");
				}
				else if (command == "fps") {
					ConsoleOut("[CONSOLE]>>Current FPS Limit=" + to_string(fpsLimit) + "\n[CONSOLE]>>Enter new FPS limit value(int) :");
					cin >> placeholder;
					if (stoi(placeholder) > GetWindowRefreshRate(window))
					{
						fpsLimit = GetWindowRefreshRate(window);
						ConsoleOut("[CONSOLE]>>Your monitor's refresh rate is:" + to_string(GetWindowRefreshRate(window)) + ". Setting fps to your monitors refresh rate.\n");
					}
					else fpsLimit = stoi(placeholder);
					ConsoleOut("\n[CONSOLE]>>FPS limit set to:" + to_string(fpsLimit) + "\n");
					ConsoleOut("[CONSOLE]>>");
				}
				else if (command == "log") {
					// Handle log command
					ConsoleOut("[CONSOLE]>>Enter message to log(use '_' if you need to write sentences:");
					cin >> placeholder;
					Log("Console Log:" + placeholder);
					ConsoleOut("\n[CONOSOLE]Message logged\n");
					ConsoleOut("[CONSOLE]>>");
				}
				else if (command == "showlog") {
					ConsoleOut("[CONSOLE]>>\n" + ReadLogFileToString() + "\n");
					ConsoleOut("[CONSOLE]>>");
				}
				else if (command == "gamestatus") {
					ConsoleOut("[CONSOLE]>>Current Game Status=" + to_string(static_cast<int>(gamestatus)) + "\n[CONSOLE]>>Enter new gamestatus value(int) :");
					cin >> placeholder;
					gamestatus = static_cast<Uint8>(stoi(placeholder));
					ConsoleOut("[CONSOLE]>>Gamestatus set to:" + to_string(static_cast<int>(gamestatus)) + "\n");
					ConsoleOut("[CONSOLE]>>");
				}
				else if (command == "dialogue")
				{
					cin >> placeholder;
					if (show_dialogue(renderer, placeholder, 50, player_resting_1_texture, &dazai.rect, dialogue_window, &dialogue_window_rect) == -1) ConsoleOut("[CONSOLE]>>ERROR:Text too long");
					ConsoleOut("[CONSOLE]>>");
				}
				else if (command == "tconsolecolour")
				{
					ConsoleColour = !ConsoleColour;
					ConsoleOut("[CONSOLE]>>ConsoleColour is now:" + to_string(ConsoleColour) + "\n");
					ConsoleOut("[CONSOLE]>>");
				}
				else if (command == "tdebug")
				{
					Debug.state = !Debug.state;
					ConsoleOut("[CONSOLE]>>Debug is now:" + to_string(Debug.state) + "\n");
					ConsoleOut("[CONSOLE]>>");
				}
				else if (command == "debugintervall")
				{
					ConsoleOut("[CONSOLE]>>Current debug intervall:" + to_string(Debug.interval) + "Enter new Debug interval:");
					cin >> placeholder;
					if (std::all_of(placeholder.begin(), placeholder.end(), ::isdigit) && stoi(placeholder) <= 65535)
					{
						Debug.interval = stoi(placeholder);
						ConsoleOut("\n[CONSOLE]>>Debug Intervall is now:" + to_string(Debug.interval) + "\n");
					}
					else ConsoleOut("[CONSOLE]>>Input Invalid. Check help for more information");
					ConsoleOut("[CONSOLE]>>");
				}
				else {
					ConsoleOut("[CONSOLE]>>Command not found\n");
					ConsoleOut("[CONSOLE]>>");
				}
			}
			break;
		case 1:
			if (currenttime - fpsLimitTimer > (1000 / fpsLimit)) {
				fpsLimitTimer = currenttime;
				dazai.walking = false;
				// Handle Events
				while (SDL_PollEvent(&event)) {
					if (event.type == SDL_QUIT) {
						quit = true;
					}
					else if (event.type == SDL_KEYDOWN) {
						switch (event.key.keysym.sym) {
						case SDLK_w:
							if (dazai.rect.y > 10) {
								dazai.walking = true;
								dazai.rect.y -= 10;

								if (Debug.state)ConsoleOut("[DEBUG]>>dazai coordinates changed to:" + to_string(dazai.rect.x) + "," + to_string(dazai.rect.y) + "\n");
							}
							break;
						case SDLK_s:
							if (dazai.rect.y < 974) {
								dazai.walking = true;
								dazai.rect.y += 10;
								if (Debug.state)ConsoleOut("[DEBUG]>>dazai coordinates changed to:" + to_string(dazai.rect.x) + "," + to_string(dazai.rect.y) + "\n");
							}
							break;
						case SDLK_a:
							if (dazai.rect.x > 10) {
								dazai.walking = true;
								dazai.rect.x -= 10;
								if (Debug.state)ConsoleOut("[DEBUG]>>dazai coordinates changed to:" + to_string(dazai.rect.x) + "," + to_string(dazai.rect.y) + "\n");
							}
							break;
						case SDLK_d:
							if (dazai.rect.x < 924) {
								dazai.walking = true;
								dazai.rect.x += 10;
								if (Debug.state)ConsoleOut("[DEBUG]>>dazai coordinates changed to:" + to_string(dazai.rect.x) + "," + to_string(dazai.rect.y) + "\n");
							}
							break;
						case SDLK_e:
							if (!dazai.fighting) {
								dazai.fighting = true;
								dazai.LastFrameSwitchFighting = SDL_GetTicks();
								hit_took = false;
							}
							break;
						case SDLK_ESCAPE:
							quit = true;
							break;
						case SDLK_c:
							gamestatus = 0;
							break;
						}
					}
				}

				// Render Background
				SDL_RenderClear(renderer);
				SDL_RenderCopy(renderer, backround_texture, 0, &backround_rect);

				// Update Frame States
				if (currenttime - dazai.LastFrameSwitchResting > 400) {
					if (dazai.restingFrame == 1) dazai.walkingFrame = 2;
					else dazai.restingFrame = 1;
					dazai.LastFrameSwitchResting = currenttime;
				}
				if (currenttime - dazai.LastFrameSwitchWalking > 200 && dazai.walking) {
					if (dazai.walkingFrame == 1) dazai.walkingFrame = 2;
					else dazai.walkingFrame = 1;
					dazai.LastFrameSwitchWalking = currenttime;
				}
				if (dazai.fighting && currenttime - dazai.LastFrameSwitchFighting > 200) {
					dazai.fightingFrame++;
					dazai.LastFrameSwitchFighting = currenttime;
					if (dazai.fightingFrame > 3) {
						dazai.fighting = false;
						dazai.fightingFrame = 1;
					}
				}
				if (chuuya.fighting && currenttime - chuuya.LastFrameSwitchFighting > 200) {
					chuuya.fightingFrame++;
					chuuya.LastFrameSwitchFighting = currenttime;
					if (chuuya.fightingFrame > 3) {
						chuuya.fightingFrame = 1;
					}
				}

				// Render Player
				if (dazai.walking) {
					if (dazai.walkingFrame == 1)
						SDL_RenderCopy(renderer, player_walking_1_texture, NULL, &dazai.rect);
					else
						SDL_RenderCopy(renderer, player_walking_2_texture, NULL, &dazai.rect);
				}
				else if (!dazai.fighting) {
					if (dazai.fightingFrame == 1)
						SDL_RenderCopy(renderer, player_resting_1_texture, NULL, &dazai.rect);
					else
						SDL_RenderCopy(renderer, player_resting_2_texture, NULL, &dazai.rect);
				}
				else {
					if (chuuya.rect.x > dazai.rect.x) {
						switch (dazai.fightingFrame) {
						case 1:
							SDL_RenderCopy(renderer, player_fighting_right_1_texture, NULL, &dazai.rect);
							break;
						case 2:
							SDL_RenderCopy(renderer, player_fighting_right_2_texture, NULL, &dazai.rect);
							break;
						case 3:
							SDL_RenderCopy(renderer, player_fighting_right_3_texture, NULL, &dazai.rect);
							break;
						}
					}
					else {
						switch (dazai.fightingFrame) {
						case 1:
							SDL_RenderCopy(renderer, player_fighting_left_1_texture, NULL, &dazai.rect);
							break;
						case 2:
							SDL_RenderCopy(renderer, player_fighting_left_2_texture, NULL, &dazai.rect);
							break;
						case 3:
							SDL_RenderCopy(renderer, player_fighting_left_3_texture, NULL, &dazai.rect);
							break;
						}
					}
				}

				if (currenttime - abilitlycountdown > 300) {
					if (dazai.ability < 115) dazai.ability = dazai.ability + 5;
					if (chuuya.ability < 115) chuuya.ability = chuuya.ability + 3;
					abilitlycountdown = currenttime;
				}

				render_abilitly_meter(renderer, dazai.ability, dazai.rect);
				render_abilitly_meter(renderer, chuuya.ability, chuuya.rect);

				// Render Chuuya
				if (Is_within_range(dazai.rect, chuuya.rect, 300)) {
					chuuya.aggressiv = true;
				}

				if (chuuya.health > 0) {
					if (!chuuya.fighting) {
						if (chuuya.aggressiv) {
							SDL_RenderCopy(renderer, chuuya_aggressiv_texture, NULL, &chuuya.rect);
						}
						else {
							SDL_RenderCopy(renderer, chuuya_resting_texture, NULL, &chuuya.rect);
						}
					}
					else {
						if (dazai.rect.x > chuuya.rect.x) {
							switch (chuuya.fightingFrame) {
							case 1:
								SDL_RenderCopy(renderer, chuuya_fighting_right_1_texture, NULL, &chuuya.rect);
								break;
							case 2:
								SDL_RenderCopy(renderer, chuuya_fighting_right_2_texture, NULL, &chuuya.rect);
								break;
							case 3:
								SDL_RenderCopy(renderer, chuuya_fighting_right_3_texture, NULL, &chuuya.rect);
								break;
							}
						}
						else {
							switch (chuuya.fightingFrame) {
							case 1:
								SDL_RenderCopy(renderer, chuuya_fighting_left_1_texture, NULL, &chuuya.rect);
								break;
							case 2:
								SDL_RenderCopy(renderer, chuuya_fighting_left_2_texture, NULL, &chuuya.rect);
								break;
							case 3:
								SDL_RenderCopy(renderer, chuuya_fighting_left_3_texture, NULL, &chuuya.rect);
								break;
							}
						}
					}

					// Handle Chuuya's Health and Fighting
					if (Is_within_range(dazai.rect, chuuya.rect, 50) && dazai.fighting && !hit_took) {
						hit_took = true;
						chuuya.health -= 10;
						ConsoleOut("[GAME]>>Enemy: 'Chuuya' took a hit!\n");
					}
					if (chuuya.health <= 0) {
						std::call_once(flag, [&]
							{
								ConsoleOut("[GAME]>>Enemy: 'Chuuya' is defeated!\n");
								chuuya.aggressiv = false;
							});
					}

					if (dazai.health <= 0) {
						ConsoleOut("[GAME]>>Player: 'dazai ' is defeated!\n");
						quit = true;
					}

					if (Is_within_range(dazai.rect, chuuya.rect, 50)) {
						chuuya.fighting = true;
					}
					else {
						chuuya.fighting = false;
					}

					if (chuuya.fighting && currenttime - chuuya.fightingCooldown > 500) {
						dazai.health -= 10;
						ConsoleOut("[GAME]>>Player: 'dazai ' took a hit!\n");
						chuuya.fightingCooldown = currenttime;
					}

					if (chuuya.aggressiv && currenttime - chuuya.walkingCooldown > 200 && !Is_within_range(chuuya.rect, dazai.rect, 50)) {
						if (chuuya.rect.x > dazai.rect.x)
							chuuya.rect.x -= 10;
						else if (chuuya.rect.x < dazai.rect.x)
							chuuya.rect.x += 10;
						else if (chuuya.rect.y > dazai.rect.y)
							chuuya.rect.y -= 10;
						else if (chuuya.rect.y < dazai.rect.y)
							chuuya.rect.y += 10;

						chuuya.walkingCooldown = currenttime;
					}

					if (chuuya.ability > 110 && Is_within_range(dazai.rect, chuuya.rect, 50)) {
						// play ability animation
						SDL_RenderCopy(renderer, backround_texture, 0, &backround_rect);
						SDL_RenderCopy(renderer, chuuya_resting_texture, NULL, &chuuya.rect);
						for (size_t i = 0; i < 720; i++) {
							SDL_RenderCopy(renderer, backround_texture, 0, &backround_rect);
							SDL_RenderCopy(renderer, chuuya_resting_texture, NULL, &chuuya.rect);
							SDL_RenderCopyEx(renderer, player_resting_1_texture, 0, &dazai.rect, i, &playerRestingCenter, SDL_FLIP_NONE);
							SDL_RenderPresent(renderer);
							Sleep(1);
						}

						dazai.health -= 20;
						chuuya.ability = 0;
					}
				}

				// fps debug prep
				if (currenttime - fpsCounterTimer > 1000) {
					lastFpsCount = fpsCounter;
					fpsCounter = 0;
					fpsCounterTimer = currenttime;
				}
				//Debug Calculation
				if (Debug.state)
				{
					Debug.gametime = SDL_GetTicks();
					Debug.fps = lastFpsCount;
					Debug.ramUsage = GetMemoryUsage();
					Debug.dazaiCords[0] = dazai.rect.x;
					Debug.dazaiCords[1] = dazai.rect.y;
					Debug.dazaiHealth = dazai.health;
					Debug.chuuyaCords[0] = chuuya.rect.x;
					Debug.chuuyaCords[1] = chuuya.rect.y;
					Debug.chuuyahealth = chuuya.health;
					Debug.chuuyaAggresiv = chuuya.aggressiv;
					Debug.disBetweenDAndC = Get_distance_between_rects(dazai.rect, chuuya.rect);
					Debug.dazaiAbility = dazai.ability;
					Debug.chuuyaAbility = chuuya.ability;
					Debug.CPULoad = GetCPULoad();
				}

				// Debug output
				if (Debug.state && currenttime - Debug_time > Debug.interval) {
					ConsoleOut("[DEBUG]>>Gametime:" + to_string(Debug.gametime) + "\n");
					ConsoleOut("[DEBUG]>>FPS:" + to_string(Debug.fps) + "\n");
					ConsoleOut("[DEBUG]>>RAM Usage:" + to_string(Debug.ramUsage) + " MB" + "\n");
					ConsoleOut("[DEBUG]>>dazai coordinates:" + to_string(Debug.dazaiCords[0]) + "," + to_string(Debug.dazaiCords[1]) + "\n");
					ConsoleOut("[DEBUG]>>dazai health:" + to_string(Debug.dazaiHealth) + "\n");
					ConsoleOut("[DEBUG]>>Chuuya coordinates:" + to_string(Debug.chuuyaCords[0]) + "," + to_string(Debug.chuuyaCords[1]) + "\n");
					ConsoleOut("[DEBUG]>>Chuuya health:" + to_string(Debug.chuuyahealth) + "\n");
					ConsoleOut("[DEBUG]>>Chuuya_aggressiv:" + to_string(Debug.chuuyaAggresiv) + "\n");
					ConsoleOut("[DEBUG]>>Distance between dazai and chuuya:" + to_string(Debug.disBetweenDAndC) + "\n");
					ConsoleOut("[DEBUG]>>dazai.abilitymeter:" + to_string(Debug.dazaiAbility) + "\n");
					ConsoleOut("[DEBUG]>>chuuya.abilitymeter:" + to_string(Debug.chuuyaAbility) + "\n");
					ConsoleOut("[DEBUG]>>Current CPU Usage:" + to_string(Debug.CPULoad) + "%\n");
					ConsoleOut("[DEBUG]>>This message will be displayed again in " + to_string(Debug.interval) + "ms\n\n");

					Debug_time = currenttime;
				}

				// Present Renderer
				SDL_RenderPresent(renderer);
				fpsCounter++;
			}
			else
			{
				SDL_Delay(1000 / fpsLimit);
			}
			break;

		case 2:  // Shooting game first person - Practice
			quit2 = false;
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_RenderClear(renderer);
			while (!quit2) {
				currenttime = SDL_GetTicks();
				if (currenttime - fpsLimitTimer > (1000 / fpsLimit)) {
					fpsLimitTimer = currenttime;
					currenttime = SDL_GetTicks();
					SDL_RenderCopy(renderer, shooting1P_1_texture, 0, &gunHolder.rect);
					SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
					DrawFilledCircle(renderer, gunHolder.rect.x - 100, gunHolder.rect.y - 100, 5);
					SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
					while (SDL_PollEvent(&event)) {
						if (event.type == SDL_QUIT) {
							quit = true;
							quit2 = true;
						}

						else if (event.type == SDL_KEYDOWN) {
							switch (event.key.keysym.sym) {
							case SDLK_c:
								gamestatus = 0;
								quit2 = true;
								break;
							case SDLK_ESCAPE:
								quit = true;
								break;
							case SDLK_w:
								if (gunHolder.rect.y > 300) gunHolder.rect.y -= 100;
								SDL_RenderClear(renderer);
								break;
							case SDLK_s:
								if (gunHolder.rect.y < 676) gunHolder.rect.y += 100;
								SDL_RenderClear(renderer);
								break;
							case SDLK_a:
								if (gunHolder.rect.x > 300) gunHolder.rect.x -= 100;
								SDL_RenderClear(renderer);
								break;
							case SDLK_d:
								if (gunHolder.rect.x < 620) gunHolder.rect.x += 100;
								SDL_RenderClear(renderer);
								break;
							case SDLK_e:  // shoot
								if (currenttime - shootingcooldown > 200) {
									SDL_RenderClear(renderer);
									SDL_RenderCopy(renderer, shooting1P_2_texture, 0, &gunHolder.rect);
									SDL_RenderPresent(renderer);
									SDL_Delay(100);
									SDL_RenderClear(renderer);
									shootingcooldown = currenttime;
								}
								break;
							}
						}
					}

					// fps debug prep
					if (currenttime - fpsCounterTimer > 1000) {
						lastFpsCount = fpsCounter;
						fpsCounter = 0;
						fpsCounterTimer = currenttime;
					}

					SDL_RenderPresent(renderer);
					fpsCounter++;
				}
				else
				{
					SDL_Delay(1000 / fpsLimit);
				}
			}

			break;
		default:
			ConsoleOut("[GAME]>>Error: Unknown gamestatus value:" + to_string(gamestatus) + "\nShutting down...");
			quit = true;
		}
	}
	play_exit_animation(renderer);
	ConsoleOut("[SYSTEM]>>Exited Game Loop. Starting cleanup at:" + get_current_time_string() + "\n");
	cleanuptime = SDL_GetTicks();
	// Clean Up Resources
	SDL_DestroyTexture(backround_texture);
	SDL_DestroyTexture(player_resting_1_texture);
	SDL_DestroyTexture(player_resting_2_texture);
	SDL_DestroyTexture(player_walking_1_texture);
	SDL_DestroyTexture(player_walking_2_texture);
	SDL_DestroyTexture(chuuya_resting_texture);
	SDL_DestroyTexture(chuuya_aggressiv_texture);
	SDL_DestroyTexture(player_fighting_right_1_texture);
	SDL_DestroyTexture(player_fighting_right_2_texture);
	SDL_DestroyTexture(player_fighting_right_3_texture);
	SDL_DestroyTexture(player_fighting_left_1_texture);
	SDL_DestroyTexture(player_fighting_left_2_texture);
	SDL_DestroyTexture(player_fighting_left_3_texture);
	SDL_DestroyTexture(chuuya_fighting_right_1_texture);
	SDL_DestroyTexture(chuuya_fighting_right_2_texture);
	SDL_DestroyTexture(chuuya_fighting_right_3_texture);
	SDL_DestroyTexture(chuuya_fighting_left_1_texture);
	SDL_DestroyTexture(chuuya_fighting_left_2_texture);
	SDL_DestroyTexture(chuuya_fighting_left_3_texture);

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	currenttime = SDL_GetTicks();

	ConsoleOut("[SYSTEM]>>Cleanup finished at:" + get_current_time_string() + "\n");
	ConsoleOut("[SYSTEM]>>Shutting down. Bye Bye!\n");

	// Ensure log file is properly closed
	CloseLog();
	return 0;
}