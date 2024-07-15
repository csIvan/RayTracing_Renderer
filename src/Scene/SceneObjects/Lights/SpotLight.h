#pragma once

#include "Light.h"

// Spot Light scene object class
class SpotLight : public Light {
private:
	glm::vec3 direction;
	float heightRef = 6.0f;
	float totalWidth;
	float falloffStart;

public:
	SpotLight(const glm::vec3 &position, const glm::vec3 &direction, float intensity, float angle, float foAngle, const string &name);
	~SpotLight() {};

	// Draw spotlight cone 
	void draw();

	// Calculate the light depending on the constraint angle and falloff angle
	float falloff(float spotAngle);

	// Setters and Getters
	void setHeightRef(float heightRef) { this->heightRef = heightRef; }
	void setTotalWidth(float totalWidth) { this->totalWidth = totalWidth; }
	void setFalloff(float falloffStart) { this->falloffStart = falloffStart; }
	float getHeightRef() const { return heightRef; }
	float getTotalWidth() const { return totalWidth; }
	float getFalloff() const { return falloffStart; }
	glm::vec3 getDirection() const { return direction; }
};