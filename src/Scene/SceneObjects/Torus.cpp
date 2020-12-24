#include "Torus.h"

Torus::Torus(glm::vec3 p, float l1, float l2, ofColor diffuse) {
	position = p;
	innerRadius = l1;
	outerRadius = l2;
	diffuseColor = diffuse;
}

// Constructor with angle and axis of ratation parameters
Torus::Torus(glm::vec3 p, float l1, float l2, float a, glm::vec3 r, ofColor diffuse) {
	position = p;
	innerRadius = l1;
	outerRadius = l2;
	diffuseColor = diffuse;
	angle = a;
	axisR = r;
}

void Torus::draw() {
	// Proxy object in 3D view
	ofDrawSphere(position, innerRadius);
}

// Calculate the sdf of the torus scene object while applying transformations
float Torus::sdf(glm::vec3 p1) {
	// Transformation matrices(translate and rotate)
	glm::mat4 m = glm::translate(glm::mat4(1.0), position);
	glm::mat4 M = glm::rotate(m, glm::radians(angle), axisR);
	glm::vec4 p = glm::inverse(M) * glm::vec4(p1.x, p1.y, p1.z, 1.0);
	glm::vec2 q = glm::vec2(glm::length(glm::vec2(p.x, p.z)) - innerRadius, p.y);
	return glm::length(q) - outerRadius;
}