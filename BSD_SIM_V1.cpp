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

TODO:
|->Buxfixes needed: Give Abilitybar final tweaks; also make it acutally useful aka add ability bar for chuuya and give him a ability for dazai to nullifiy
|->Move all the .bmp texture files to a seperate folder to clear up the main folder. (looks horrible in git)
|->Create config (that can be written to using the ingame console menu) for things like other exit animations etc.
|->Make chuuyas ability better, buff chuuya!
|->HEALTH BAR IMPORTANT
|->Create a GAME!->dazai walking aroung, shooting first person at enemys etc..
|->Add exit crash animaton
|->convert all bmp to png to save space and have transparency (load via IMG_loadtexture)
|->resize to 1024x1024 (PoT)
|->ADD PROPER LOGGING AND DEBUG OUPUT FOR GODS SAKE
|->CONVERT ALL COUT TO CONSOLEOUT!!!!!!!!!!!!!!!!!!!
******************************************************************************************/

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
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
    //Startup
    HWND consoleWindow = GetConsoleWindow();
    CreateLog();
    std::cout << "BSD_SIM_V1.0\n";
	std::cout << "[DEBUG]>>Debug Mode, y/n?\n";
	cin>>placeholder;
    if (placeholder == "y")
    {
        Debug = true;
        std::cout << "[DEBUG]>>Debug Mode activated\n"; 
		Log("Debug activated");
    }
    else {
        Debug = false;
        std::cout << "[DEBUG]>>Debug Mode NOT activated\n";
        Log("Debug not activated");
    }


    std::cout << "[SYSTEM]>>Startup started at:" << get_current_time_string() << endl;

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



    // Check if surfaces loaded successfully
    if (!backround_surface || !player_resting_1_surface || !player_resting_2_surface || !player_walking_1_surface ||
        !player_walking_2_surface || !chuuya_resting_surface || !chuuya_aggressiv_surface || !player_fighting_right_1 ||
        !player_fighting_right_2 || !player_fighting_right_3 || !player_fighting_left_1 || !player_fighting_left_2 ||
        !player_fighting_left_3 || !chuuya_fighting_right_1 || !chuuya_fighting_right_2 || !chuuya_fighting_right_3 ||
        !chuuya_fighting_left_1 || !chuuya_fighting_left_2 || !chuuya_fighting_left_3) {
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
    SDL_Texture* shooting1P_1_texture = IMG_LoadTexture(renderer, "Shooting1P_1.png");
	SDL_Texture* shooting1P_2_texture = IMG_LoadTexture(renderer, "Shooting1P_2.png");




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

    // Define Rectangles for Player, Chuuya, and Background (surface bound->for now)
    SDL_Rect player_rect = { 100, 100, player_resting_1_surface->w, player_resting_1_surface->h };
    SDL_Rect chuuya_rect = { 800, 800, chuuya_resting_surface->w, chuuya_resting_surface->h };
    SDL_Rect backround_rect = { 0, 0, backround_surface->w, backround_surface->h };
    SDL_Point chuuyaRestingCenter = { chuuya_rect.w / 2, chuuya_rect.h / 2 };
    SDL_Point playerRestingCenter = { player_rect.w / 2, player_rect.h / 2 };
    
    //Defining rects for shooting "animation" (texture bound->better)
    SDL_QueryTexture(shooting1P_1_texture, NULL, NULL, &textureW, &textureH);
	SDL_Rect shooting1P_rect = { 620, 676, textureW, textureH };//no need for a rect for the 2 frame because the dimesions are the same for both frames



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
    std::cout << "[SYSTEM]>>Startup finished after:" << currenttime - general_time << "ms " << "at:" << get_current_time_string() << endl;
    if (!Debug)Intro(renderer);
    std::cout << "[SYSTEM]>>Program loop started\n";
    gamestatus = 2;//for testing purposes





























































    // Main Game Loop
    quit = false;
     while (!quit) {
         currenttime = SDL_GetTicks();
        switch(gamestatus)
        { 


        case 0: //console 

            quit2 = false;
            SetForegroundWindow(consoleWindow);
            std::cout << "[CONSOLE]>>";
            while(!quit2)
            {
                    cin >> command;
                    if (command == "exit") {
                        if(gamestatus!=0)
                        {
                        std::cout << "[CONSOLE]>>Quitted Console\n";
                        quit2 = true;
                        }
                        else
                        {
							cout << "[CONSOLE]>>You cannot exit the console while in console mode! Change gamestatus first via the gamestatus command.\n";
							cout << "[CONSOLE]>>";
                        }
                    }
                    else if (command == "help") {
                        std::cout << "[CONSOLE]>>Commands:\n";
                        std::cout << "[CONSOLE]>>exit: exits the console\n";
                        std::cout << "[CONSOLE]>>help: shows this message\n";
                        std::cout << "[CONSOLE]>>showlog: shows the log\n";
                        std::cout << "[CONSOLE]>>log: put message into log\n";
                        std::cout << "[CONSOLE]>>gamestatus: set gamestatus\n";
						std::cout << "[CONSOLE]>>fps: set fps limit\n";
                        std::cout << "[CONSOLE]>>";

                    }
                    else if (command == "fps") {
						std::cout << "[CONSOLE]>>Current FPS Limit=" + to_string(fpsLimit) + "\nEnter new FPS limit value(int) :";
						cin >> fpsLimit;
						consoleout("\n[CONSOLE]>>FPS limit set to:" + to_string(fpsLimit) + "\n");
						std::cout << "[CONSOLE]>>";
                    }
                    else if (command == "log") {
                        // Handle log command
                        std::cout << "[CONSOLE]>>Enter message to log(use '_' if you need to write sentences:";
                        cin >> placeholder;
                        Log(placeholder);
                        std::cout << "\n[CONOSOLE]Message logged\n";
                        std::cout << "[CONSOLE]>>";
                    }
                    else if (command == "showlog")
                    {
                        std::cout << "[CONSOLE]>>\n" << ReadLogFileToString() << endl;
                        std::cout << "[CONSOLE]>>";


                    }
                    else if (command == "gamestatus")
                    {
                        std::cout << "[CONSOLE]>>Current Game Status=" + to_string(gamestatus) + "\nEnter new gamestatus value(int) :";
                        cin >> gamestatus;
                        consoleout("[CONSOLE]>>Gamestatus set to:" + to_string(gamestatus) + "\n");
                        consoleout("[CONSOLE]>>");
                    }
                    else {
                        std::cout << "[CONSOLE]>>Command not found\n";
                        std::cout << "[CONSOLE]>>";
                    }
                
              
            }





















        case 1:
            if (currenttime - fpsLimitTimer > (1000 / fpsLimit))
            {
				fpsLimitTimer = currenttime;
                walking = false;
                // Handle Events
                while (SDL_PollEvent(&event)) {
                    if (event.type == SDL_QUIT) {
                        quit = true;

                    }
                    else if (event.type == SDL_KEYDOWN) {
                        switch (event.key.keysym.sym) {
                        case SDLK_w: if (player_rect.y > 10) { walking = true; player_rect.y -= 10; Log("player coordinates changed to:" + to_string(player_rect.x) + "," + to_string(player_rect.y)); } break;
                        case SDLK_s: if (player_rect.y < 950){walking = true; player_rect.y += 10; Log("player coordinates changed to:" + to_string(player_rect.x) + "," + to_string(player_rect.y));} break;
                        case SDLK_a: if(player_rect.x>10){walking = true; player_rect.x -= 10; Log("player coordinates changed to:" + to_string(player_rect.x) + "," + to_string(player_rect.y));} break;
                        case SDLK_d: if (player_rect.x < 900){walking = true; player_rect.x += 10; Log("player coordinates changed to:" + to_string(player_rect.x) + "," + to_string(player_rect.y));} break;
                        case SDLK_e: if (!fighting) { fighting = true; fighting_time = SDL_GetTicks(); hit_took = false; } break;
                        case SDLK_ESCAPE: play_exit_animation(renderer); quit = true; break;
                        case SDLK_c: gamestatus = 0; break;
                        }
                    }
                }

                // Render Background
                SDL_RenderClear(renderer);
                SDL_RenderCopy(renderer, backround_texture, 0, &backround_rect);
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
                    if (playerability < 115)playerability = playerability + 5;
                    if (chuuyaability < 115)chuuyaability = chuuyaability + 3;
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

                    if (playerhealth <= 0)
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

                    if (chuuya_fighting && currenttime - Chuuya_fighting_cooldown > 500) {
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


                    if (chuuyaability > 110 && Is_within_range(player_rect, chuuya_rect, 50))
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


                //fps debug prep
                if (currenttime - fpsCounterTimer > 1000)
                {
                    lastFpsCount = fpsCounter;
                    fpsCounter = 0;
                    fpsCounterTimer = currenttime;
                }


                //Debug handling 
                if (Debug && currenttime - Debug_time > 500)
                {
                    std::cout << "[DEBUG]>>Gametime:" << SDL_GetTicks() << endl;
                    std::cout << "[DEBUG]>>FPS:" << lastFpsCount << endl;
                    std::cout << "[DEBUG]>>RAM Usage:" << GetMemoryUsage() << " MB" << endl;
                    std::cout << "[DEBUG]>>Player coordinates:" << player_rect.x << "," << player_rect.y << endl;
                    std::cout << "[DEBUG]>>Player health:" << playerhealth << endl;
                    std::cout << "[DEBUG]>>Chuuya coordinates:" << chuuya_rect.x << "," << chuuya_rect.y << endl;
                    std::cout << "[DEBUG]>>Chuuya health:" << chuuyahealth << endl;
                    std::cout << "[DEBUG]>>Chuuya_aggressiv:" << chuuya_aggressiv << endl;
                    std::cout << "[DEBUG]>>Distance between player and chuuya:" << Get_distance_between_rects(player_rect, chuuya_rect) << endl;
                    std::cout << "[DEBUG]>>Playerabilitymeter:" << playerability << endl;
                    std::cout << "[DEBUG]>>Chuuyaabilitymeter:" << chuuyaability << endl;
                    std::cout << "[DEBUG]>>This message will be displayed again in 500ms\n\n";
                    Debug_time = currenttime;


                }



                // Present Renderer
                SDL_RenderPresent(renderer);
                fpsCounter++;
            }
        break;











        case 2://Shooting game first person - Practice 
            quit2 = false;
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        while (!quit2)
        {
			currenttime = SDL_GetTicks();
            if (currenttime - fpsLimitTimer > (1000 / fpsLimit))
            {
                fpsLimitTimer = currenttime;
                currenttime = SDL_GetTicks();
                SDL_RenderCopy(renderer, shooting1P_1_texture, 0, &shooting1P_rect);

                while (SDL_PollEvent(&event)) {
                    if (event.type == SDL_QUIT) {
                        quit = true;
                    }

                    else if (event.type == SDL_KEYDOWN) {
                        switch (event.key.keysym.sym) {
                        case SDLK_c: gamestatus = 0; quit2 = true; break;
                        case SDLK_ESCAPE:play_exit_animation(renderer); quit = true; break;
                        case SDLK_w: if (shooting1P_rect.y > 300) shooting1P_rect.y -= 100; SDL_RenderClear(renderer); break;
                        case SDLK_s: if (shooting1P_rect.y < 676) shooting1P_rect.y += 100; SDL_RenderClear(renderer); break;
                        case SDLK_a: if (shooting1P_rect.x > 300) shooting1P_rect.x -= 100; SDL_RenderClear(renderer); break;
                        case SDLK_d: if (shooting1P_rect.x < 620) shooting1P_rect.x += 100; SDL_RenderClear(renderer); break;
                        case SDLK_e://shoot
                            if (currenttime - shootingcooldown > 200)
                            {
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

                //fps debug prep
                if (currenttime - fpsCounterTimer > 1000)
                {
                    lastFpsCount = fpsCounter;
                    fpsCounter = 0;
                    fpsCounterTimer = currenttime;
                }


                if (Debug && currenttime - Debug_time > 500)
                {
                    std::cout << "[DEBUG]>>Gametime:" << SDL_GetTicks() << endl;
                    std::cout << "[DEBUG]>>FPS:" << lastFpsCount << endl;
                    std::cout << "[DEBUG]>>RAM Usage:" << GetMemoryUsage() << " MB" << endl;
                    std::cout << "[DEBUG]>>This message will be displayed again in 500ms\n\n";
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
	std::cout << "[SYSTEM]>>Exited Game Loop. Starting cleanup at:"<<get_current_time_string()<<endl;
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

    std::cout << "[SYSTEM]>>Cleanup finished at:" << get_current_time_string() << endl; 
	std::cout << "[SYSTEM]>>Shutting down. Bye Bye!\n";
    Log("Program ended properly");
    return 0;
}