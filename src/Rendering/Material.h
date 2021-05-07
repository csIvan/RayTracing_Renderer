#pragma once

#include "ofMain.h"

class Material {
public:
	enum Mat {	MATTE, MIRROR, GLASS, REFLECTIVE  };

	Mat mat;
	ofColor diffuseColor = ofColor::grey;   
	ofColor specularColor = ofColor::white;
	float specularCoeff = 1.0f;
	float ambient = 0.08f;
	float refraction = 0.0f;
	float reflectiveCoeff = 0.2f;
	float roughness = 1.0f;
	int shininess = 40;
	string materialString = "Matte";
	
	Material();
	void setMatte();
	void setReflective();
	void setGlass();
	void setMirror();
	Material::Mat getType();

	void setString(string s) { materialString = s; };
	string toString() {	return materialString; };

};