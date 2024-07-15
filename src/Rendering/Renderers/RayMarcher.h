#pragma once

#include "ofMain.h"
#include "Renderer.h"

class RayMarcher : public Renderer {
public:
	RayMarcher() {};
	~RayMarcher() {};

	// Override functions from Renderer
	bool castRay(const Ray &ray, ofColor &color, const Scene &scene, int depth = 0);


	// Moves point along the ray and checks if the point is very near the object's surface
	bool rayMarch(const Ray &ray, const Scene &scene, HitInfo &hitInfo, int &objectHitIndex);


	// Checks every primitive's sdf and determines the closest one to the point
	float sceneSDF(const glm::vec3 &point, const Scene &scene, int &index);


	// Calculates and returns the normal of a given point within the scene
	glm::vec3 getNormalRM(const glm::vec3 &point, const Scene &scene);
};


