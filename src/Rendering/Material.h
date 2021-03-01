#pragma once

#include "ofMain.h"

class Material {
public:
	enum Mat {	MATTE, MIRROR, GLASS, METAL	};

	Mat mat;
	ofColor diffuseColor = ofColor::grey;   
	ofColor specularColor = ofColor::lightGray;
	string materialString = "Matte";
	float reflectCoeff = 0.0f;

	
	Material();
	ofColor Matte();
	ofColor Mirror();
	ofColor Glass();
	ofColor Metal();

	void setString(string s) { materialString = s; };
	string toString() {	return materialString; };

};