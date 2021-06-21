#pragma once

#include "ofMain.h"

class Texture {
public:
	Texture() {};

	ofColor getTextureColor(glm::vec2 uv);
	ofColor getMeshTextureColor(glm::vec2 uv, ofImage &tex);

	void addTexture(string path);
	void removeTexture();

	ofImage texture;
	ofImage bumpMap;
	bool hasTexture = false;
	int uvTileFactor = 1;

};