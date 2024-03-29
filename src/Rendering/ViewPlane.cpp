#include "ViewPlane.h"

ViewPlane::ViewPlane(glm::vec2 p0, glm::vec2 p1) {
	min = p0;
	max = p1;
}

ViewPlane::ViewPlane() {			// create reasonable defaults (3x2 aspect)
	min = glm::vec2(-1.5, -1);
	max = glm::vec2(1.5, 1);
	position = glm::vec3(0, 0, -3.5);
}

void ViewPlane::draw() {
	applyMatrix();
	ofNoFill();
	ofPushMatrix();
		ofMultMatrix(Transform);
		ofDrawRectangle(glm::vec3(min.x, min.y, 0.0f), width(), height());
	ofPopMatrix();
	ofFill();
}

glm::vec3 ViewPlane::toWorld(float u, float v) {	 // (u, v) --> (x, y, z) [ world space ]
	float w = width();
	float h = height();
	return (glm::vec3((u * w) + min.x, (v * h) + min.y, position.z));
}

void ViewPlane::setSize(glm::vec2 min, glm::vec2 max) {
	this->min = min;
	this->max = max;
}

float ViewPlane::getAspect() {
	return width() / height();
}

float ViewPlane::width() {
	return (max.x - min.x);
}

float ViewPlane::height() {
	return (max.y - min.y);
}
