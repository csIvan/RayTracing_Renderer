#pragma once

#include "ofMain.h"
#include "../SceneObject.h"

//************************************** Torus Class *********************************
class Torus : public SceneObject {
public:
	float innerRadius;
	float outerRadius;
	float angle = 45.0f;
	glm::vec3 axisR = glm::vec3(1, 0, 0);

	Torus() {};
	Torus(glm::vec3 p, float l1, float l2, ofColor diffuse = ofColor::azure) {
		position = p;
		innerRadius = l1;
		outerRadius = l2;
		diffuseColor = diffuse;
	}

	//constructor with angle and axis of rotation parameters
	Torus(glm::vec3 p, float l1, float l2, float a, glm::vec3 r, ofColor diffuse = ofColor::azure) {
		position = p;
		innerRadius = l1;
		outerRadius = l2;
		diffuseColor = diffuse;
		angle = a;
		axisR = r;
	}

	//Calculate the sdf of the torus scene object while applying transformations
	float sdf(glm::vec3 p1) {
		//Transformation matrices(translate and rotate)
		glm::mat4 m = glm::translate(glm::mat4(1.0), position);
		glm::mat4 M = glm::rotate(m, glm::radians(angle), axisR);
		glm::vec4 p = glm::inverse(M) * glm::vec4(p1.x, p1.y, p1.z, 1.0);
		glm::vec2 q = glm::vec2(glm::length(glm::vec2(p.x, p.z)) - innerRadius, p.y);
		return glm::length(q) - outerRadius;
	}


	void draw() {
		//draw proxy object in 3D view
		ofDrawSphere(position, innerRadius);
	}
};
