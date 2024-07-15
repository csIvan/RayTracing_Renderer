#pragma once

#include "ofMain.h"
#include "../../SceneObject.h"
#include "../../../Application/Definitions.h"

// Base Light class that functions as a point light
class Light : public SceneObject {
protected:
	float intensity = 6.5f;
	float radius = 0.1f;

public:

	Light(const glm::vec3 &p, float i, const string &name) {
		position = p;
		intensity = i;
		objName = name;
	}
	~Light() {};

	bool intersect(const Ray &ray, HitInfo &hitInfo) {
		return (glm::intersectRaySphere(ray.p, ray.d, position, radius, hitInfo.point, hitInfo.normal));
	}

	void draw() {
		applyMatrix();
		if (isSelected) {
			glLineWidth(0.1f);

			ofSetColor(SELECTED_COLOR);
			ofPushMatrix();
			ofMultMatrix(Transform);
			ofDrawAxis(radius * 1.5f);
			ofDrawSphere(ZERO_VECTOR, radius);
			ofPopMatrix();

			glLineWidth(1.0f);
		}

		ofSetColor(ofColor::cyan);
		ofPushMatrix();
		ofMultMatrix(Transform);
		ofDrawSphere(ZERO_VECTOR, radius);
		ofPopMatrix();
	}

	// Get the direction from hit point to light position
	virtual glm::vec3 getLightDirection(const glm::vec3 &hit) const {
		return glm::normalize(position - hit);
	}

	// Get the distance between the hit point and the light position
	virtual double getDistance(const glm::vec3 &hit) const {
		return glm::distance(position, hit);
	}

	// Calculate the intensity with dropoff for the light
	virtual double getIntensity(double dist) {
		return intensity / glm::max((dist * dist), 1.0);
	}

	// Setters and Getters
	void setIntensity(float intensity) { this->intensity = intensity; }
	float getIntensity() const { return intensity; }

};