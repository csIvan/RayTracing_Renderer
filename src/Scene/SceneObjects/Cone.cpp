#include "Cone.h"

Cone::Cone(glm::vec3 p, float h, float r, string name, ofColor diffuse) {
	position = p;
	height = h;
	radius = r;
	diffuseColor = diffuse;
}

bool Cone::intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal) {

	return true;
}

void Cone::draw() {
	applyMatrix();
	if (isSelected) {
		ofDisableLighting();
		ofSetColor(ofColor::yellow);
		ofNoFill();
		ofPushMatrix();
			ofMultMatrix(Transform);
			ofDrawAxis(height * 1.5);
			ofDrawCone(radius, height);
		ofPopMatrix();
		ofFill();
		ofEnableLighting();
	}

	ofSetColor(ofColor::white);
	material.begin();
	material.setDiffuseColor(diffuseColor);
	ofPushMatrix();
		ofMultMatrix(Transform);
		ofDrawCone(radius, height);
	ofPopMatrix();
	material.end();
}

float Cone::sdf(const glm::vec3 p) {
	return 1.0f;
}