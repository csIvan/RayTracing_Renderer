#include "Cylinder.h"

Cylinder::Cylinder(glm::vec3 p, float h, float r, string name, ofColor diffuse) {
	position = p;
	height = h;
	radius = r;
	diffuseColor = diffuse;
}

bool Cylinder::intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal) {

	return true;
}

void Cylinder::draw() {
	applyMatrix();
	if (isSelected) {
		ofDisableLighting();
		ofSetColor(ofColor::yellow);
		ofNoFill();
		ofPushMatrix();
			ofMultMatrix(Transform);
			ofDrawAxis(height * 1.5);
			ofDrawCylinder(glm::vec3(0, 0, 0), radius, height);
		ofPopMatrix();
		ofFill();
		ofEnableLighting();
	}

	ofSetColor(ofColor::white);
	material.begin();
	material.setDiffuseColor(diffuseColor);
	ofPushMatrix();
		ofMultMatrix(Transform);
		ofDrawCylinder(glm::vec3(0, 0, 0), radius, height);
	ofPopMatrix();
	material.end();
}

float Cylinder::sdf(const glm::vec3 p) {
	return 1.0f;
}