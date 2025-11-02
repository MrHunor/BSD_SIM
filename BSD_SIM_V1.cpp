/****************************************************************************************
                              -Information-
Filename:BSD_SIM_V1.cpp 
Version:1.0
Github Link:None
Changes Made [Date/Time/Summary of Changes Made]:
|->11-10-2025/23:50/Reused Project, installed corrosponding (old SDL2) libarys to make it work. 
|->12-10-2025/00:28/Ability Bar pretty much works now, need some final cosmetic touches.
|->12-10-2025/00:45/Added Chuuya Ability bar; 
|->14-10-2025/18:39/ Added Chuuya ability (needs tweaks)
|->15-10-2025/19:20/ Added Intro to game
|->16-10-2025/19:10/Created switch logic and created shooting animation surfaces and textures and created gamestatus console command
TODO:
|->Buxfixes needed: Give Abilitybar final tweaks; also make it acutally useful aka add ability bar for chuuya and give him a ability for dazai to nullifiy
|->Move all the .bmp texture files to a seperate folder to clear up the main folder. (looks horrible in git)
|->Create config (that can be written to using the ingame console menu) for things like other exit animations etc.
|->Make chuuyas ability better, buff chuuya!
|->HEALTH BAR IMPORTANT
|->Move logs to dedicated log folder
|->Create a GAME!->dazai walking aroung, shooting first person at enemys etc..
|->Add exit crash animaton

******************************************************************************************/

#include <iostream>
#include <SDL.h>
#include <Windows.h>
#include "Header.h"
#include <conio.h>

using namespace std;


// Global Variables
bool quit = false;
bool display_frame1_resting = true;
bool display_frame1_walking = true;
bool walking = false;
bool chuuya_aggressiv = false;
bool fighting = false;
bool chuuya_fighting = false;
bool hit_took = false;
bool quit2 = false;
bool console = false;
bool Debug = false;

int fighting_frame_player = 1;
int fighting_frame_chuuya = 1;
int playerhealth = 100;
int chuuyahealth = 50;
int playerability = 0;
int chuuyaability = 0;
int gamestatus=0;
string placeholder;
string command;
Uint32 currenttime;

int main(int argc, char* argv[]) {
    //Startup
    HWND consoleWindow = GetConsoleWindow();
    CreateLog();
    cout << "BSD_SIM_V1.0\n";
	cout << "[DEBUG]>>Debug, y/n?\n";
	cin>>placeholder;
    if (placeholder == "y")
    {
        Debug = true; cout << "[DEBUG]>>Debug activated\n"; 
		Log("Debug activated");
    }
    else {
        Debug = false; cout << "[DEBUG]>>Debug NOT activated\n";
        Log("Debug not activated");
    }


    cout << "[SYSTEM]>>Startup started at:" << get_current_time_string() << endl;

    Uint32 general_time = SDL_GetTicks();
    
    
    
    
    //Init SDL & other stuff
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        cerr << "SDL_Init Error: " << SDL_GetError() << endl;
        return 1;
    }
 

    // Create Window and Renderer
    SDL_Window* window = SDL_CreateWindow("SDL2 Displaying Image", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1000, 1000, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    // Load Surfaces
    SDL_Surface* backround_surface = SDL_LoadBMP("backround.bmp");
    SDL_Surface* player_resting_1_surface = SDL_LoadBMP("dazai_resting_1.bmp");
    SDL_Surface* player_resting_2_surface = SDL_LoadBMP("dazai_resting_2.bmp");
    SDL_Surface* player_walking_1_surface = SDL_LoadBMP("dazai_walking_1.bmp");
    SDL_Surface* player_walking_2_surface = SDL_LoadBMP("dazai_walking_2.bmp");
    SDL_Surface* chuuya_resting_surface = SDL_LoadBMP("chuuya_resting.bmp");
    SDL_Surface* chuuya_aggressiv_surface = SDL_LoadBMP("chuuya_aggressiv.bmp");
    SDL_Surface* player_fighting_right_1 = SDL_LoadBMP("Dazai_fighting_right_1.bmp");
    SDL_Surface* player_fighting_right_2 = SDL_LoadBMP("Dazai_fighting_right_2.bmp");
    SDL_Surface* player_fighting_right_3 = SDL_LoadBMP("Dazai_fighting_right_3.bmp");
    SDL_Surface* player_fighting_left_1 = SDL_LoadBMP("Dazai_fighting_left_1.bmp");
    SDL_Surface* player_fighting_left_2 = SDL_LoadBMP("Dazai_fighting_left_2.bmp");
    SDL_Surface* player_fighting_left_3 = SDL_LoadBMP("Dazai_fighting_left_3.bmp");
    SDL_Surface* chuuya_fighting_right_1 = SDL_LoadBMP("chuuya_fighting_right_1.bmp");
    SDL_Surface* chuuya_fighting_right_2 = SDL_LoadBMP("chuuya_fighting_right_2.bmp");
    SDL_Surface* chuuya_fighting_right_3 = SDL_LoadBMP("chuuya_fighting_right_3.bmp");
    SDL_Surface* chuuya_fighting_left_1 = SDL_LoadBMP("chuuya_fighting_left_1.bmp");
    SDL_Surface* chuuya_fighting_left_2 = SDL_LoadBMP("chuuya_fighting_left_2.bmp");
    SDL_Surface* chuuya_fighting_left_3 = SDL_LoadBMP("chuuya_fighting_left_3.bmp");
	SDL_Surface* shooting1P_1_surface = SDL_LoadBMP("Shooting1P_1.bmp");
	SDL_Surface* shooting1P_2_surface = SDL_LoadBMP("Shooting1P_2.bmp");


    // Check if surfaces loaded successfully
    if (!backround_surface || !player_resting_1_surface || !player_resting_2_surface || !player_walking_1_surface ||
        !player_walking_2_surface || !chuuya_resting_surface || !chuuya_aggressiv_surface || !player_fighting_right_1 ||
        !player_fighting_right_2 || !player_fighting_right_3 || !player_fighting_left_1 || !player_fighting_left_2 ||
        !player_fighting_left_3 || !chuuya_fighting_right_1 || !chuuya_fighting_right_2 || !chuuya_fighting_right_3 ||
        !chuuya_fighting_left_1 || !chuuya_fighting_left_2 || !chuuya_fighting_left_3||!shooting1P_1_surface||!shooting1P_2_surface) {
		consoleout("[SYSTEM]>>Surface Load Error: " + string(SDL_GetError()));
        return 1;
    }

    // Create Textures from Surfaces
    SDL_Texture* backround_texture = SDL_CreateTextureFromSurface(renderer, backround_surface);
    SDL_Texture* player_resting_1_texture = SDL_CreateTextureFromSurface(renderer, player_resting_1_surface);
    SDL_Texture* player_resting_2_texture = SDL_CreateTextureFromSurface(renderer, player_resting_2_surface);
    SDL_Texture* player_walking_1_texture = SDL_CreateTextureFromSurface(renderer, player_walking_1_surface);
    SDL_Texture* player_walking_2_texture = SDL_CreateTextureFromSurface(renderer, player_walking_2_surface);
    SDL_Texture* chuuya_resting_texture = SDL_CreateTextureFromSurface(renderer, chuuya_resting_surface);
    SDL_Texture* chuuya_aggressiv_texture = SDL_CreateTextureFromSurface(renderer, chuuya_aggressiv_surface);
    SDL_Texture* player_fighting_right_1_texture = SDL_CreateTextureFromSurface(renderer, player_fighting_right_1);
    SDL_Texture* player_fighting_right_2_texture = SDL_CreateTextureFromSurface(renderer, player_fighting_right_2);
    SDL_Texture* player_fighting_right_3_texture = SDL_CreateTextureFromSurface(renderer, player_fighting_right_3);
    SDL_Texture* player_fighting_left_1_texture = SDL_CreateTextureFromSurface(renderer, player_fighting_left_1);
    SDL_Texture* player_fighting_left_2_texture = SDL_CreateTextureFromSurface(renderer, player_fighting_left_2);
    SDL_Texture* player_fighting_left_3_texture = SDL_CreateTextureFromSurface(renderer, player_fighting_left_3);
    SDL_Texture* chuuya_fighting_right_1_texture = SDL_CreateTextureFromSurface(renderer, chuuya_fighting_right_1);
    SDL_Texture* chuuya_fighting_right_2_texture = SDL_CreateTextureFromSurface(renderer, chuuya_fighting_right_2);
    SDL_Texture* chuuya_fighting_right_3_texture = SDL_CreateTextureFromSurface(renderer, chuuya_fighting_right_3);
    SDL_Texture* chuuya_fighting_left_1_texture = SDL_CreateTextureFromSurface(renderer, chuuya_fighting_left_1);
    SDL_Texture* chuuya_fighting_left_2_texture = SDL_CreateTextureFromSurface(renderer, chuuya_fighting_left_2);
    SDL_Texture* chuuya_fighting_left_3_texture = SDL_CreateTextureFromSurface(renderer, chuuya_fighting_left_3);
	SDL_Texture* shooting1P_1_texture = SDL_CreateTextureFromSurface(renderer, shooting1P_1_surface);
	SDL_Texture* shooting1P_2_texture = SDL_CreateTextureFromSurface(renderer, shooting1P_2_surface);




    // Check if textures created successfully
    if (!backround_texture || !player_resting_1_texture || !player_resting_2_texture || !player_walking_1_texture ||
        !player_walking_2_texture || !chuuya_resting_texture || !chuuya_aggressiv_texture || !player_fighting_right_1_texture ||
        !player_fighting_right_2_texture || !player_fighting_right_3_texture || !player_fighting_left_1_texture ||
        !player_fighting_left_2_texture || !player_fighting_left_3_texture || !chuuya_fighting_right_1_texture ||
        !chuuya_fighting_right_2_texture || !chuuya_fighting_right_3_texture || !chuuya_fighting_left_1_texture ||
        !chuuya_fighting_left_2_texture || !chuuya_fighting_left_3_texture|| !shooting1P_1_texture||!shooting1P_2_texture) {
		consoleout("[SYSTEM]>>Texture Creation Error: " + string(SDL_GetError()));
        return 1;
    }

    // Define Rectangles for Player, Chuuya, and Background
    SDL_Rect player_rect = { 100, 100, player_resting_1_surface->w, player_resting_1_surface->h };
    SDL_Rect chuuya_rect = { 800, 800, chuuya_resting_surface->w, chuuya_resting_surface->h };
    SDL_Rect backround_rect = { 0, 0, backround_surface->w, backround_surface->h };
    SDL_Point chuuyaRestingCenter = { chuuya_rect.w / 2, chuuya_rect.h / 2 };
    SDL_Point playerRestingCenter = { player_rect.w / 2, player_rect.h / 2 };

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

    // Event Handling
    SDL_Event event;
    currenttime = SDL_GetTicks();
    cout << "[SYSTEM]>>Startup finished after:" << currenttime - general_time << "ms " << "at:" << get_current_time_string() << endl;
    if (!Debug)Intro(renderer);
    cout << "[SYSTEM]>>Program loop started\n";
    gamestatus = 1;//for testing purposes





























































    // Main Game Loop
     while (!quit) {
        switch(gamestatus)
        { 
        case 1:

        walking = false;
        currenttime = SDL_GetTicks();
           // Handle Events
           while (SDL_PollEvent(&event)) {
              if (event.type == SDL_QUIT) {
                quit = true;
              }
              else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                case SDLK_w: walking = true; player_rect.y -= 10; Log("player coordinates changed to:" + to_string(player_rect.x) + "," + to_string(player_rect.y)); break;
                case SDLK_s: walking = true; player_rect.y += 10; Log("player coordinates changed to:" + to_string(player_rect.x) + "," + to_string(player_rect.y)); break;
                case SDLK_a: walking = true; player_rect.x -= 10; Log("player coordinates changed to:" + to_string(player_rect.x) + "," + to_string(player_rect.y)); break;
                case SDLK_d: walking = true; player_rect.x += 10; Log("player coordinates changed to:" + to_string(player_rect.x) + "," + to_string(player_rect.y)); break;
                case SDLK_e: if (!fighting) { fighting = true; fighting_time = SDL_GetTicks(); hit_took = false; } break;
                case SDLK_ESCAPE: play_exit_animation(renderer); quit = true; break;
                case SDLK_c: console = true; break;
                }
              }
           }

        // Console Handling
        if (console) {
            quit2 = false;
            SetForegroundWindow(consoleWindow);
            cout << "[CONSOLE]>>";
            while (!quit2) {
                
                cin >> command;
                if (command == "exit") {
                    cout << "[CONSOLE]>>Quitted Console\n";
                    quit2 = true;
                }
                else if (command == "help") {
                    cout << "[CONSOLE]>>Commands:\n";
                    cout << "[CONSOLE]>>exit: exits the console\n";
                    cout << "[CONSOLE]>>help: shows this message\n";
                    cout << "[CONSOLE]>>showlog: shows the log\n";
                    cout << "[CONSOLE]>>log: put message into log\n";
                    cout << "[CONSOLE]>>gamestatus: set gamestatus\n";
                    cout << "[CONSOLE]>>";

                }
                else if (command == "log") {
                    // Handle log command
					cout << "[CONSOLE]>>Enter message to log(use '_' if you need to write sentences:";
                    cin >> placeholder;
					Log(placeholder);
					cout << "\n[CONOSOLE]Message logged\n";
                    cout << "[CONSOLE]>>";
                }
                else if (command == "showlog")
                {
					cout << "[CONSOLE]>>\n" << ReadLogFileToString() << endl;
					cout << "[CONSOLE]>>";


                }
                else if (command == "gamestatus")
                {
                    cout << "[CONSOLE]>>Current Game Status=" + to_string(gamestatus) + "\nEnter new gamestatus value(int) :";
                    cin >> gamestatus;
					consoleout("[CONSOLE]>>Gamestatus set to:" + to_string(gamestatus) + "\n");
                    consoleout("[CONSOLE]>>");
                }
                else {
                    cout << "[CONSOLE]>>Command not found\n";
                    cout << "[CONSOLE]>>";
                }
            }
            console = false;
        }

        // Update Frame States
        if (currenttime - LastFrameSwitch_resting > 400) {
            display_frame1_resting = !display_frame1_resting;
            LastFrameSwitch_resting = currenttime;
        }
        if (currenttime - LastFrameSwitch_walking > 200 && walking) {
            display_frame1_walking = !display_frame1_walking;
            LastFrameSwitch_walking = currenttime;
        }
        if (fighting && currenttime - fighting_time > 200) {
            fighting_frame_player++;
            fighting_time = currenttime;
            if (fighting_frame_player > 3) {
                fighting = false;
                fighting_frame_player = 1;
            }
        }
        if (chuuya_fighting && currenttime - Chuuya_fighting_time > 200) {
            fighting_frame_chuuya++;
            Chuuya_fighting_time = currenttime;
            if (fighting_frame_chuuya > 3) {
                fighting_frame_chuuya = 1;
            }
        }

        // Render Background
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, backround_texture, 0, &backround_rect);

        // Render Player
        if (walking) {
            if (display_frame1_walking)
                SDL_RenderCopy(renderer, player_walking_1_texture, NULL, &player_rect);
            else
                SDL_RenderCopy(renderer, player_walking_2_texture, NULL, &player_rect);
        }
        else if (!fighting) {
            if (display_frame1_resting)
                SDL_RenderCopy(renderer, player_resting_1_texture, NULL, &player_rect);
            else
                SDL_RenderCopy(renderer, player_resting_2_texture, NULL, &player_rect);
        }
        else {
            if (chuuya_rect.x > player_rect.x) {
                switch (fighting_frame_player) {
                case 1: SDL_RenderCopy(renderer, player_fighting_right_1_texture, NULL, &player_rect); break;
                case 2: SDL_RenderCopy(renderer, player_fighting_right_2_texture, NULL, &player_rect); break;
                case 3: SDL_RenderCopy(renderer, player_fighting_right_3_texture, NULL, &player_rect); break;
                }
            }
            else {
                switch (fighting_frame_player) {
                case 1: SDL_RenderCopy(renderer, player_fighting_left_1_texture, NULL, &player_rect); break;
                case 2: SDL_RenderCopy(renderer, player_fighting_left_2_texture, NULL, &player_rect); break;
                case 3: SDL_RenderCopy(renderer, player_fighting_left_3_texture, NULL, &player_rect); break;
                }
            }
        }

        if (currenttime - abilitlycountdown > 300)
        {
            if(playerability<115)playerability = playerability + 5;
			if(chuuyaability<115)chuuyaability = chuuyaability + 3;
            abilitlycountdown = currenttime;

        }
		
        render_abilitly_meter(renderer, playerability, player_rect);
		render_abilitly_meter(renderer, chuuyaability, chuuya_rect);

       


        // Render Chuuya
        if (Is_within_range(player_rect, chuuya_rect, 300)) {
            chuuya_aggressiv = true;
        }

        if (chuuyahealth > 0) {
            if (!chuuya_fighting) {
                if (chuuya_aggressiv) {
                    SDL_RenderCopy(renderer, chuuya_aggressiv_texture, NULL, &chuuya_rect);
                }
                else {
                    SDL_RenderCopy(renderer, chuuya_resting_texture, NULL, &chuuya_rect);
                }
            }
            else {
                if (player_rect.x > chuuya_rect.x) {
                    switch (fighting_frame_chuuya) {
                    case 1: SDL_RenderCopy(renderer, chuuya_fighting_right_1_texture, NULL, &chuuya_rect); break;
                    case 2: SDL_RenderCopy(renderer, chuuya_fighting_right_2_texture, NULL, &chuuya_rect); break;
                    case 3: SDL_RenderCopy(renderer, chuuya_fighting_right_3_texture, NULL, &chuuya_rect); break;
                    }
                }
                else {
                    switch (fighting_frame_chuuya) {
                    case 1: SDL_RenderCopy(renderer, chuuya_fighting_left_1_texture, NULL, &chuuya_rect); break;
                    case 2: SDL_RenderCopy(renderer, chuuya_fighting_left_2_texture, NULL, &chuuya_rect); break;
                    case 3: SDL_RenderCopy(renderer, chuuya_fighting_left_3_texture, NULL, &chuuya_rect); break;
                    }
                }
            }

            // Handle Chuuya's Health and Fighting
            if (Is_within_range(player_rect, chuuya_rect, 50) && fighting && !hit_took) {
                hit_took = true;
                chuuyahealth -= 10;
                Log("[GAME]>>Enemy: 'Chuuya' took a hit!");
            }
            if (chuuyahealth <= 0 && chuuya_rect.x != 9999) {
                Log("[GAME]>>Enemy: 'Chuuya' is defeated!");
                chuuya_aggressiv = false;
                chuuya_rect.x = 9999;
            }

            if(playerhealth <=0)
			{
				Log("[GAME]>>Player: 'Dazai' is defeated!");
				play_exit_animation(renderer);
				quit = true;
			}

            if (Is_within_range(player_rect, chuuya_rect, 50)) {
                chuuya_fighting = true;
            }
            else {
                chuuya_fighting = false;
            }

            if (chuuya_fighting&&currenttime-Chuuya_fighting_cooldown>500) {
                playerhealth -= 10;
                Log("[GAME]>>Player: 'Dazai' took a hit!");
				Chuuya_fighting_cooldown = currenttime;
            }

            if (chuuya_aggressiv && currenttime - Chuuya_walking_time > 200 && !Is_within_range(chuuya_rect, player_rect, 50)) {
                if (chuuya_rect.x > player_rect.x) chuuya_rect.x -= 10;
                else if (chuuya_rect.x < player_rect.x) chuuya_rect.x += 10;
                else if (chuuya_rect.y > player_rect.y) chuuya_rect.y -= 10;
                else if (chuuya_rect.y < player_rect.y) chuuya_rect.y += 10;

                Chuuya_walking_time = currenttime;
            }

    
            if (chuuyaability>110&&Is_within_range(player_rect, chuuya_rect,50))
            {
                //play ability animation 
                SDL_RenderCopy(renderer, backround_texture, 0, &backround_rect);
                SDL_RenderCopy(renderer, chuuya_resting_texture, NULL, &chuuya_rect);
                for (size_t i = 0; i < 720; i++)
                {
                    SDL_RenderCopy(renderer, backround_texture, 0, &backround_rect);
                    SDL_RenderCopy(renderer, chuuya_resting_texture, NULL, &chuuya_rect);
                    SDL_RenderCopyEx(renderer, player_resting_1_texture, 0, &player_rect, i, &playerRestingCenter, SDL_FLIP_NONE);
                    SDL_RenderPresent(renderer);
                    Sleep(1);
                }

				playerhealth -= 20;
				chuuyaability = 0;


            }




        }


        //Debug handling 
        if (Debug&&currenttime-Debug_time>500)
        {
            cout << "[DEBUG]>>Gametime:" << SDL_GetTicks()<<endl;
			cout << "[DEBUG]>>Player coordinates:" << player_rect.x << "," << player_rect.y << endl;
			cout << "[DEBUG]>>Player health:" << playerhealth << endl;
			cout << "[DEBUG]>>Chuuya coordinates:" << chuuya_rect.x << "," << chuuya_rect.y << endl;
			cout << "[DEBUG]>>Chuuya health:" << chuuyahealth << endl;
			cout << "[DEBUG]>>Chuuya_aggressiv:" << chuuya_aggressiv << endl;
			cout << "[DEBUG]>>Distance between player and chuuya:" << Get_distance_between_rects(player_rect, chuuya_rect) << endl;
			cout << "[DEBUG]>>Playerabilitymeter:" << playerability << endl;
			cout << "[DEBUG]>>Chuuyaabilitymeter:" << chuuyaability << endl;
			cout << "[DEBUG]>>This message will be displayed again in 500ms\n\n";
            
			Debug_time = currenttime;


        }



        // Present Renderer
        SDL_RenderPresent(renderer);

        break;
        default:
			consoleout("[GAME]>>Error: Unknown gamestatus value:" + to_string(gamestatus) + "\nShutting down...");
            quit = true;
     }
    }
    play_exit_animation(renderer);
	cout << "[SYSTEM]>>Exited Game Loop. Starting cleanup at:"<<get_current_time_string()<<endl;
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

    SDL_FreeSurface(backround_surface);
    SDL_FreeSurface(player_resting_1_surface);
    SDL_FreeSurface(player_resting_2_surface);
    SDL_FreeSurface(player_walking_1_surface);
    SDL_FreeSurface(player_walking_2_surface);
    SDL_FreeSurface(chuuya_resting_surface);
    SDL_FreeSurface(chuuya_aggressiv_surface);
    SDL_FreeSurface(player_fighting_right_1);
    SDL_FreeSurface(player_fighting_right_2);
    SDL_FreeSurface(player_fighting_right_3);
    SDL_FreeSurface(player_fighting_left_1);
    SDL_FreeSurface(player_fighting_left_2);
    SDL_FreeSurface(player_fighting_left_3);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    currenttime = SDL_GetTicks();

    cout << "[SYSTEM]>>Cleanup finished at:" << get_current_time_string() << endl; 
	cout << "[SYSTEM]>>Shutting down. Bye Bye!\n";
    Log("Program ended properly");
    return 0;
}