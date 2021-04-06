#pragma once

#include "ofMain.h"
#include "../Scene/SceneObject.h"

// view plane for render camera
class  ViewPlane : public SceneObject {
public:
	//  To define an infinite plane, we just need a point and normal.
	//  The ViewPlane is a finite plane so we need to define the boundaries.
	//  We will define this in terms of min, max  in 2D.  
	//  (in local 2D space of the plane)
	//  ultimately, will want to locate the ViewPlane with RenderCam anywhere
	//  in the scene, so it is easier to define the View rectangle in a local'
	//  coordinate system.
	//
	glm::vec2 min, max;

	ViewPlane(glm::vec2 p0, glm::vec2 p1);
	ViewPlane();
	~ViewPlane() {};
	void draw();

	glm::vec3 toWorld(float u, float v);
	void setSize(glm::vec2 min, glm::vec2 max);
	float getAspect();

	float width();
	float height();

	// Some convenience methods for returning the corners
	glm::vec2 topLeft() { return glm::vec2(min.x, max.y); }
	glm::vec3 topRight() { return glm::vec3(max, position.z); }
	glm::vec3 bottomLeft() { return glm::vec3(min, position.z); }
	glm::vec2 bottomRight() { return glm::vec2(max.x, min.y); }
};