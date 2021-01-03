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
	ofPushMatrix();
		ofTranslate(position);
		ofRotate(angle, axisR.x, axisR.y, axisR.z);
		ofDrawAxis(2);
		ofDrawBox(ofVec3f::zero(), side);

	ofPopMatrix();
}

float Cube::sdf(const glm::vec3 p1) {
	glm::mat4 m = glm::translate(glm::mat4(1.0), position);
	glm::mat4 M = glm::rotate(m, glm::radians(angle), axisR);
	glm::vec4 p = glm::inverse(M) * glm::vec4(p1.x, p1.y, p1.z, 1.0);
	glm::vec3 q = abs(p) - side;
	return MIN(MAX(q.x, max(q.y, q.z)), 0.0) + length(max(q, 0.0f));
}