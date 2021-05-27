#pragma once

#include "ofMain.h"

// General Purpose Ray class 
class Ray {
public:
	glm::vec3 p, d;
	glm::vec3 inv_direction;
	int sign[3];

	Ray(glm::vec3 p, glm::vec3 d) { 
		this->p = p; 
		this->d = d; 
		inv_direction = glm::vec3(1 / d.x, 1 / d.y, 1 / d.z);
		sign[0] = (inv_direction.x < 0);
		sign[1] = (inv_direction.y < 0);
		sign[2] = (inv_direction.z < 0);
	}

	void draw(float t) { 
		ofDrawLine(p, p + t * d); 
	}

	glm::vec3 evalPoint(float t) {
		return (p + t * d);
	}
};