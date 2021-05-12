#pragma once

#include "ofMain.h"

class Texture {
public:
	Texture() {};

	ofColor getTextureColor(glm::vec2 uv);

	void addTexture(string path);
	void addBumpMap(string path);
	void removeTexture();
	void removeBumpMap();

	ofImage texture;
	ofImage bumpMap;
	bool hasTexture = false;
	bool hasBumpMap = false;
	int uvTileFactor = 1;

};