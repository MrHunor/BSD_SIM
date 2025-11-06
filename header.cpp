#pragma warning(disable : 4996)

#include <string>
#include <fstream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <SDL2_gfxPrimitives.h>
#include <random>
#include <filesystem>
using namespace std;
namespace fs = std::filesystem;

void DrawFilledCircle(SDL_Renderer* renderer, int cx, int cy, int r) {
	int dx, dy;
	for (dy = -r; dy <= r; dy++) {
		dx = (int)sqrt(r * r - dy * dy);
		SDL_RenderDrawLine(renderer, cx - dx, cy + dy, cx + dx, cy + dy);
	}
}




int random(int lower_bound, int upper_bound)
{
	static std::random_device rd;  // seed generator
	static std::mt19937 gen(rd()); // Mersenne Twister engine

	std::uniform_int_distribution<> dis(lower_bound, upper_bound);
	return dis(gen);
}

void Intro(SDL_Renderer* renderer)
{

	for (size_t i = 0; i < 5; i++)
	{
		SDL_SetRenderDrawColor(renderer, random(0, 255), random(0, 255), random(0, 255), 0);
		for (size_t i = 0; i < 1000; i++)
		{
			SDL_RenderDrawLine(renderer, 0, i, 1000, i);
		}
		SDL_RenderPresent(renderer);
		SDL_SetRenderDrawColor(renderer, random(0, 255), random(0, 255), random(0, 255), 0);
		for (size_t i = 0; i < 710; i=i+2)
		{

			DrawFilledCircle(renderer, 500, 500, i);
			SDL_Delay(1);
			SDL_RenderPresent(renderer);
		}
	}


}





















string get_current_time_string() {
	auto now = chrono::system_clock::now();
	auto in_time_t = chrono::system_clock::to_time_t(now);
	stringstream ss;
	ss << put_time(localtime(&in_time_t), "%Y-%m-%d|%H-%M-%S");
	return ss.str();
}


// Writes the given content to a file in the executing directory.
// If the file already exists, its contents will be overwritten.
//return 0 on sucess, 1 on failure
bool overwriteToFile(const std::string& filename, const std::string& content) {
	std::ofstream outFile(filename, std::ios::trunc); // std::ios::trunc ensures overwrite
	if (!outFile) {
		return 1;
	}
	outFile << content;
	outFile.close();
	return 0;
}




string read_file_to_string(string file_path)
{
	string filestring;

	ifstream filehandler(file_path);

	if (!filehandler.is_open())
	{
		cout << "Error opening file" << endl;
		return NULL;
	}
	filehandler >> filestring;
	filehandler.close();
	if (filehandler.is_open())
	{
		cout << "Error closing file" << endl;
		return NULL;
	}


	return filestring;

}


void render_abilitly_meter(SDL_Renderer* renderer, int value, SDL_Rect Rect)
{
	int drawvalue;

	//Draw Border
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderDrawLine(renderer, Rect.x, Rect.y + Rect.h + 5, Rect.x + Rect.w, Rect.y + Rect.h + 5);
	SDL_RenderDrawLine(renderer, Rect.x, Rect.y + Rect.h + 6, Rect.x + Rect.w, Rect.y + Rect.h + 6);
	SDL_RenderDrawLine(renderer, Rect.x, Rect.y + Rect.h + 7, Rect.x + 5, Rect.y + Rect.h + 7);
	SDL_RenderDrawLine(renderer, Rect.x, Rect.y + Rect.h + 8, Rect.x + 5, Rect.y + Rect.h + 8);
	SDL_RenderDrawLine(renderer, Rect.x, Rect.y + Rect.h + 9, Rect.x + Rect.w, Rect.y + Rect.h + 9);
	SDL_RenderDrawLine(renderer, Rect.x, Rect.y + Rect.h + 10, Rect.x + Rect.w, Rect.y + Rect.h + 10);
	SDL_RenderDrawLine(renderer, Rect.x + Rect.w - 5, Rect.y + Rect.h + 7, Rect.x + Rect.w, Rect.y + Rect.h + 7);
	SDL_RenderDrawLine(renderer, Rect.x + Rect.w - 5, Rect.y + Rect.h + 8, Rect.x + Rect.w, Rect.y + Rect.h + 8);



	//Draw Actual Bar
	drawvalue = round((value / 100.0f) * (Rect.w - 10));
	SDL_SetRenderDrawColor(renderer, 83, 195, 189, 0);
	SDL_RenderDrawLine(renderer, Rect.x + 6, Rect.y + Rect.h + 7, drawvalue + Rect.x, Rect.y + Rect.h + 7);
	SDL_RenderDrawLine(renderer, Rect.x + 6, Rect.y + Rect.h + 8, drawvalue + Rect.x, Rect.y + Rect.h + 8);

}











std::string getLineFromFile(const std::string& filePath, int lineNumber) {
	std::ifstream file(filePath); // Open the file
	if (!file.is_open()) {
		cout << "Could not open the file: " + filePath + "(Error in getLineFromFile)"<<endl;
		exit(1);
	}

	std::string line;
	int currentLine = 0;

	// Read lines until the desired line number is reached
	while (std::getline(file, line)) {
		if (++currentLine == lineNumber) {
			return line; // Return the line when lineNumber is matched
		}
	}

	cout << "Line number exceeds the total lines in the file.(Error in getLineFromFile)" << endl;
	exit(1);
}


void CreateLog()//god thank chatgpt
{
	// Make sure the log directory exists
     filesystem::create_directory("log");

	// Read the last log number
	int logNumber = 0;
	try {
		logNumber = stoi(getLineFromFile("log.txt", 1));
	}
	catch (...) {
		logNumber = 0;
	}

	// Increment it for the new log
	int newLogNumber = logNumber + 1;

	// File paths
	string tempFile = "log_new.txt";
	string mainLog = "log.txt";
	string archivedLog = "log/log_" + to_string(newLogNumber) + ".txt";

	// Write the new log info to the archived file
	ofstream archived(archivedLog);
	if (archived.is_open()) {
		archived << "Log #" << newLogNumber << endl;
		archived << "Current Time: " << get_current_time_string() << endl;
		archived.close();
	}
	else {
		cerr << "Error: could not write to " << archivedLog << endl;
	}

	// Update log.txt with the latest number
	ofstream logFile(tempFile);
	logFile << newLogNumber << endl;
	logFile << "Last log saved to: " << archivedLog << endl;
	logFile.close();

	remove(mainLog.c_str());
	if (rename(tempFile.c_str(), mainLog.c_str()) != 0)
	{
		cerr << "Error renaming " << tempFile << " to " << mainLog << endl;
	}

	cout << "Created log file: " << archivedLog << endl;
}

string ReadLogFileToString() {
	std::ifstream logFile("log.txt"); // Open the log file
	if (!logFile.is_open()) {
		std::cerr << "Could not open the logfile: "<< std::endl;
		return ""; // Return an empty string if the file cannot be opened
	}

	std::stringstream buffer; // Use a stringstream to accumulate the file contents
	buffer << logFile.rdbuf(); // Read the entire file into the buffer

	logFile.close(); // Close the file
	return buffer.str(); // Return the contents as a string
}

Uint32 GetTimeDifference(Uint32 current, Uint32 previous) {
	if (current >= previous) {
		return current - previous;
	}
	else {
		// Handle wrap-around
		return (0xFFFFFFFF - previous) + current + 1;
	}
}

void Log(const string& message) {
	
	ofstream logFile("log.txt", ios_base::app);
	logFile << message << endl;
	logFile.close();
}
void consoleout(string message) {
	cout << message;
	Log("Consoleout:" + message);
}
bool Is_within_range(const SDL_Rect& rect1, const SDL_Rect& rect2, int range) {
    int dx = rect1.x - rect2.x;
    int dy = rect1.y - rect2.y;
    double distance = sqrt(dx * dx + dy * dy);
    return distance <= range;
}

int Get_distance_between_rects(const SDL_Rect& rect1, const SDL_Rect& rect2) {
	// Calculate the difference in x and y coordinates
	int dx = rect1.x - rect2.x;
	int dy = rect1.y - rect2.y;

	// Calculate the Euclidean distance
	double distance = std::sqrt(dx * dx + dy * dy);

	// Return the distance as an integer (rounded)
	return static_cast<int>(distance);
}

void play_exit_animation(SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	for (size_t i = 0; i < 1000; i++)
	{
		
		SDL_RenderDrawLine(renderer, 0,i,1000,i);
		SDL_Delay(1);
		SDL_RenderPresent(renderer);
		
	}
	TTF_Init();
	TTF_Font* Font = TTF_OpenFont("Impact.TTF", 300);
	TTF_Font* Font2 = TTF_OpenFont("Impact2.TTF", 40);
	SDL_Color color = { 255, 0, 0 };
	SDL_Surface* text_surface = TTF_RenderText_Solid(Font, "EXIT!", color);
	SDL_Surface* text_surface2 = TTF_RenderText_Solid(Font2, "Thank you for playing!", color);
	SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
	SDL_Texture* text_texture2 = SDL_CreateTextureFromSurface(renderer, text_surface2);
	SDL_Rect text_rect;
	SDL_Rect text_rect2;
	text_rect.x = 200;
	text_rect.y = 300;
	text_rect.w = text_surface->w;
	text_rect.h = text_surface->h;
	text_rect2.x = 300;
	text_rect2.y = 600;
	text_rect2.w = text_surface2->w;
	text_rect2.h = text_surface2->h;
	
	for (size_t i = 0; i < 3; i++)
	{
		SDL_RenderCopy(renderer, text_texture, NULL, &text_rect);
		SDL_RenderCopy(renderer, text_texture2, NULL, &text_rect2);
		SDL_RenderPresent(renderer);
		SDL_Delay(600);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, text_texture2, NULL, &text_rect2);
		SDL_RenderPresent(renderer);
		SDL_Delay(600);
	}
	SDL_RenderCopy(renderer, text_texture, NULL, &text_rect);
	SDL_RenderPresent(renderer);
	SDL_Delay(1000);
	
}
