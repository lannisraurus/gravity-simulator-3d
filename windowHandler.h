#pragma once

#include <iostream>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <string.h>
#include <algorithm>

#include "fileHandler.h"
#include "camera.h"
#include "graphics.h"
#include "vector.h"


#define toDeg 180 / M_PI


class windowHandler {
private:
	//SETTINGS
	int WID = 100;
	int HEI = 100;
	double mouseSensitivityX = 0.01;
	double mouseSensitivityY = 0.001;
	double step = 0.01;
	double camDist = 1;
	double timeMultiplier = 1;

	//rainbow loop
	Uint8 rainbowR = 255;
	Uint8 rainbowG = 0;
	Uint8 rainbowB = 0;
	double rainbowTimer = 0;



	//save file type of beat
	std::vector<std::string> saveNames;
	unsigned int saveIndex = 0;
	int saveSelected = -1;



	//Cursor
	int xMouse;
	int yMouse;
	bool leftClick = false;
	bool holdingLeftClick = false;
	double dxMouse;
	double dyMouse;


	//SDL Type of beat
	SDL_Window* win = NULL;
	SDL_Event ev;
	SDL_Renderer* rend = NULL;

	//bools on states
	bool isOpen = true;
	bool textInput = false;
	bool inGame = false;
	bool controllingCamera = false;
	bool keyDown = false;
	

	//handles the menus
	int menu = 0;


	//dT is the delta time, ticks per second. t0 is used in the calculation to determine dT
	double dT = 0, t0 = 0;
	double fps = 0;
	double fpsDelay = 1;

	//file handling variables, object stack
	fileHandler* file;

	//CAMERA INFORMATION
	double xAngle = 0;
	double yAngle = 0;
	vector camCenter;
	vector camDirection;
	camera cam;
	SDL_Texture* crosshairTexture;

	//UNIVERSE
	std::vector<planet> universe;
	int selectedPlanet = -1;



public:
	//create window object (constructor)
	windowHandler(fileHandler& f) :
		file(&f)
	{
		file->loadSettings(WID, HEI, mouseSensitivityX, mouseSensitivityY, step, camDist, timeMultiplier);
		win = SDL_CreateWindow("Raurus Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WID, HEI, SDL_WINDOW_SHOWN);
		rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
		eventHandler();
		mouseHandler();
		file->loadTextures(rend);
		crosshairTexture = file->getCrosshairTexture();
	}








	//main game loop
	void gameLoop() {

		while (isOpen == true) {


			//clears the screen
			clearScreen();



			//game handler - sortObjects, renderObjects and manage game 
			gameHandler();


			//menu handler (not in game) - render objects and UI
			menuHandler();


			//technical handlers
			rainbowLoop();
			timeHandler();
			bindCamera();
			movementHandler();
			mouseHandler();
			eventHandler();
			
			

			
			


			//updates the screen
			updateScreen();


		}
	}




















	//GRAPHICS

	void updateScreen() { SDL_RenderPresent(rend); }

	void clearScreen() { SDL_SetRenderDrawColor(rend, 0, 0, 0, 0); SDL_RenderClear(rend); }





	//HANDLERS
	void movementHandler() {
		const Uint8* keyState = SDL_GetKeyboardState(NULL);
		vector walk(0, 0, 0);
		if (keyState[SDL_SCANCODE_W]) {
			walk = vector(cam.getDirection().getX(), cam.getDirection().getY(), 0);
			walk.setSize(step);
			camCenter = camCenter + walk;
		}
		if (keyState[SDL_SCANCODE_S]) {
			walk = vector(cam.getDirection().getX(), cam.getDirection().getY(), 0);
			walk.setSize(-step);
			camCenter = camCenter + walk;
		}
		if (keyState[SDL_SCANCODE_D]) {
			walk = cam.getXComponent();
			walk.multiplyConstant(step);
			camCenter = camCenter + walk;
		}
		if (keyState[SDL_SCANCODE_A]) {
			walk = cam.getXComponent();
			walk.multiplyConstant(-step);
			camCenter = camCenter + walk;
		}
		if (keyState[SDL_SCANCODE_SPACE]) {
			walk = vector(0, 0, 1);
			walk.multiplyConstant(step);
			camCenter = camCenter + walk;
		}
		if (keyState[SDL_SCANCODE_LSHIFT]) {
			walk = vector(0, 0, -1);
			walk.multiplyConstant(step);
			camCenter = camCenter + walk;
		}
	}

	void eventHandler() {
		while (SDL_PollEvent(&ev)) {
			switch (ev.type) {
			case SDL_QUIT:
				isOpen = false;
				break;
			case SDL_KEYDOWN:
				keyDown = true;
				switch (ev.key.keysym.sym) {
				case SDLK_ESCAPE:
					if (inGame) {
						if (menu == 3) {
							menu = -1;
							controllingCamera = true;
						}
						else {
							menu = 3;
							controllingCamera = false;
						}
					}
					break;
				case SDLK_UP:
					if (controllingCamera && timeMultiplier < 1000) {
						if (timeMultiplier >= -1 && timeMultiplier < 0) {
							timeMultiplier = 0;
						}else
						if (timeMultiplier == 0) {
							timeMultiplier = 1;
						}else
						if (timeMultiplier >= 1) {
							timeMultiplier *= 2;
						}
						else if (timeMultiplier < -1) {
							timeMultiplier /= 2;
						}
					}
					break;
				case SDLK_DOWN:
					if (controllingCamera) {
						if (timeMultiplier <= 1 && timeMultiplier > 0) {
							timeMultiplier = 0;
						}
						else
							if (timeMultiplier == 0) {
								timeMultiplier = -1;
							}
							else
								if (timeMultiplier <= -1) {
									timeMultiplier *= 2;
								}
								else if (timeMultiplier > 1) {
									timeMultiplier /= 2;
								}
						
					}
					break;
				case SDLK_LEFT:
					if (controllingCamera) {
						step /= 2;
					}
					break;
				case SDLK_RIGHT:
					if (controllingCamera) {
						step *= 2;
					}
					break;
				}break;


			case SDL_KEYUP:
				keyDown = false;
				break;



			case SDL_MOUSEBUTTONDOWN:
				switch (ev.button.button) {
				case SDL_BUTTON_LEFT:
					leftClick = true;
					holdingLeftClick = true;
					break;
				default:
					break;
				}break;

			case SDL_MOUSEBUTTONUP:
				switch (ev.button.button) {
				case SDL_BUTTON_LEFT:
					holdingLeftClick = false;
					break;
				default: break;
				}break;

			case SDL_MOUSEMOTION:
				dxMouse = ev.motion.xrel;
				dyMouse = -ev.motion.yrel;
				if (controllingCamera) {
					xAngle += dxMouse * mouseSensitivityX;
					yAngle += dyMouse * mouseSensitivityY;
				}
				break;
			default: break;
			}
		}
	}

	void timeHandler() {
		//time handler
		dT = ((double)SDL_GetTicks64() - t0) / 1000;
		t0 = (double)SDL_GetTicks64();
		fpsDelay += dT;

		if (fpsDelay > 0.5) {
			fps = 1 / dT;
			fpsDelay = 0;
		}


		text fpsCounter(WID, HEI, -0.98, 0.96, 0.02, file->getTextTexture(), "fps " + std::to_string((int)fps), false);
		fpsCounter.render(rend, 255, 255, 255);

	}

	void mouseHandler() {
		//mouse position stuff
		SDL_GetWindowSize(win, &WID, &HEI);
		SDL_GetMouseState(&xMouse, &yMouse);
		leftClick = false;

		if (controllingCamera) {
			SDL_SetRelativeMouseMode(SDL_TRUE);
		}
		else {
			SDL_SetRelativeMouseMode(SDL_FALSE);
		}

	}

	void universeHandler() {
		//DO PLANET PHYSICS
		for (unsigned int i = 0; i < universe.size(); i++) {
			universe[i].physics(universe, timeMultiplier * dT);
		}


		//CALCULATE PLANETS DISTANCE
		for (unsigned int i = 0; i < universe.size(); i++) {
			universe[i].setCameraDist(cam);
		}

		//SORT PLANETS
		std::sort(universe.begin(), universe.end(), std::greater<planet>());


		//RENDER PLANETS
		for (unsigned int i = 0; i < universe.size(); i++) {
			universe[i].render(rend, cam);
		}


		//RENDER SELECTED PLANET
		selectedPlanet = -1;
		for (unsigned int i = 0; i < universe.size(); i++) {
			if (universe[i].isSelected()) {
				selectedPlanet = i;
			}
		}
		if (selectedPlanet >= 0 && selectedPlanet < universe.size()) {
			universe[selectedPlanet].renderWithColour(rend, cam, rainbowR, rainbowG, rainbowB);
		}
		

		
	}

	void gameHandler() {
		if (inGame) {
			//graphic text in game UI
			text cameraPosText(WID, HEI, -0.98, 0.90, 0.02, file->getTextTexture(), "Camera Position " + camCenter.getString(1), false);
			cameraPosText.render(rend, 255, 255, 255);
			text cameraAngleText(WID, HEI, -0.98, 0.84, 0.02, file->getTextTexture(), "Camera Angles (" + std::to_string((int)(xAngle * toDeg)) + " , " + std::to_string((int)(yAngle * toDeg)) + ")", false);
			cameraAngleText.render(rend, 255, 255, 255);

			std::ostringstream stringStream;

			stringStream << std::fixed << std::setprecision(4) << step;
			std::string stepString = stringStream.str();
			text stepText(WID, HEI, 0.7, -0.9, 0.02, file->getTextTexture(), "Step " + stepString, false);
			stepText.render(rend, 255, 255, 255);

			stringStream.str("");

			stringStream << std::fixed << std::setprecision(4) << timeMultiplier;
			std::string timeString = stringStream.str();
			text timeText(WID, HEI, 0.7, -0.84, 0.02, file->getTextTexture(), "Time X" + timeString, false);
			timeText.render(rend, 255, 255, 255);

			


			if (controllingCamera) {
				//CREATE CAMERA OBJECT ACCORDING TO THE LOOKING ANGLES
				double x, y, z;
				x = cos(xAngle) * cos(yAngle);
				y = sin(xAngle) * cos(yAngle);
				z = sin(yAngle);
				camDirection = vector(x, y, z);
				cam = camera(camDirection, camCenter, WID, HEI, camDist);
			}


			universeHandler();




			SDL_Rect crosshair;
			crosshair.w = 0.05 * WID;
			crosshair.h = 0.05 * HEI;
			crosshair.x = WID/2 - crosshair.w/2;
			crosshair.y = HEI/2 - crosshair.h/2;
			
			SDL_SetTextureColorMod(crosshairTexture, 255, 255, 255);
			SDL_RenderCopy(rend, crosshairTexture, NULL, &crosshair);

			if (menu == 3) {

				menuInGame();

			}
		}
	}

	void bindCamera() {
		if (xAngle < -M_PI) { xAngle = M_PI; }
		if (xAngle > M_PI) { xAngle = -M_PI; }
		if (yAngle > M_PI/2) { yAngle = M_PI/2; }
		if (yAngle < -M_PI/2) { yAngle = -M_PI/2; }
	}

	void rainbowLoop() {
		rainbowTimer += dT;
		if (rainbowTimer > 0.01) {
			if (rainbowR > 0 && rainbowG < 255 && rainbowB == 0) {
				rainbowR -= 1;
				rainbowG += 1;
			}
			if (rainbowR == 0 && rainbowG > 0 && rainbowB < 255) {
				rainbowG -= 1;
				rainbowB += 1;
			}
			if (rainbowR < 255 && rainbowG == 0 && rainbowB > 0) {
				rainbowR += 1;
				rainbowB -= 1;
			}
			rainbowTimer = 0;
		}
	}









	//MENUS
	void menuHandler() {
		switch (menu) {
		case 0:
			mainMenu();
			break;
		case 1:
			savesMenu();
			break;
		case 2:
			settingsMenu();
			break;
		case 3:
			menuInGame();
			break;
		default: break;
		}
	}

	void menuInGame() {
		button returnToTitle(WID, HEI, 0, -0.3, 0.35, 0.06);
		returnToTitle.render(rend, rainbowR, rainbowG, rainbowB, xMouse, yMouse);
		text returnToTitleText(WID, HEI, 0, -0.3, 0.035, file->getTextTexture(), "Quit", true);
		returnToTitleText.render(rend, 0, 0, 0);

		button returnToTitleAndSave(WID, HEI, 0, 0, 0.35, 0.06);
		returnToTitleAndSave.render(rend, rainbowR, rainbowG, rainbowB, xMouse, yMouse);
		text returnToTitleTextSave(WID, HEI, 0, 0, 0.035, file->getTextTexture(), "Save and Quit", true);
		returnToTitleTextSave.render(rend, 0, 0, 0);

		button returnToGame(WID, HEI, 0, 0.3, 0.35, 0.06);
		returnToGame.render(rend, rainbowR, rainbowG, rainbowB, xMouse, yMouse);
		text returnToGameText(WID, HEI, 0, 0.3, 0.035, file->getTextTexture(), "Return", true);
		returnToGameText.render(rend, 0, 0, 0);

		//close the world 
		if (returnToTitle.isInside(xMouse, yMouse) && leftClick) {
			universe.clear();
			controllingCamera = false;
			inGame = false;
			file->loadSaves();
			menu = 1;
			leftClick = false;
		}
		if (returnToTitleAndSave.isInside(xMouse, yMouse) && leftClick) {
			file->saveSave(saveSelected, camCenter, xAngle, yAngle, universe);
			universe.clear();
			controllingCamera = false;
			inGame = false;
			file->loadSaves();
			menu = 1;
			leftClick = false;
		}
		if (returnToGame.isInside(xMouse, yMouse) && leftClick) {
			menu = -1;
			controllingCamera = true;
			leftClick = false;
		}

		

	}

	void mainMenu() {
		button play(WID, HEI, 0, -0.1, 0.35, 0.06);
		play.render(rend, rainbowR, rainbowG, rainbowB, xMouse, yMouse);
		button sett(WID, HEI, 0, -0.3, 0.35, 0.06);
		sett.render(rend, rainbowR, rainbowG, rainbowB, xMouse, yMouse);
		button quit(WID, HEI, 0, -0.5, 0.35, 0.06);
		quit.render(rend, rainbowR, rainbowG, rainbowB, xMouse, yMouse);


		if (play.isInside(xMouse, yMouse) && leftClick) { menu = 1; file->loadSaves(); saveIndex = 0; }
		if (sett.isInside(xMouse, yMouse) && leftClick) { menu = 2; }
		if (quit.isInside(xMouse, yMouse) && leftClick) { isOpen = false; }


		text playText(WID, HEI, 0, -0.1, 0.035, file->getTextTexture(), "Start", true);
		playText.render(rend, 0, 0, 0);
		text settText(WID, HEI, 0, -0.3, 0.035, file->getTextTexture(), "Settings", true);
		settText.render(rend, 0, 0, 0);
		text quitText(WID, HEI, 0, -0.5, 0.035, file->getTextTexture(), "Quit", true);
		quitText.render(rend, 0, 0, 0);


		text title(WID, HEI, 0, 0.5, 0.1, file->getTextTexture(), "Raurus.Engine", true);
		title.render(rend, rainbowR, rainbowG, rainbowB);

	}

	void settingsMenu() {
		button title(WID, HEI, -0.62, -0.9, 0.35, 0.06);
		title.render(rend, rainbowR, rainbowG, rainbowB, xMouse, yMouse);
		if (title.isInside(xMouse, yMouse) && leftClick) { menu = 0; }
		text titleText(WID, HEI, -0.62, -0.9, 0.035, file->getTextTexture(), "Return", true);
		titleText.render(rend, 0, 0, 0);


		button box1(WID, HEI, -0.62, 0.8, 0.35, 0.06);
		box1.render(rend, 255, 255, 255, xMouse, yMouse);
		button box2(WID, HEI, -0.62, 0.6, 0.35, 0.06);
		box2.render(rend, 255, 255, 255, xMouse, yMouse);
		button box3(WID, HEI, -0.62, 0.4, 0.35, 0.06);
		box3.render(rend, 255, 255, 255, xMouse, yMouse);
		button box4(WID, HEI, -0.62, 0.2, 0.35, 0.06);
		box4.render(rend, 255, 255, 255, xMouse, yMouse);
		button box5(WID, HEI, -0.62, 0, 0.35, 0.06);
		box5.render(rend, 255, 255, 255, xMouse, yMouse);
		button box6(WID, HEI, -0.62, -0.2, 0.35, 0.06);
		box6.render(rend, 255, 255, 255, xMouse, yMouse);


		text text1(WID, HEI, -0.62, 0.8, 0.035, file->getTextTexture(), "Resolution", true);
		text1.render(rend, 0, 0, 0);
		text text2(WID, HEI, -0.62, 0.6, 0.035, file->getTextTexture(), "X Mouse Sensitivity", true);
		text2.render(rend, 0, 0, 0);
		text text3(WID, HEI, -0.62, 0.4, 0.035, file->getTextTexture(), "Y Mouse Sensitivity", true);
		text3.render(rend, 0, 0, 0);
		text text4(WID, HEI, -0.62, 0.2, 0.035, file->getTextTexture(), "Time Multiplier", true);
		text4.render(rend, 0, 0, 0);
		text text5(WID, HEI, -0.62, 0, 0.035, file->getTextTexture(), "Step", true);
		text5.render(rend, 0, 0, 0);
		text text6(WID, HEI, -0.62, -0.2, 0.035, file->getTextTexture(), "Camera Distance", true);
		text6.render(rend, 0, 0, 0);



	}

	void savesMenu() {


		button title(WID, HEI, -0.62, -0.9, 0.3, 0.06);
		title.render(rend, rainbowR, rainbowG, rainbowB, xMouse, yMouse);
		button newWorld(WID, HEI, 0, -0.9, 0.3, 0.06);
		newWorld.render(rend, rainbowR, rainbowG, rainbowB, xMouse, yMouse);
		button loadWorld(WID, HEI, 0.62, -0.9, 0.3, 0.06);
		loadWorld.render(rend, rainbowR, rainbowG, rainbowB, xMouse, yMouse);
		button deletebutton(WID, HEI, 0.62, 0.9, 0.3, 0.06);
		deletebutton.render(rend, rainbowR, rainbowG, rainbowB, xMouse, yMouse);


		text titleText(WID, HEI, -0.62, -0.9, 0.035, file->getTextTexture(), "Return", true);
		titleText.render(rend, 0, 0, 0);
		text newWorldText(WID, HEI, 0, -0.9, 0.035, file->getTextTexture(), "New Universe", true);
		newWorldText.render(rend, 0, 0, 0);
		text loadWorldText(WID, HEI, 0.62, -0.9, 0.035, file->getTextTexture(), "Load Universe", true);
		loadWorldText.render(rend, 0, 0, 0);
		text deltext(WID, HEI, 0.62, 0.9, 0.035, file->getTextTexture(), "DELETE", true);
		deltext.render(rend, 0, 0, 0);


		if (title.isInside(xMouse, yMouse) && leftClick) { menu = 0; saveSelected = -1; }
		if (deletebutton.isInside(xMouse, yMouse) && leftClick) { file->deleteSave(saveSelected); saveSelected = -1; }
		//LOAD THE WORLD!!!!!!
		if (loadWorld.isInside(xMouse, yMouse) && leftClick && saveSelected >= 0) {
			file->loadSettings(WID, HEI, mouseSensitivityX, mouseSensitivityY, step, camDist, timeMultiplier);
			file->loadSave(saveSelected,camCenter,xAngle,yAngle,universe);
			menu = -1;
			inGame = true;
			controllingCamera = true;
		}





		button goUp(WID, HEI, -0.9, 0.6, 0.03, 0.05);
		button goDown(WID, HEI, -0.9, -0.6, 0.03, 0.05);
		goUp.render(rend, rainbowR, rainbowG, rainbowB, xMouse, yMouse);
		goDown.render(rend, rainbowR, rainbowG, rainbowB, xMouse, yMouse);

		if (goUp.isInside(xMouse, yMouse) && leftClick && saveIndex > 0) {
			saveIndex -= 1;
		}
		if (goDown.isInside(xMouse, yMouse) && leftClick && saveIndex < saveNames.size()) {
			saveIndex += 1;
		}


		file->getSaveNames(saveNames);
		for (unsigned int i = saveIndex; i < saveNames.size(); i++) {
			double x = -0.75;
			double y = 0.6;
			y -= (i - saveIndex) * 0.15;
			if (y < -0.7) { break; }
			button saveBackground(WID, HEI, 0, y, 0.8, 0.05);
			if (i == saveSelected) { saveBackground.setDefaultColour(rainbowR, rainbowG, rainbowB); }
			saveBackground.render(rend, rainbowR, rainbowG, rainbowB, xMouse, yMouse);
			if (saveBackground.isInside(xMouse, yMouse) && leftClick) { saveSelected = i; std::cout << "selected " << i << std::endl; }
			text saveList(WID, HEI, x, y, 0.03, file->getTextTexture(), saveNames[i], false);
			saveList.render(rend, 0, 0, 0);
		}



	}
	




























};