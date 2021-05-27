#include "Box.h"

/*
 * Ray-box intersection using IEEE numerical properties to ensure that the
 * test is both robust and efficient, as described in:
 *
 *      Amy Williams, Steve Barrus, R. Keith Morley, and Peter Shirley
 *      "An Efficient and Robust Ray-Box Intersection Algorithm"
 *      Journal of graphics tools, 10(1):49-54, 2005
 *
 */

bool Box::intersect(const Ray &r, float t0, float t1) const {
	float tmin, tmax, tymin, tymax, tzmin, tzmax;

	tmin = (parameters[r.sign[0]].x - r.p.x) * r.inv_direction.x;
	tmax = (parameters[1 - r.sign[0]].x - r.p.x) * r.inv_direction.x;
	tymin = (parameters[r.sign[1]].y - r.p.y) * r.inv_direction.y;
	tymax = (parameters[1 - r.sign[1]].y - r.p.y) * r.inv_direction.y;
	if ((tmin > tymax) || (tymin > tmax))
		return false;
	if (tymin > tmin)
		tmin = tymin;
	if (tymax < tmax)
		tmax = tymax;
	tzmin = (parameters[r.sign[2]].z - r.p.z) * r.inv_direction.z;
	tzmax = (parameters[1 - r.sign[2]].z - r.p.z) * r.inv_direction.z;
	if ((tmin > tzmax) || (tzmin > tmax))
		return false;
	if (tzmin > tmin)
		tmin = tzmin;
	if (tzmax < tmax)
		tmax = tzmax;
	return ((tmin < t1) && (tmax > t0));
}

bool Box::intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal, glm::vec2 &uv) {
	return true;
}

void Box::draw() {
	glm::vec3 min = parameters[0];
	glm::vec3 max = parameters[1];
	glm::vec3 size = max - min;
	glm::vec3 center = size / 2 + min;
	glm::vec3 p = glm::vec3(center.x, center.y, center.z);
	float w = size.x;
	float h = size.y;
	float d = size.z;
	ofDisableLighting();
	ofSetColor(ofColor::green);
	ofNoFill();
	ofDrawBox(p, w, h, d);
	ofFill();
	ofSetColor(ofColor::gray);
	ofEnableLighting();
	
}