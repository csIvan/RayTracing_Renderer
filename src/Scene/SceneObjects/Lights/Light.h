#pragma once

#include "ofMain.h"
#include "../../SceneObject.h"

//Point Light class with an intensity variable
class Light : public SceneObject {
public:
	ofMaterial material;
	float intensity = 100;
	float radius = 0.1;

	Light(glm::vec3 p, float i, string name) {
		position = p;
		intensity = i;
		objName = name;
	}

	Light() {}
	~Light() {};

	bool intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal) {
		return (glm::intersectRaySphere(ray.p, ray.d, position, radius, point, normal));
	}

	void draw() {
		if (isSelected) {
			ofSetColor(ofColor::yellow);
			ofNoFill();
			ofPushMatrix();
				ofMultMatrix(Transform);
				ofDrawAxis(radius * 1.5);;
				ofDrawSphere(position, radius);
			ofPopMatrix();
			ofFill();
		}
		ofSetColor(ofColor::cyan);
		ofDrawSphere(position, radius);
	};
};