#pragma once

#include "ofMain.h"
#include "../../Application/Definitions.h"

// Predefined Materials
enum class Mat { MATTE, MIRROR, GLASS, REFLECTIVE };

// Material class for scene objects
class Material {
private:
	Mat mat;
	ofColor diffuseColor = DEFAULT_COLOR;
	ofColor specularColor = DEFAULT_COLOR;
	float specularCoeff = 0.5f;
	float ambientCoeff = 0.1f;
	float refractionCoeff = 0.0f;
	float reflectiveCoeff = 0.2f;
	float roughness = 0.8f;
	int shininess = 32;
	string materialString = "Matte";

public:
	Material();
	~Material() {};

	// Pre-defined material setters
	void setMatte();
	void setReflective();
	void setGlass();
	void setMirror();


	// Getters and Setters
	Mat getType() const { return mat; }
	ofColor getDiffuse() const { return diffuseColor; }
	ofColor getSpecular() const { return specularColor; }
	string toString() const { return materialString; }
	float getAmbient() const { return ambientCoeff; }
	float getRoughness() const { return roughness; }
	float getSpecularCoeff() const { return specularCoeff; }
	float getShininess() const { return shininess; }
	float getRefraction() const { return refractionCoeff; }
	float getReflective() const { return reflectiveCoeff; }
	bool isReflective() const { return mat == Mat::REFLECTIVE; }

	void SetMat(const string &material);
	void setDiffuse(const ofColor &color) { diffuseColor = color; }
	void setSpecular(const ofColor &color) { specularColor = color; }

};