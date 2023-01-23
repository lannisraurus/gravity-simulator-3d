//pre existing libraries
#include <iostream>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <time.h>
//my libraries
#include "windowHandler.h"



#ifdef _WIN32
#include <Windows.h>
#endif





//MAIN FUNCTION
int main(int argc, char** args) {

	//HIDE CONSOLE ON WINDOWS
#ifdef _WIN32
	//HWND windowHandle = GetConsoleWindow();
	//ShowWindow(windowHandle, SW_HIDE);
#endif

	//Initialize SDL
	SDL_SetMainReady();
	SDL_Init(SDL_INIT_EVERYTHING);

	

	//Create Initial Objects
	fileHandler file;
	windowHandler win(file);
	

	//Window Loop
	win.gameLoop();
	


	//Close the Program
	SDL_Quit();
	return 0;

}