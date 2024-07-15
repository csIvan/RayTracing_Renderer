#pragma once
#include "ofMain.h"
#include "../Scene/SceneObject.h"

// Camera ViewPlane class; functions as the view that we will render
class ViewPlane : public SceneObject {
private:
	glm::vec2 min, max;

public:
	ViewPlane(const glm::vec2 &min, const glm::vec2 &max, const glm::vec3 &position) : min(min), max(max) { this->position = position; }
	~ViewPlane() {};

	void draw() {
		applyMatrix();
		ofPushMatrix();
		ofMultMatrix(Transform);
		ofDrawRectangle(glm::vec3(min.x, min.y, 0.0f), width(), height());
		ofPopMatrix();
	}

	glm::vec3 toWorld(float u, float v) const {	// (u, v) --> (x, y, z) [ world space ]
		float w = width();
		float h = height();
		return (glm::vec3((u * w) + min.x, (v * h) + min.y, position.z));
	}

	void setSize(glm::vec2 min, glm::vec2 max) {
		this->min = min;
		this->max = max;
	}

	float width() const { return max.x - min.x; }
	float height() const { return max.y - min.y; }
	float getAspect() const { return width() / height(); }
	glm::vec2 topLeft() const { return glm::vec2(min.x, max.y); }
	glm::vec3 topRight() const { return glm::vec3(max, position.z); }
	glm::vec3 bottomLeft() const { return glm::vec3(min, position.z); }
	glm::vec2 bottomRight() const { return glm::vec2(max.x, min.y); }
};