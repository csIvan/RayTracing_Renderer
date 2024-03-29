#pragma once

#include "ofMain.h"
#include "../../SceneObject.h"

//Point Light class with an intensity variable
class Light : public SceneObject {
public:
	float intensity = 6;
	float radius = 0.1;

	Light(glm::vec3 p, float i, string name) {
		position = p;
		intensity = i;
		objName = name;
	}

	Light() {}
	~Light() {};

	bool intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal, ofColor &surfaceColor) {
		return (glm::intersectRaySphere(ray.p, ray.d, position, radius, point, normal));
	}

	void draw() {
		applyMatrix();
		if (isSelected) {
			ofSetColor(ofColor::yellow);
			ofNoFill();
			ofPushMatrix();
				ofMultMatrix(Transform);
				ofDrawAxis(radius * 1.5);;
				ofDrawSphere(ofVec3f::zero(), radius);
			ofPopMatrix();
			ofFill();
		}
		ofSetColor(ofColor::cyan);
		ofPushMatrix();
			ofMultMatrix(Transform);
			ofDrawSphere(ofVec3f::zero(), radius);
		ofPopMatrix();
	};

	glm::vec3 getLightDir(glm::vec3 pos, glm::vec3 hit) {
		return (pos - hit);
	}

	double  getLightDist(glm::vec3 pos, glm::vec3 hit) {
		return glm::length(hit - pos);
	}

	double  getLightIntensity(float i, double dist) {
		return i / glm::pow(dist, 2);
	}
};