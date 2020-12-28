#pragma once

#include "ofMain.h"
#include "SceneObjects/Plane.h"

// view plane for render camera
class  ViewPlane : public Plane {
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
	void draw();

	glm::vec3 toWorld(float u, float v);
	void setSize(glm::vec2 min, glm::vec2 max);
	float getAspect();

	float width();
	float height();

	// Some convenience methods for returning the corners
	glm::vec2 topLeft() { return glm::vec2(min.x, max.y); }
	glm::vec2 topRight() { return max; }
	glm::vec2 bottomLeft() { return min; }
	glm::vec2 bottomRight() { return glm::vec2(max.x, min.y); }
};