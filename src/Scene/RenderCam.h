#pragma once

#include "ofMain.h"
#include "SceneObject.h"
#include "ViewPlane.h"

// render camera  - currently must be z axis aligned (we will improve this in project 4)
class RenderCam : public SceneObject {
public:
	RenderCam() {
		position = glm::vec3(0, 0, 12);
		aim = glm::vec3(0, 0, -1);
	}
	Ray getRay(float u, float v);
	void draw() { ofDrawBox(position, 1.0); };
	void drawFrustum() { view.draw(); };

	glm::vec3 aim;
	ViewPlane view;          // The camera viewplane, this is the view that we will render 
};