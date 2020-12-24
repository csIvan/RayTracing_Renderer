#include "Sphere.h"

Sphere::Sphere(glm::vec3 p, float r, ofColor diffuse) {
	position = p;
	radius = r;
	diffuseColor = diffuse;
}

bool Sphere::intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal) {
	return (glm::intersectRaySphere(ray.p, ray.d, position, radius, point, normal));
}

void Sphere::draw() {
	ofDrawSphere(position, radius);
}

float Sphere::sdf(glm::vec3 p) {
	float distance = glm::distance(p, position) - radius;
	return distance;
}
