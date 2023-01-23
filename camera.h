#pragma once

#include <iostream>
#include <stdio.h>
#include <math.h>
#include <string>

#include "vector.h"

struct xy;

class camera {
private:
	//this holds the direction vector and also the plane  a,b,c (ax+by+cz+d=0)
	vector direction;
	//this holds the x component of a transformed point
	vector xComponent;
	//this holds the y component of a transformed point
	vector yComponent;
	//this defines the camera's position in the world
	vector cameraCenter;
	//this defines the plane (ax+by+cz+d=0) d value
	double d = 0;
	//used for pixel conversion, need window size
	int WID;
	int HEI;



public:
	//CONSTRUCTORS
	camera(){
		WID = 0;
		HEI = 0;
	}
	camera(vector v, vector p, int WID, int HEI, double camDist) {

		////////////////////////////////////////////////////////////////////////////////////////////UPDATE THE DIRECTION VECTOR
		direction = v;
		direction.setSize(camDist);
		xComponent = direction.xComponent();
		yComponent = direction.yComponent();
		////////////////////////////////////////////////////////////////////////////////////////////UPDATE THE CAMERA CENTER
		cameraCenter = p;
		////////////////////////////////////////////////////////////////////////////////////////////CALCULATE D
		d = direction * direction;
		////////////////////////////////////////////////////////////////////////////////////////////UPDATE SCREEN SIZE FOR RENDERING
		this->WID = WID;
		this->HEI = HEI;
		


	}
	










	//GETTERS
	vector getDirection() { return direction; }
	vector getCenter() { return cameraCenter; }
	vector getXComponent() { return xComponent; }
	vector getYComponent() { return yComponent; }
	int getWID() { return WID; }
	int getHEI() { return HEI; }




	//print camera for debugging
	void printCamera(int precision) {
		std::cout << std::fixed << std::setprecision(precision) << "Eyes(" << direction.getX() << " , " << direction.getY() << " , " << direction.getZ() << ") --- Size:" << direction.getVectorSize() << std::endl;
		std::cout << std::fixed << std::setprecision(precision) << "X Component(" << xComponent.getX() << " , " << xComponent.getY() << " , " << xComponent.getZ() << ") --- Size:" << xComponent.getVectorSize() << std::endl;
		std::cout << std::fixed << std::setprecision(precision) << "Y Component(" << yComponent.getX() << " , " << yComponent.getY() << " , " << yComponent.getZ() << ") --- Size:" << yComponent.getVectorSize() << std::endl;
		std::cout << std::fixed << std::setprecision(precision) << "Camera Center(" << cameraCenter.getX() << " , " << cameraCenter.getY() << " , " << cameraCenter.getZ() << ")\n";
	}



	//RENDER POINT FROM 3D TO 2D
	xy renderPoint(vector point) {
		
		//create the vector outsideVector(camera origin , point you want to render)
		vector outsideVector(cameraCenter, point);

		//make a vector collinear to outside vector but the terminal lies in the plane: transVector. k is the coefficient of the transformation
		double k = d / (direction * outsideVector);

		//return null point if the vector is behind the camera
		if (k<0) {
			return xy();
		}

		//transform the outside vector into the transVector
		vector transVector = outsideVector;
		transVector.multiplyConstant(k);

		//create vectorInPlane, origin in the center of the plane (cameraCenter+direction) and end in the terminal of transVector
		vector vectorInPlane(direction, transVector);

		//final step: the scalar product has 3 definitions, and one of them is the orthogonal projection of one vector onto the other;
		//because of this vectorInPlane � xComp = ||vectorInPlane projected onto xComp line||*||xComp|| = x (pixel x). the same for y, and this works because ||xComp||=1.

		double pointX = vectorInPlane * xComponent;
		double pointY = vectorInPlane * yComponent;

		//safeguard to prevent overflow
		if (pointX > WID * 2.0){
			return xy();
		}
		else if (pointX < WID * -2.0) {
			return xy();
		}

		if (pointY > HEI * 2.0) {
			return xy();
		}
		else if (pointY < HEI * -2.0) {
			return xy();
		}

		//coordinates of the pixels being sent back
		double x, y;
		if (WID > HEI) {
			y = (-WID * pointY);
			x = (WID * pointX);
		}
		else {
			y = (-HEI * pointY);
			x = (HEI * pointX);
		}
		



		//safeguard to prevent overflow
		if (x > WID * 2.0) {
			return xy();
		}
		else if (x < WID * -2.0) {
			return xy();
		}

		if (y > HEI * 2.0) {
			return xy();
		}
		else if (y < HEI * -2.0) {
			return xy();
		}





		//returns the pixel coordinates
		return xy(x, y);
	}



};