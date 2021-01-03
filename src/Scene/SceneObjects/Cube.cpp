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
	ofPushMatrix();
		//ofRotate(angle, -axisR.x, axisR.y, -axisR.z);

		ofRotateZ(-angle * axisR.z);
		ofRotateX(-angle * axisR.x);
		ofRotateY(-angle * axisR.y);
		ofDrawAxis(2);
		ofDrawBox(position, side);

	ofPopMatrix();
}

float Cube::sdf(const glm::vec3 p) {
	glm::vec3 q = abs(p) - side;
	return MIN(MAX(q.x, max(q.y, q.z)), 0.0) + length(max(q, 0.0f));
}