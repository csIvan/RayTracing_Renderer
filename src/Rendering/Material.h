#pragma once

#include "ofMain.h"

class Material {
public:
	enum Mat {	MATTE, MIRROR, GLASS, METAl	};

	Mat mat;
	ofColor diffuseColor = ofColor::grey;   
	ofColor specularColor = ofColor::lightGray;
	float reflectCoeff = 0.0f;

	
	Material();
	ofColor Matte();
	ofColor Mirror();
	ofColor Glass();
	ofColor Metal();

};