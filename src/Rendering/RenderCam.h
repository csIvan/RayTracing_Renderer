#pragma once

#include "ofMain.h"
#include "ViewPlane.h"
#include "../Scene/SceneObject.h"

// render camera 
class RenderCam : public SceneObject {
public:
	ViewPlane view;          // The camera viewplane, this is the view that we will render
	float side = 0.25f;

	RenderCam();
	~RenderCam() {};

	bool intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal, ofColor &surfaceColor);
	void draw();
	void drawFrustum(); 

	Ray getRay(float u, float v);
};