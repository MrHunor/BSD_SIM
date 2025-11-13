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
|->08-11-2025/21:41/Converted std::cout to consoleout
|->08-11-2025/22:02/converted all bmp to png to save space and have transparency (load via IMG_loadtexture)
|->13-11-2025/20:11/Cleaned up and Optimesed by some variables to classes (Character class in class_def.h)
TODO:
|->Buxfixes needed: Give Abilitybar final tweaks; also make it acutally useful aka add ability bar for chuuya and give him a ability for dazai  to nullifiy
|->Move all the .bmp texture files to a seperate folder to clear up the main folder. (looks horrible in git)
|->Create config (that can be written to using the ingame console menu) for things like other exit animations etc.
|->Make chuuyas ability better, buff chuuya!
|->HEALTH BAR IMPORTANT
|->Create a GAME!->dazai  walking aroung, shooting first person at enemys etc..
|->Add exit crash animaton
|->resize to 1024x1024 (PoT)
|->ADD PROPER LOGGING AND DEBUG OUPUT FOR GODS SAKE
|->Debug Info class &  move dazai/chuuya frame time switch to their respective classes
******************************************************************************************/

#include <SDL.h>
#include <SDL_image.h>
#include <Windows.h>
#include <conio.h>

#include <iostream>

#include "Header.h"
#include "class_def.h"
using namespace std;

// Global Variables
bool quit = false;
bool hit_took = false;
bool quit2 = false;
bool console = false;
bool Debug = false;
int gamestatus = 0;
int textureW = 0;
int textureH = 0;
int fpsCounter = 0;
int lastFpsCount = 0;
int fpsLimit = 30;
string placeholder;
string command;
Uint32 currenttime;

int main(int argc, char* argv[]) {
	// Startup
	HWND consoleWindow = GetConsoleWindow();
	CreateLog();
	consoleout("BSD_SIM_V1.0\n");
	consoleout("[DEBUG]>>Debug Mode, y/n?\n");
	cin >> placeholder;
	if (placeholder == "y") {
		Debug = true;
		consoleout("[DEBUG]>>Debug Mode activated\n");
	}
	else {
		Debug = false;
		consoleout("[DEBUG]>>Debug Mode NOT activated\n");
	}

	consoleout("[SYSTEM]>>Startup started at:" + get_current_time_string() + "\n");
	Uint32 general_time = SDL_GetTicks();

	// Init SDL & other stuff
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		consoleout("[SYSTEM]>>SDL Initialization Error: " + string(SDL_GetError()) + "\n");
		return 1;
	}

	// Create Window and Renderer
	SDL_Window* window = SDL_CreateWindow("SDL2 Displaying Image", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1000, 1000, 0);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

	// Create Textures from Surfaces
	SDL_Texture* backround_texture = IMG_LoadTexture(renderer, "backround.png");
	SDL_Texture* player_resting_1_texture = IMG_LoadTexture(renderer, "dazai_resting_1.png");
	SDL_Texture* player_resting_2_texture = IMG_LoadTexture(renderer, "dazai_resting_2.png");
	SDL_Texture* player_walking_1_texture = IMG_LoadTexture(renderer, "dazai_walking_1.png");
	SDL_Texture* player_walking_2_texture = IMG_LoadTexture(renderer, "dazai_walking_2.png");
	SDL_Texture* chuuya_resting_texture = IMG_LoadTexture(renderer, "chuuya_resting.png");
	SDL_Texture* chuuya_aggressiv_texture = IMG_LoadTexture(renderer, "chuuya_aggressiv.png");
	SDL_Texture* player_fighting_right_1_texture = IMG_LoadTexture(renderer, "dazai_fighting_right_1.png");
	SDL_Texture* player_fighting_right_2_texture = IMG_LoadTexture(renderer, "dazai_fighting_right_2.png");
	SDL_Texture* player_fighting_right_3_texture = IMG_LoadTexture(renderer, "dazai_fighting_right_3.png");
	SDL_Texture* player_fighting_left_1_texture = IMG_LoadTexture(renderer, "dazai_fighting_left_1.png");
	SDL_Texture* player_fighting_left_2_texture = IMG_LoadTexture(renderer, "dazai_fighting_left_2.png");
	SDL_Texture* player_fighting_left_3_texture = IMG_LoadTexture(renderer, "dazai_fighting_left_3.png");
	SDL_Texture* chuuya_fighting_right_1_texture = IMG_LoadTexture(renderer, "chuuya_fighting_right_1.png");
	SDL_Texture* chuuya_fighting_right_2_texture = IMG_LoadTexture(renderer, "chuuya_fighting_right_2.png");
	SDL_Texture* chuuya_fighting_right_3_texture = IMG_LoadTexture(renderer, "chuuya_fighting_right_3.png");
	SDL_Texture* chuuya_fighting_left_1_texture = IMG_LoadTexture(renderer, "chuuya_fighting_left_1.png");
	SDL_Texture* chuuya_fighting_left_2_texture = IMG_LoadTexture(renderer, "chuuya_fighting_left_2.png");
	SDL_Texture* chuuya_fighting_left_3_texture = IMG_LoadTexture(renderer, "chuuya_fighting_left_3.png");
	SDL_Texture* shooting1P_1_texture = IMG_LoadTexture(renderer, "Shooting1P_1.png");
	SDL_Texture* shooting1P_2_texture = IMG_LoadTexture(renderer, "Shooting1P_2.png");

	// Check if textures created successfully
	if (!backround_texture || !player_resting_1_texture || !player_resting_2_texture || !player_walking_1_texture || !player_walking_2_texture || !chuuya_resting_texture || !chuuya_aggressiv_texture || !player_fighting_right_1_texture || !player_fighting_right_2_texture || !player_fighting_right_3_texture || !player_fighting_left_1_texture || !player_fighting_left_2_texture || !player_fighting_left_3_texture || !chuuya_fighting_right_1_texture || !chuuya_fighting_right_2_texture || !chuuya_fighting_right_3_texture || !chuuya_fighting_left_1_texture || !chuuya_fighting_left_2_texture || !chuuya_fighting_left_3_texture || !shooting1P_1_texture || !shooting1P_2_texture) {
		consoleout("[SYSTEM]>>Texture Creation Error: " + string(SDL_GetError()));
		return 1;
	}

	Character dazai = {};
	Character chuuya = {};
	dazai.health = 100;
	chuuya.health = 150;

	// Defining rects
	SDL_QueryTexture(backround_texture, NULL, NULL, &textureW, &textureH);
	SDL_Rect backround_rect = { 0, 0, textureW, textureH };
	SDL_QueryTexture(player_resting_1_texture, NULL, NULL, &textureW, &textureH);
	dazai.rect = { 100, 100, textureW, textureH };
	SDL_QueryTexture(chuuya_resting_texture, NULL, NULL, &textureW, &textureH);
	chuuya.rect = { 800, 800, textureW, textureH };
	SDL_QueryTexture(shooting1P_1_texture, NULL, NULL, &textureW, &textureH);
	SDL_Rect shooting1P_rect = { 620, 676, textureW, textureH };  // no need for a rect for the 2 frame because the dimesions are the same for both frames

	SDL_Point chuuyaRestingCenter = { chuuya.rect.w / 2, chuuya.rect.h / 2 };
	SDL_Point playerRestingCenter = { dazai.rect.w / 2, dazai.rect.h / 2 };

	// Timing Variables
	Uint32 LastFrameSwitch_resting = SDL_GetTicks();
	Uint32 LastFrameSwitch_walking = SDL_GetTicks();
	Uint32 Chuuya_walking_time = SDL_GetTicks();
	Uint32 Chuuya_fighting_time = SDL_GetTicks();
	Uint32 Chuuya_fighting_cooldown = SDL_GetTicks();
	Uint32 fighting_time = SDL_GetTicks();
	Uint32 fighting_cooldown_time = SDL_GetTicks();
	Uint32 Debug_time = SDL_GetTicks();
	Uint32 cleanuptime = SDL_GetTicks();
	Uint32 abilitlycountdown = SDL_GetTicks();
	Uint32 shootingcooldown = SDL_GetTicks();
	Uint32 fpsCounterTimer = SDL_GetTicks();
	Uint32 fpsLimitTimer = SDL_GetTicks();
	// Event Handling
	SDL_Event event;
	currenttime = SDL_GetTicks();
	consoleout("[SYSTEM]>>Startup finished after:" + to_string(currenttime - general_time) + "ms  at:" + get_current_time_string() + "\n");
	if (!Debug) Intro(renderer);
	consoleout("[SYSTEM]>>Program loop started\n");
	gamestatus = 1;  // for testing purposes

	// Main Game Loop
	quit = false;
	while (!quit) {
		currenttime = SDL_GetTicks();
		switch (gamestatus) {
		case 0:  // console

			quit2 = false;
			SetForegroundWindow(consoleWindow);
			consoleout("[CONSOLE]>>");
			while (!quit2) {
				cin >> command;
				if (command == "exit") {
					if (gamestatus != 0) {
						consoleout("[CONSOLE]>>Quitted Console\n");
						quit2 = true;
					}
					else {
						consoleout("[CONSOLE]>>You cannot exit the console while in console mode! Change gamestatus first via the gamestatus command.\n");
						consoleout("[CONSOLE]>>");
					}
				}
				else if (command == "help") {
					consoleout("[CONSOLE]>>Commands:\n");
					consoleout("[CONSOLE]>>exit: exits the console\n");
					consoleout("[CONSOLE]>>help: shows this message\n");
					consoleout("[CONSOLE]>>showlog: shows the log\n");
					consoleout("[CONSOLE]>>log: put message into log\n");
					consoleout("[CONSOLE]>>gamestatus: set gamestatus\n");
					consoleout("[CONSOLE]>>fps: set fps limit\n");
					consoleout("[CONSOLE]>>");
				}
				else if (command == "fps") {
					consoleout("[CONSOLE]>>Current FPS Limit=" + to_string(fpsLimit) + "\n[CONSOLE]>>Enter new FPS limit value(int) :");
					cin >> fpsLimit;
					consoleout("\n[CONSOLE]>>FPS limit set to:" + to_string(fpsLimit) + "\n");
					consoleout("[CONSOLE]>>");
				}
				else if (command == "log") {
					// Handle log command
					consoleout("[CONSOLE]>>Enter message to log(use '_' if you need to write sentences:");
					cin >> placeholder;
					Log("Console Log:" + placeholder);
					consoleout("\n[CONOSOLE]Message logged\n");
					consoleout("[CONSOLE]>>");
				}
				else if (command == "showlog") {
					consoleout("[CONSOLE]>>\n" + ReadLogFileToString() + "\n");
					consoleout("[CONSOLE]>>");
				}
				else if (command == "gamestatus") {
					consoleout("[CONSOLE]>>Current Game Status=" + to_string(gamestatus) + "\n[CONSOLE]>>Enter new gamestatus value(int) :");
					cin >> gamestatus;
					consoleout("[CONSOLE]>>Gamestatus set to:" + to_string(gamestatus) + "\n");
					consoleout("[CONSOLE]>>");
				}
				else {
					consoleout("[CONSOLE]>>Command not found\n");
					consoleout("[CONSOLE]>>");
				}
			}

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
								Log("dazai coordinates changed to:" + to_string(dazai.rect.x) + "," + to_string(dazai.rect.y));
							}
							break;
						case SDLK_s:
							if (dazai.rect.y < 950) {
								dazai.walking = true;
								dazai.rect.y += 10;
								Log("dazai coordinates changed to:" + to_string(dazai.rect.x) + "," + to_string(dazai.rect.y));
							}
							break;
						case SDLK_a:
							if (dazai.rect.x > 10) {
								dazai.walking = true;
								dazai.rect.x -= 10;
								Log("dazai coordinates changed to:" + to_string(dazai.rect.x) + "," + to_string(dazai.rect.y));
							}
							break;
						case SDLK_d:
							if (dazai.rect.x < 900) {
								dazai.walking = true;
								dazai.rect.x += 10;
								Log("dazai coordinates changed to:" + to_string(dazai.rect.x) + "," + to_string(dazai.rect.y));
							}
							break;
						case SDLK_e:
							if (!dazai.fighting) {
								dazai.fighting = true;
								fighting_time = SDL_GetTicks();
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
				if (currenttime - LastFrameSwitch_resting > 400) {
					if (dazai.resting_frame == 1) dazai.walking_frame = 2;
					else dazai.resting_frame = 1;
					LastFrameSwitch_resting = currenttime;
				}
				if (currenttime - LastFrameSwitch_walking > 200 && dazai.walking) {
					if (dazai.walking_frame == 1) dazai.walking_frame = 2;
					else dazai.walking_frame = 1;
					LastFrameSwitch_walking = currenttime;
				}
				if (dazai.fighting && currenttime - fighting_time > 200) {
					dazai.fighting_frame++;
					fighting_time = currenttime;
					if (dazai.fighting_frame > 3) {
						dazai.fighting = false;
						dazai.fighting_frame = 1;
					}
				}
				if (chuuya.fighting && currenttime - Chuuya_fighting_time > 200) {
					chuuya.fighting_frame++;
					Chuuya_fighting_time = currenttime;
					if (chuuya.fighting_frame > 3) {
						chuuya.fighting_frame = 1;
					}
				}

				// Render Player
				if (dazai.walking) {
					if (dazai.walking_frame == 1)
						SDL_RenderCopy(renderer, player_walking_1_texture, NULL, &dazai.rect);
					else
						SDL_RenderCopy(renderer, player_walking_2_texture, NULL, &dazai.rect);
				}
				else if (!dazai.fighting) {
					if (dazai.fighting_frame == 1)
						SDL_RenderCopy(renderer, player_resting_1_texture, NULL, &dazai.rect);
					else
						SDL_RenderCopy(renderer, player_resting_2_texture, NULL, &dazai.rect);
				}
				else {
					if (chuuya.rect.x > dazai.rect.x) {
						switch (dazai.fighting_frame) {
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
						switch (dazai.fighting_frame) {
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
							switch (chuuya.fighting_frame) {
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
							switch (chuuya.fighting_frame) {
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
						consoleout("[GAME]>>Enemy: 'Chuuya' took a hit!\n");
					}
					if (chuuya.health <= 0 && chuuya.rect.x != 9999) {
						consoleout("[GAME]>>Enemy: 'Chuuya' is defeated!\n");
						chuuya.aggressiv = false;
						chuuya.rect.x = 9999;
					}

					if (dazai.health <= 0) {
						consoleout("[GAME]>>Player: 'dazai ' is defeated!\n");
						quit = true;
					}

					if (Is_within_range(dazai.rect, chuuya.rect, 50)) {
						chuuya.fighting = true;
					}
					else {
						chuuya.fighting = false;
					}

					if (chuuya.fighting && currenttime - Chuuya_fighting_cooldown > 500) {
						dazai.health -= 10;
						consoleout("[GAME]>>Player: 'dazai ' took a hit!\n");
						Chuuya_fighting_cooldown = currenttime;
					}

					if (chuuya.aggressiv && currenttime - Chuuya_walking_time > 200 && !Is_within_range(chuuya.rect, dazai.rect, 50)) {
						if (chuuya.rect.x > dazai.rect.x)
							chuuya.rect.x -= 10;
						else if (chuuya.rect.x < dazai.rect.x)
							chuuya.rect.x += 10;
						else if (chuuya.rect.y > dazai.rect.y)
							chuuya.rect.y -= 10;
						else if (chuuya.rect.y < dazai.rect.y)
							chuuya.rect.y += 10;

						Chuuya_walking_time = currenttime;
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

				// Debug handling
				if (Debug && currenttime - Debug_time > 500) {
					consoleout("[DEBUG]>>Gametime:" + to_string(SDL_GetTicks()) + "\n");
					consoleout("[DEBUG]>>FPS:" + to_string(lastFpsCount) + "\n");
					consoleout("[DEBUG]>>RAM Usage:" + to_string(GetMemoryUsage()) + " MB" + "\n");
					consoleout("[DEBUG]>>dazai coordinates:" + to_string(dazai.rect.x) + "," + to_string(dazai.rect.y) + "\n");
					consoleout("[DEBUG]>>dazai health:" + to_string(dazai.health) + "\n");
					consoleout("[DEBUG]>>Chuuya coordinates:" + to_string(chuuya.rect.x) + "," + to_string(chuuya.rect.y) + "\n");
					consoleout("[DEBUG]>>Chuuya health:" + to_string(chuuya.health) + "\n");
					consoleout("[DEBUG]>>Chuuya_aggressiv:" + to_string(chuuya.aggressiv) + "\n");
					consoleout("[DEBUG]>>Distance between dazai and chuuya:" + to_string(Get_distance_between_rects(dazai.rect, chuuya.rect)) + "\n");
					consoleout("[DEBUG]>>dazai.abilitymeter:" + to_string(dazai.ability) + "\n");
					consoleout("[DEBUG]>>chuuya.abilitymeter:" + to_string(chuuya.ability) + "\n");
					consoleout("[DEBUG]>>This message will be displayed again in 500ms\n\n");
					Debug_time = currenttime;
				}

				// Present Renderer
				SDL_RenderPresent(renderer);
				fpsCounter++;
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
					SDL_RenderCopy(renderer, shooting1P_1_texture, 0, &shooting1P_rect);

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
								if (shooting1P_rect.y > 300) shooting1P_rect.y -= 100;
								SDL_RenderClear(renderer);
								break;
							case SDLK_s:
								if (shooting1P_rect.y < 676) shooting1P_rect.y += 100;
								SDL_RenderClear(renderer);
								break;
							case SDLK_a:
								if (shooting1P_rect.x > 300) shooting1P_rect.x -= 100;
								SDL_RenderClear(renderer);
								break;
							case SDLK_d:
								if (shooting1P_rect.x < 620) shooting1P_rect.x += 100;
								SDL_RenderClear(renderer);
								break;
							case SDLK_e:  // shoot
								if (currenttime - shootingcooldown > 200) {
									SDL_RenderClear(renderer);
									SDL_RenderCopy(renderer, shooting1P_2_texture, 0, &shooting1P_rect);
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

					if (Debug && currenttime - Debug_time > 500) {
						consoleout("[DEBUG]>>Gametime:" + to_string(SDL_GetTicks()) + "\n");
						consoleout("[DEBUG]>>FPS:" + to_string(lastFpsCount) + "\n");
						consoleout("[DEBUG]>>RAM Usage:" + to_string(GetMemoryUsage()) + " MB" + "\n");
						consoleout("[DEBUG]>>This message will be displayed again in 500ms\n\n");
						Debug_time = currenttime;
					}

					SDL_RenderPresent(renderer);
					fpsCounter++;
				}
			}

			break;
		default:
			consoleout("[GAME]>>Error: Unknown gamestatus value:" + to_string(gamestatus) + "\nShutting down...");
			quit = true;
		}
	}
	play_exit_animation(renderer);
	consoleout("[SYSTEM]>>Exited Game Loop. Starting cleanup at:" + get_current_time_string() + "\n");
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

	consoleout("[SYSTEM]>>Cleanup finished at:" + get_current_time_string() + "\n");
	consoleout("[SYSTEM]>>Shutting down. Bye Bye!\n");
	return 0;
}