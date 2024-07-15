#pragma once

#include "ofMain.h"
#include "ViewPlane.h"
#include "../Scene/SceneObject.h"

// Rendering Camera
class RenderCam : public SceneObject {
private:
	ViewPlane view;
	float side = 0.25f;

public:
	RenderCam(const glm::vec2 &viewMin, const glm::vec2 &viewMax, const glm::vec3 &viewPos, const glm::vec3 &pos);
	~RenderCam() {};

	// Override functions from SceneObject
	bool intersect(const Ray &ray, HitInfo &hitInfo); 
	void draw();

	// Calculates ray from camera to given point on a viewplane
	Ray getRay(float u, float v) const;

	// Draws camera frustum
	void drawFrustum();

	// Returns current view plane of render cam;
	ViewPlane getViewPlane() const {
		return view;
	}
};