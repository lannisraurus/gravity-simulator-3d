#pragma once

#include <iostream>
#include <stdio.h>
#include <math.h>
#include <string>
#include <sstream>





struct xy {
	double x;
	double y;
	bool visible;
	xy() { x = 0; y = 0; visible = false; }
	xy(double x, double y) {
		this->x = x;
		this->y = y;
		visible = true;
	}
	xy(xy start, xy end) {
		x = end.x - start.x;
		y = end.y - start.y;
		visible = true;
	}
	double size() {
		return sqrt((double)(x * x) + (double)(y * y));
	}
};





class vector {
private:
	//vector coordinates (x,y,z)
	double x = 0;
	double y = 0;
	double z = 0;



public:
	//constructor: creates the vector either with 3 coordinates or 2 points (points and vectors are the same thing)
	vector() {}
	vector(double x, double y, double z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
	vector(vector start, vector end) {
		x = end.getX() - start.getX();
		y = end.getY() - start.getY();
		z = end.getZ() - start.getZ();
	}

	//invert the vector coordinates
	void invertVector() {
		x *= -1;
		y *= -1;
		z *= -1;
	}

	//Get vector size
	double getVectorSize() {
		return sqrt(x * x + y * y + z * z);
	}



	//Get X, Get Y, Get Z
	double getX() { return x; }
	double getY() { return y; }
	double getZ() { return z; }



	//Normalize Vector (make it become unit one vector)
	void normalizeVector() {
		double k = 1 / getVectorSize();
		x *= k;
		y *= k;
		z *= k;
	}

	//Print vector onto the console for debugging
	void printVector(std::string name) {
		std::cout << name << "(" << x << " , " << y << " , " << z << ") --- Size: " << getVectorSize() << std::endl;
	}

	//return vector as a string
	std::string getString(int n) {
		std::stringstream stream;
		stream << "(";
		stream << std::fixed << std::setprecision(n) << x;
		stream << " , ";
		stream << std::fixed << std::setprecision(n) << y;
		stream << " , ";
		stream << std::fixed << std::setprecision(n) << z;
		stream << ")";
		return stream.str();
	}



	//set vector size to certain number
	void setSize(double k) {
		normalizeVector();
		x *= k; y *= k; z *= k;
	}

	//multiply by constant
	void multiplyConstant(double k) {
		x *= k; y *= k; z *= k;
	}


	//horizontal component vector (UNIT VECTOR)
	vector xComponent() {
		vector result(-y, x, 0);
		result.normalizeVector();
		return result;
	}

	//vertical component vector (UNIT VECTOR)
	vector yComponent() {
		vector result(-z * x, -z * y, x * x + y * y);
		result.normalizeVector();
		return result;
	}


	//OPERATOR OVERLOADING OPERATIONS
	// ADD VECTOR OPERATOR
	vector operator + (vector const& obj) {
		vector result;
		result.x = x + obj.x;
		result.y = y + obj.y;
		result.z = z + obj.z;
		return result;
	}

	//SCALAR PRODUCT
	double operator * (vector const& obj) {
		double result = x * obj.x + y * obj.y + z * obj.z;
		return result;
	}


};




