#pragma once

#include "ofMain.h"
#include "Ray.h"

// Base class for any renderable object in the scene
class SceneObject {
public:
	virtual void draw() = 0;    // pure virtual funcs - must be overloaded
	virtual bool intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal) {
		//cout << "SceneObject::intersect" << endl; 
		return false;
	}
	virtual float sdf(glm::vec3 p) {
		return 0.0f;
	}


	// any data common to all scene objects goes here
	glm::vec3 position = glm::vec3(0, 0, 0);

	// material properties (we will ultimately replace this with a Material class - TBD)
	ofColor diffuseColor = ofColor::grey;    // default colors - can be changed.
	ofColor specularColor = ofColor::lightGray;
	string objName = "SceneObject_";
};