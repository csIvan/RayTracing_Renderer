#include "Plane.h"

Plane::Plane() {
	normal = glm::vec3(0, 1, 0);
	plane.rotateDeg(90, 1, 0, 0);
}

Plane::Plane(glm::vec3 p, glm::vec3 n, ofColor diffuse, float w, float h) {
	position = p;
	normal = n;
	width = w;
	height = h;
	diffuseColor = diffuse;
	if (normal == glm::vec3(0, 1, 0)) {
		plane.rotateDeg(90, 1, 0, 0);
	}
}

bool Plane::intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal) {
	float dist;
	bool insidePlane = false;
	bool hit = glm::intersectRayPlane(ray.p, ray.d, position, this->normal, dist);
	if (hit) {
		Ray r = ray;
		point = r.evalPoint(dist);
		normal = this->normal;
		glm::vec2 xRange = glm::vec2(position.x - width / 2, position.x + width / 2);
		glm::vec2 zRange = glm::vec2(position.z - height / 2, position.z + height / 2);
		if (point.x < xRange[1] && point.x > xRange[0] && point.z < zRange[1] && point.z > zRange[0]) {
			insidePlane = true;
		}
	}
	return insidePlane;
}

void Plane::draw() {
	plane.setPosition(position);
	plane.setWidth(width);
	plane.setHeight(height);
	plane.setResolution(4, 4);
	plane.drawWireframe();
}

float Plane::sdf(glm::vec3 p) {
	return p.y - position.y;
}

glm::vec3 Plane::getNormal(const glm::vec3 &p) {
	return this->normal;
}