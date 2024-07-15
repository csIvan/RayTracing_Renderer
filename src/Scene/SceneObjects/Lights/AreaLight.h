#pragma once

#include "Light.h"
#include "../Plane.h"

// Area Light scene object class
class AreaLight : public Light {
private:
	Plane *plane;
	glm::vec3 direction;
	float height = 1;
	float width = 1;
	glm::vec3 corner, uVec, vVec;
	int uSteps, vSteps, samples;

public:
	AreaLight(const glm::vec3 &position, const glm::vec3 &direction, float intensity, float height, float width, const string &name);
	~AreaLight() {};

	// Override functions from SceneObject
	bool intersect(const Ray &ray, HitInfo &hitInfo);
	void draw();


	// Override functions from Light
	glm::vec3 getLightDirection(const glm::vec3 &sample, const glm::vec3 &hit) const {
		return glm::normalize(sample - hit);
	}
	double getDistance(const glm::vec3 &sample, const glm::vec3 &hit) const {
		return glm::distance(sample, hit);
	}
	double getIntensity(double dist) {
		dist = std::max(dist, 4.0);
		return (intensity / samples) / glm::pow(dist, 2);
	}

	// Query for a point on the area light
	glm::vec3 pointOnLight(int u, int v);


	// Setters and Getters
	glm::vec3 getDirection() const { return direction; }
	int getUSteps() const { return uSteps; }
	int getVSteps() const { return vSteps; }
};