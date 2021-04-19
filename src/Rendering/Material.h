#pragma once

#include "ofMain.h"

class Material {
public:
	enum Mat {	MATTE, MIRROR, GLASS, METAL };

	Mat mat;
	ofColor diffuseColor = ofColor::grey;   
	ofColor specularColor = ofColor::lightGray;
	float reflection = 0.0f;
	float refraction = 0.0f;
	float roughness = 1.0f;
	int shininess = 40;
	string materialString = "Matte";
	
	Material();
	void setMatte();
	void setMirror();
	void setGlass();
	void setMetal();
	Material::Mat getType();

	void setString(string s) { materialString = s; };
	string toString() {	return materialString; };

};