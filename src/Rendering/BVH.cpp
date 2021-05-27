#include "BVH.h"

BVH::BVH() {
	position = glm::vec3(0, 0, 12);
}

bool BVH::intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal, glm::vec2 &uv) {
	glm::vec3 rdd, roo, invdir, sign, t, tMinV, tMaxV, tMin, tMax;
	Ray rTemp = Ray(ray.p, ray.d);

	// Apply Transformation
	rdd = (glm::inverse(Transform) * glm::vec4(ray.d.x, ray.d.y, ray.d.z, 0.0));
	roo = (glm::inverse(Transform) * glm::vec4(ray.p.x, ray.p.y, ray.p.z, 1.0));

	return true;
}

void BVH::draw() {
	applyMatrix();
	if (isSelected) {
		ofSetColor(ofColor::yellow);
		ofNoFill();
		ofPushMatrix();
			ofMultMatrix(Transform);
		ofPopMatrix();
		ofFill();
	}

	ofPushMatrix();
		ofMultMatrix(Transform);
		ofSetColor(ofColor::orange);
	ofPopMatrix();

}