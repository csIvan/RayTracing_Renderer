#pragma once

#include "ofMain.h"

class Material {
public:
	enum Mat {	MATTE, MIRROR, GLASS, METAL	};

	Mat mat;
	ofColor diffuseColor = ofColor::grey;   
	ofColor specularColor = ofColor::lightGray;
	float reflection = 0.0f;
	float refraction = 0.0f;
	float roughness = 0.0f;
	string materialString = "Matte";
	
	Material();
	ofColor Matte();
	ofColor Mirror();
	ofColor Glass();
	ofColor Metal();

	void setString(string s) { materialString = s; };
	string toString() {	return materialString; };

};