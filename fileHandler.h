#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <filesystem>
#include "SDL2/SDL_image.h"
#include "graphics.h"
#include "vector.h"



class fileHandler {
private:
	//handles writes and reads of the files
	std::ifstream fileIn;
	std::ofstream fileOut;
	//holds the worthy save files (considered for having worthy information)
	std::vector<std::string> saves;
	//suffixes for ease of writting
	const std::string savesPath = "saves";
	const std::string worldDatPath = "world.dat";
	const std::string eraseFlag = "ERASE";
	const std::string regionPath = "/region";

	// TEXTURE PATHS
	const std::vector<const char*> objectTexturePaths = {
		"textures/objects/orb.png"
	};
	const char* textTexturePath = "textures/text/text.png";
	const char* crosshairTexturePath = "textures/text/crosshair.png";

	

	//TEXTURE STORAGE
	std::vector<SDL_Texture*> objectTextures;
	SDL_Texture* textTexture;
	SDL_Texture* crosshairTexture;
	

public:
	//constructor: loads the saves
	fileHandler() {
		loadSaves();
	}
	//loads all the save files
	void loadSaves() {
		//clears the vector
		saves.clear();

		//reads all the files in the save directory
		for (const auto& entry : std::filesystem::directory_iterator(savesPath)) {
			saves.push_back(entry.path().string());
		}

		//analyses folders
		std::vector<int> del;
		std::string location;
		for (unsigned int i = 0; i < saves.size(); i++) {
			location = saves[i] + "/" + worldDatPath;
			fileIn.open(location);
			if (!fileIn) {
				//flags indexes for erasure
				del.push_back(i);
			}
			fileIn.close();
		}


		//flag unworthy folders
		for (unsigned int i = 0; i < del.size(); i++) {
			saves[del[i]] = eraseFlag;
		}
		//delete the flagged folders
		for (unsigned int i = 0; i < saves.size(); i++) {
			if (saves[i] == eraseFlag) {
				saves.erase(saves.begin() + i);
				i--;
			}
		}

		//print worthy saves
		std::cout << "SAVES: \n";
		for (unsigned int i = 0; i < saves.size(); i++) {
			std::cout << saves[i] << std::endl;
		}
	}
	//loads textures
	void loadTextures(SDL_Renderer* rend) {
		for (unsigned int i = 0; i < objectTexturePaths.size(); i++) {
			SDL_Surface* temp = IMG_Load(objectTexturePaths[i]);
			SDL_Texture* create = SDL_CreateTextureFromSurface(rend, temp);
			objectTextures.push_back(create);
		}
		SDL_Surface* temp = IMG_Load(textTexturePath);
		textTexture = SDL_CreateTextureFromSurface(rend, temp);

		temp = IMG_Load(crosshairTexturePath);
		crosshairTexture = SDL_CreateTextureFromSurface(rend, temp);


	}
	//loads settings
	void loadSettings(int &WID, int &HEI, double &mouseSensitivityX, double& mouseSensitivityY, double& step, double &camDist, double &timeMultiplier) {
		fileIn.open("settings/settings.dat");
		std::string setting;
		while (!fileIn.eof()) {
			fileIn >> setting;
			if (setting == "dimensions") {
				std::cout << "DIMENSIONS INITIALIZED\n";
				fileIn >> WID;
				fileIn >> HEI;
			}
			if (setting == "x_sensitivity") {
				std::cout << "X SENSITIVITY INITIALIZED\n";
				fileIn >> mouseSensitivityX;
			}
			if (setting == "y_sensitivity") {
				std::cout << "Y SENSITIVITY INITIALIZED\n";
				fileIn >> mouseSensitivityY;
			}
			if (setting == "step") {
				std::cout << "STEP INITIALIZED\n";
				fileIn >> step;
			}
			if (setting == "camera_distance") {
				std::cout << "CAM DIST INITIALIZED\n";
				fileIn >> camDist;
			}
			if (setting == "time_multiplier") {
				std::cout << "TIME MULTIPLIER INITIALIZED\n";
				fileIn >> timeMultiplier;
			}
		}
		fileIn.close();
	}




	SDL_Texture* getTextTexture() { return textTexture; }
	SDL_Texture* getObjectTexture(unsigned int n) {
		if (n < objectTextures.size()) {
			return objectTextures[n];
		}
		else {
			return objectTextures.back();
		}	
	}
	SDL_Texture* getCrosshairTexture() {
		return crosshairTexture;
	}



	//adds int index to a string
	std::string addIndex(std::string str, int num) {
		return str + std::to_string(num);
	}



	//returns the save file names
	void getSaveNames(std::vector<std::string> &copyPath) {
		copyPath.clear();
		std::string name;
		for (unsigned int i = 0; i < saves.size(); i++) {
			name = saves[i].substr(6, saves[i].size());
			copyPath.push_back(name);
		}

	}

	//delete a save file
	void deleteSave(unsigned int num) {
		if (num >= 0 && num < saves.size()) {
			std::string path = saves[num];
			std::cout << "DELETING ---- " << path << std::endl;
			std::filesystem::remove_all(path);
			loadSaves();
		}
		
	}


	//load world information onto the main window handler
	void loadSave(int num, vector& camCenter, double& xAngle, double& yAngle, std::vector<planet>& universe) {
		universe.clear();
		fileIn.open(saves[num] + "/" + worldDatPath);
		std::string identifier;
		double x=0, y=0, z=0;
		vector position(0,0,0);
		vector velocity(0,0,0);
		double radius=10;
		double mass=10;
		unsigned int textureId=0;
		int r = 255, g = 255, b = 255;
		camCenter = vector(0, 0, 0);
		xAngle = 0;
		yAngle = 0;
		int cycles = 0;
		while (!fileIn.eof()) {
			
			fileIn >> identifier;

			if (identifier == "camera_center") {
				fileIn >> x >> y >> z;
				std::cout << "CAMERA CENTER LOADED\n";
				camCenter = vector(x, y, z);
			}
			if (identifier == "camera_angles") {
				std::cout << "CAMERA ANGLES LOADED\n";
				fileIn >> xAngle >> yAngle;
			}
			if (identifier == "planet_position") {
				fileIn >> x >> y >> z;
				std::cout << "SET PLANET POS\n";
				position = vector(x, y, z);
			}
			if (identifier == "planet_velocity") {
				fileIn >> x >> y >> z;
				std::cout << "SET PLANET VELOCITY\n";
				velocity = vector(x, y, z);
			}
			if (identifier == "planet_radius") {
				std::cout << "SET PLANET RADIUS\n";
				fileIn >> radius;
			}
			if (identifier == "planet_mass") {
				std::cout << "SET PLANET MASS\n";
				fileIn >> mass;
			}
			if (identifier == "planet_texture") {
				std::cout << "SET PLAMET TEXTURE\n";
				fileIn >> textureId;
			}
			if (identifier == "planet_colour") {
				std::cout << "SET PLAMET colour\n";
				fileIn >> r;
				fileIn >> g;
				fileIn >> b;
			}
			if (identifier == "create_planet") {
				planet p(position,velocity,radius,mass,objectTextures[textureId],textureId,r,g,b);
				std::cout << "PLANET CREATED\n";
				universe.push_back(p);
			}
				
		
			cycles++;
			if (cycles > 999999999) { break; }
		}
		fileIn.close();
	}
	


	void saveSave(int saveIndex, vector camCenter, double xAngle, double yAngle, std::vector<planet> universe) {
		fileOut.open(saves[saveIndex]+"/"+worldDatPath, std::ofstream::trunc);
		fileOut << "camera_center " << camCenter.getX() << " " << camCenter.getY() << " " << camCenter.getZ() << "\n";
		fileOut << "camera_angles " << xAngle << " " << yAngle << "\n";
		fileOut << "\n";
		for (unsigned int i = 0; i < universe.size(); i++) {
			fileOut << "planet_colour " << (int)universe[i].getR() << " " << (int)universe[i].getG() << " " << (int)universe[i].getB() << "\n";
			fileOut << "planet_position " << universe[i].getPosition().getX() << " " << universe[i].getPosition().getY() << " " << universe[i].getPosition().getZ() << "\n";
			fileOut << "planet_velocity " << universe[i].getVelocity().getX() << " " << universe[i].getVelocity().getY() << " " << universe[i].getVelocity().getZ() << "\n";
			fileOut << "planet_mass " << universe[i].getMass() << "\n";
			fileOut << "planet_radius " << universe[i].getRadius() << "\n";
			fileOut << "planet_texture " << universe[i].getTextureId() << "\n";
			fileOut << "create_planet\n";
			fileOut << "\n";
		}
		fileOut << "\n\nend_file5";
		fileOut.close();
	}





	//closes all files, destructor
	~fileHandler() {
		fileIn.close();
		fileOut.close();
	}



};