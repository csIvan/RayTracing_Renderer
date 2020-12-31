#include "Cube.h"

Cube::Cube(glm::vec3 p, float s, ofColor diffuse) {
	position = p;
	side = s;
	diffuseColor = diffuse;

}

bool Cube::intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal) {
	// Placeholder intersect
	return (glm::intersectRaySphere(ray.p, ray.d, position, side, point, normal));
}

void Cube::draw() {
	// Placeholder scene object
	ofDrawBox(position, side);

}

float Cube::sdf(const glm::vec3 p) {
	glm::vec3 q = abs(p) - side;
	return MIN(MAX(q.x, max(q.y, q.z)), 0.0) + length(max(q, 0.0f));
}