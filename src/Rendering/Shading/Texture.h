#pragma once

#include "ofMain.h"

// Texture class for scene objects
class Texture {
private:
	ofImage texture;
	bool hasTexture = false;
	int uvTileFactor = 1;

public:
	Texture() {};
	~Texture() {};

	// Calculate texture coordinates based on uv input
	ofColor getTextureColor(const glm::vec2 &uv, const ofColor &diffuse);

	// Calculates texture coordinates without tiling
	ofColor getMeshTextureColor(const glm::vec2 &uv, const ofImage &tex);

	// Loads texture file 
	void addTexture(const string &path);

	// Clears loaded texture
	void removeTexture();

	// Setter and getter for UV tiling factor
	void setUVTile(int tileFactor) { uvTileFactor = tileFactor; }
	int getTileFactor() const { return uvTileFactor; }
};