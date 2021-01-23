#include "Cube.h"

Cube::Cube(glm::vec3 p, float s, string name, ofColor diffuse) {
	position = p;
	side = s / 2;
	objName = name;
	diffuseColor = diffuse;
}

bool Cube::intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal) {
	glm::vec3 rdd, roo, invdir, sign, t, tMinV, tMaxV, tMin, tMax;
	Ray rTemp = Ray(ray.p, ray.d);

	// Apply Transformation
	rdd = (glm::inverse(Transform) * glm::vec4(ray.d.x, ray.d.y, ray.d.z, 0.0));
	roo = (glm::inverse(Transform) * glm::vec4(ray.p.x, ray.p.y, ray.p.z, 1.0));

	// Calculate intersection
	invdir = 1.0f / rdd;
	sign = glm::vec3((rdd.x < 0.0) ? 1.0 : -1.0, (rdd.y < 0.0) ? 1.0 : -1.0, (rdd.z < 0.0) ? 1.0 : -1.0);
	t = sign * glm::vec3(side, side, side);

	tMinV = glm::vec3(-roo.x + t.x, -roo.y + t.y, -roo.z + t.z);
	tMaxV = glm::vec3(-roo.x - t.x, -roo.y - t.y, -roo.z - t.z);
	tMin = invdir * tMinV;
	tMax = invdir * tMaxV;

	// Calculate entering and exiting points
	float tN = max(max(tMin.x, tMin.y), tMin.z);
	float tF = min(min(tMax.x, tMax.y), tMax.z);

	// Cube distance
	glm::vec3 d = abs(roo) - side;
	float cubeDist = MIN(MAX(d.x, max(d.y, d.z)), 0.0) + length(max(d, 0.0f));

	// No intersection
	if (tN > tF || tF < 0.0 || isnan(tF) || tN < cubeDist)
		return false;

	// Point
	point = rTemp.evalPoint(tN);

	// Normal
	if (tMin.x > tMin.y && tMin.x > tMin.z)
		normal = glm::vec3(Transform[0].x * sign.x, Transform[0].y*sign.x, Transform[0].z*sign.x);
	else if (tMin.y > tMin.z)
		normal = glm::vec3(Transform[1].x * sign.y, Transform[1].y*sign.y, Transform[1].z*sign.y);
	else
		normal = glm::vec3(Transform[2].x * sign.z, Transform[2].y*sign.z, Transform[2].z*sign.z);

	return true;
}

void Cube::draw() {
	applyMatrix();
	if (isSelected) {
		ofDisableLighting();
		ofSetColor(ofColor::yellow);
		ofNoFill();
		ofPushMatrix();
			ofMultMatrix(Transform);
			ofDrawAxis(side * 1.5);
			ofDrawBox(ofVec3f::zero(), side * 2);
		ofPopMatrix();
		ofFill();
		ofEnableLighting();
	}

	ofSetColor(ofColor::white);
	material.begin();
	material.setDiffuseColor(diffuseColor);
	ofPushMatrix();
		ofMultMatrix(Transform);
		ofDrawBox(ofVec3f::zero(), side*2);
	ofPopMatrix();
	material.end();
}

float Cube::sdf(const glm::vec3 p1) {
	glm::mat4 m = glm::translate(glm::mat4(1.0), position);
	glm::mat4 M = glm::rotate(m, glm::radians(angle), axisR);
	glm::vec4 p = glm::inverse(M) * glm::vec4(p1.x, p1.y, p1.z, 1.0);
	glm::vec3 q = abs(p) - side;
	return MIN(MAX(q.x, max(q.y, q.z)), 0.0) + length(max(q, 0.0f));
}