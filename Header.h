#pragma once
#include <iostream>
#include <string>
using namespace std;

extern void Log(const string& message);
extern bool Is_within_range(const SDL_Rect& rect1, const SDL_Rect& rect2, int range);
extern void play_exit_animation(SDL_Renderer* renderer);
extern void CreateLog();
extern string get_current_time_string();
extern int Get_distance_between_rects(const SDL_Rect& rect1, const SDL_Rect& rect2);
extern string ReadLogFileToString();
extern Uint32 GetTimeDifference(Uint32 current, Uint32 previous);
extern void render_abilitly_meter(SDL_Renderer* renderer, int value, SDL_Rect Rect);
extern void consoleout(string message);
extern bool overwriteToFile(const std::string& filename, const std::string& content);
extern string read_file_to_string(string file_path);
extern void Intro(SDL_Renderer* renderer);
extern float GetMemoryUsage();
extern string get_current_clock_string();