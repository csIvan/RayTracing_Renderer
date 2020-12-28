#pragma once

#include "ofMain.h"
#include "SceneObject.h"
#include "ViewPlane.h"

// render camera  - currently must be z axis aligned (we will improve this in project 4)
class RenderCam : public SceneObject {
public:
	glm::vec3 aim;
	ViewPlane view;          // The camera viewplane, this is the view that we will render

	RenderCam();
	Ray getRay(float u, float v);
	void draw();
	void drawFrustum(); 
};