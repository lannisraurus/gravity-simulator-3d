#pragma once

#include <iostream>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <string>
#include <vector>

#include "vector.h"
#include "camera.h"


class planet {

private:
	//planet vectors
	vector position, velocity;

	//planet texture
	SDL_Texture* texture;
	int textureId;

	//planet doubles
	double radius;
	double mass;

	//constants of nature
	const double G = 0.000000000066742;

	//distance to camera
	double cameraDist;

	//planet colour
	Uint8 r, g, b;

	//is planet selected
	bool selected = false;

public:

	vector getPosition() { return position; }
	vector getVelocity() { return velocity; }
	int getTextureId() { return textureId; }
	double getRadius() { return radius; }
	double getMass() { return mass; }
	Uint8 getR() { return r; }
	Uint8 getG() { return g; }
	Uint8 getB() { return b; }


	planet(vector position, vector velocity, double radius, double mass, SDL_Texture* texture, int textureId, Uint8 r, Uint8 g, Uint8 b) {
		this->position = position;
		this->texture = texture;
		this->velocity = velocity;
		this->radius = radius;
		this->mass = mass;
		this->r = r;
		this->g = g;
		this->b = b;
		this->textureId = textureId;
		cameraDist = 0;
	}

	void renderWithColour(SDL_Renderer* rend, camera cam, Uint8 R, Uint8 G, Uint8 B) {
		SDL_SetTextureColorMod(texture, 255, 255, 255);
		SDL_SetTextureColorMod(texture, R, G, B);
		vector Comp = cam.getXComponent();
		Comp.multiplyConstant(radius);
		vector boundaryPoint = position + Comp;
		xy center = cam.renderPoint(position);
		xy boundary = cam.renderPoint(boundaryPoint);

		if (boundary.x < center.x) {
			boundary.x = center.x;
		}


		if (center.visible && boundary.visible) {
			xy sizexy(center, boundary);
			double size = 2 * sizexy.size();
			if (size < 1) {
				size = 1;
			}
			SDL_Rect box;
			box.w = size;
			box.h = size;
			box.x = cam.getWID() / 2 + center.x - (size / 2);
			box.y = cam.getHEI() / 2 + center.y - (size / 2);

			SDL_RenderCopy(rend, texture, NULL, &box);
		}
	}


	void render(SDL_Renderer* rend, camera cam) {

		

		SDL_SetTextureColorMod(texture, 255, 255, 255);
		SDL_SetTextureColorMod(texture, r, g, b);
		vector Comp = cam.getXComponent();
		Comp.multiplyConstant(radius);
		vector boundaryPoint = position + Comp;
		xy center = cam.renderPoint(position);
		xy boundary = cam.renderPoint(boundaryPoint);

		if (boundary.x < center.x) {
			boundary.x = center.x;
		}


		if (center.visible && boundary.visible) {
			xy sizexy(center, boundary);
			double size = 2 * sizexy.size();
			if (size < 1) {
				size = 1;
			}
			SDL_Rect box;
			box.w = size;
			box.h = size;
			box.x = cam.getWID() / 2 + center.x - (size / 2);
			box.y = cam.getHEI() / 2 + center.y - (size / 2);

			if (box.x < cam.getWID()/2 && box.x + box.w > cam.getWID() / 2 && box.y < cam.getHEI() / 2 && box.y + box.h >  cam.getHEI() / 2) {
				selected = true;
			}
			else {
				selected = false;
			}

			SDL_RenderCopy(rend, texture, NULL, &box);
		}
	}


	bool isSelected() { return selected; }


	void physics(std::vector<planet> universe, double dT) {

		//set total to set
		vector Ftotal(0,0,0);
		//calculate the total force acting on the object
		for (unsigned int i = 0; i < universe.size(); i++) {
			vector dist(position, universe[i].position);
			double d = dist.getVectorSize();
			if (d == 0) {} else {
				double Fmod = G * universe[i].mass;
				Fmod /= d * d;
				vector force = dist;
				force.setSize(Fmod);
				Ftotal = Ftotal + force;
			}
		}
		//calculate the acceleration caused by the total of the forces
		vector acceleration = Ftotal;
		vector dV = acceleration;
		dV.multiplyConstant(dT);
		velocity = velocity + dV;
		vector dPos = velocity;
		dPos.multiplyConstant(dT);
		position = position + dPos;
	}


	void setCameraDist(camera cam) {
		vector distance(cam.getCenter(), position);
		cameraDist = distance.getVectorSize();
	}


	bool operator > (const planet obj) const {
		return cameraDist > obj.cameraDist;
	}
	void operator = (planet obj) {
		position = obj.position;
		velocity = obj.velocity;
		radius = obj.radius;
		texture = obj.texture;
		cameraDist = obj.cameraDist;
		mass = obj.mass;
		r = obj.r;
		g = obj.g;
		b = obj.b;
	}
};



class button {
private:
	int* WID, *HEI;
	double x, y;
	double w, h;
	int actualX, actualY, actualW, actualH;
	Uint8 oR = 255, oG = 255, oB = 255;

public:
	button(int &WID, int& HEI, double x, double y, double w, double h) {
		this->WID = &WID;
		this->HEI = &HEI;
		this->x = x;
		this->y = y;
		this->w = w;
		this->h = h;
		setActualCoords();
	}
	void setActualCoords() {
		actualX = (int)(*WID / 2 + x * *WID / 2 - *WID * w / 2);
		actualY = (int)(*HEI / 2 - y * *HEI / 2 - *HEI * h / 2);
		actualW = (int)(*WID * w);
		actualH = (int)(*HEI * h);
	}
	void render(SDL_Renderer* rend, Uint8 R, Uint8 G, Uint8 B, int xmouse, int ymouse) {
		SDL_Rect rect = {actualX, actualY, actualW, actualH};

		if (isInside(xmouse,ymouse)) { 
			SDL_SetRenderDrawColor(rend, R, G, B, 255);
		}else {
			SDL_SetRenderDrawColor(rend, oR, oG, oB, 255);
		}
		SDL_RenderFillRect(rend, &rect);
		SDL_RenderDrawRect(rend, &rect);
	}

	bool isInside(int x, int y) {
		if (x>actualX&&x<actualX+actualW&&y>actualY&&y<actualY+actualH) {
			return true;
		}
		else {
			return false;
		}
	}


	void setDefaultColour(Uint8 R, Uint8 G, Uint8 B) {
		oR = R;
		oG = G;
		oB = B;
	}


	
};



class character {
private:
	int* WID, * HEI;
	double x, y;
	double size;
	SDL_Texture* font;
	int textureRow;
	int textureCol;
	int actualX, actualY;
	int actualW,actualH;
	int fontW, fontH;
	SDL_Rect crop;

public:
	character(int& WID, int& HEI, double x, double y, double size, SDL_Texture* font, char character) {
		this->WID = &WID;
		this->HEI = &HEI;
		this->x = x;
		this->y = y;
		this->size = size;
		this->font = font;
		setTextureIndex(character);
		setActualCoords();
		SDL_QueryTexture(font, NULL, NULL, &fontW, &fontH);
		setCrop();
	}

	void setTextureIndex(char character) {
		switch (character) {
		case 'A':
			textureRow = 0;
			textureCol = 0;
			break;
		case 'B':
			textureRow = 0;
			textureCol = 1;
			break;
		case 'C':
			textureRow = 0;
			textureCol = 2;
			break;
		case 'D':
			textureRow = 0;
			textureCol = 3;
			break;
		case 'E':
			textureRow = 0;
			textureCol = 4;
			break;
		case 'F':
			textureRow = 0;
			textureCol = 5;
			break;
		case 'G':
			textureRow = 0;
			textureCol = 6;
			break;
		case 'H':
			textureRow = 0;
			textureCol = 7;
			break;
		case 'I':
			textureRow = 0;
			textureCol = 8;
			break;
		case 'J':
			textureRow = 0;
			textureCol = 9;
			break;
		case 'K':
			textureRow = 0;
			textureCol = 10;
			break;
		case 'L':
			textureRow = 0;
			textureCol = 11;
			break;
		case 'M':
			textureRow = 0;
			textureCol = 12;
			break;
		case 'N':
			textureRow = 0;
			textureCol = 13;
			break;
		case 'O':
			textureRow = 0;
			textureCol = 14;
			break;
		case 'P':
			textureRow = 0;
			textureCol = 15;
			break;
		case 'Q':
			textureRow = 0;
			textureCol = 16;
			break;
		case 'R':
			textureRow = 0;
			textureCol = 17;
			break;
		case 'S':
			textureRow = 0;
			textureCol = 18;
			break;
		case 'T':
			textureRow = 0;
			textureCol = 19;
			break;
		case 'U':
			textureRow = 0;
			textureCol = 20;
			break;
		case 'V':
			textureRow = 0;
			textureCol = 21;
			break;
		case 'W':
			textureRow = 0;
			textureCol = 22;
			break;
		case 'X':
			textureRow = 0;
			textureCol = 23;
			break;
		case 'Y':
			textureRow = 0;
			textureCol = 24;
			break;
		case 'Z':
			textureRow = 0;
			textureCol = 25;
			break;
		case 'a':
			textureRow = 1;
			textureCol = 0;
			break;
		case 'b':
			textureRow = 1;
			textureCol = 1;
			break;
		case 'c':
			textureRow = 1;
			textureCol = 2;
			break;
		case 'd':
			textureRow = 1;
			textureCol = 3;
			break;
		case 'e':
			textureRow = 1;
			textureCol = 4;
			break;
		case 'f':
			textureRow = 1;
			textureCol = 5;
			break;
		case 'g':
			textureRow = 1;
			textureCol = 6;
			break;
		case 'h':
			textureRow = 1;
			textureCol = 7;
			break;
		case 'i':
			textureRow = 1;
			textureCol = 8;
			break;
		case 'j':
			textureRow = 1;
			textureCol = 9;
			break;
		case 'k':
			textureRow = 1;
			textureCol = 10;
			break;
		case 'l':
			textureRow = 1;
			textureCol = 11;
			break;
		case 'm':
			textureRow = 1;
			textureCol = 12;
			break;
		case 'n':
			textureRow = 1;
			textureCol = 13;
			break;
		case 'o':
			textureRow = 1;
			textureCol = 14;
			break;
		case 'p':
			textureRow = 1;
			textureCol = 15;
			break;
		case 'q':
			textureRow = 1;
			textureCol = 16;
			break;
		case 'r':
			textureRow = 1;
			textureCol = 17;
			break;
		case 's':
			textureRow = 1;
			textureCol = 18;
			break;
		case 't':
			textureRow = 1;
			textureCol = 19;
			break;
		case 'u':
			textureRow = 1;
			textureCol = 20;
			break;
		case 'v':
			textureRow = 1;
			textureCol = 21;
			break;
		case 'w':
			textureRow = 1;
			textureCol = 22;
			break;
		case 'x':
			textureRow = 1;
			textureCol = 23;
			break;
		case 'y':
			textureRow = 1;
			textureCol = 24;
			break;
		case 'z':
			textureRow = 1;
			textureCol = 25;
			break;
		case '0':
			textureRow = 2;
			textureCol = 0;
			break;
		case '1':
			textureRow = 2;
			textureCol = 1;
			break;
		case '2':
			textureRow = 2;
			textureCol = 2;
			break;
		case '3':
			textureRow = 2;
			textureCol = 3;
			break;
		case '4':
			textureRow = 2;
			textureCol = 4;
			break;
		case '5':
			textureRow = 2;
			textureCol = 5;
			break;
		case '6':
			textureRow = 2;
			textureCol = 6;
			break;
		case '7':
			textureRow = 2;
			textureCol = 7;
			break;
		case '8':
			textureRow = 2;
			textureCol = 8;
			break;
		case '9':
			textureRow = 2;
			textureCol = 9;
			break;
		case '.':
			textureRow = 2;
			textureCol = 10;
			break;
		case ',':
			textureRow = 2;
			textureCol = 11;
			break;
		case ' ':
			textureRow = 2;
			textureCol = 25;
			break;
		case '(':
			textureRow = 2;
			textureCol = 12;
			break;
		case ')':
			textureRow = 2;
			textureCol = 13;
			break;
		case '-':
			textureRow = 2;
			textureCol = 14;
			break;
		default:
			textureRow = 2;
			textureCol = 25;
			break;

		}
	}

	void setCrop() {
		crop.x = fontW * textureCol / 26;
		crop.y = fontH * textureRow / 3;
		crop.w = fontW / 26 - 1;
		crop.h = fontH / 3 - 1;
	}

	void setActualCoords() {
		actualW = (*WID * size)/2;
		actualH = *HEI * size;
		actualX = (int)(*WID / 2 + x * *WID / 2 - actualW/2);
		actualY = (int)(*HEI / 2 - y * *HEI / 2 - actualH/2);
	}

	void render(SDL_Renderer* rend, Uint8 R, Uint8 G, Uint8 B) {
		SDL_Rect rect = { actualX, actualY, actualW, actualH };
		SDL_SetTextureColorMod(font, R, G, B);
		SDL_RenderCopy(rend, font, &crop, &rect);
	}


};







class text {
private:
	std::vector<character> characters;
	

public:
	text(int& WID, int& HEI, double x, double y, double size, SDL_Texture* font, std::string string, bool centered) {
		
		if (!centered) {
			for (unsigned int i = 0; i < string.size(); i++) {
				character cha(WID, HEI, x+i*size, y, size, font, string.at(i));
				characters.push_back(cha);
			}
		} else {
			for (unsigned int i = 0; i < string.size(); i++) {
				character cha(WID, HEI, x + i * size - size*(string.size()-1)/2, y, size, font, string.at(i));
				characters.push_back(cha);
			}
		}

	}

	void render(SDL_Renderer* rend, Uint8 R, Uint8 G, Uint8 B) {
		for (unsigned int i = 0; i < characters.size(); i++) {
			characters[i].render(rend, R, G, B);
		}
	}

};











