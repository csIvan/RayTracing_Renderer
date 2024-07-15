#include "AABB.h"

// ------------------------------------------------------
AABB::AABB(const glm::vec3 &min, const glm::vec3 &max) {
	bounds[0] = min;
	bounds[1] = max;
}

// ------------------------------------------------------
void AABB::setParameters(const glm::vec3 &min, const glm::vec3 &max) {
	bounds[0] = min;
	bounds[1] = max;
	corners.clear();
	corners.push_back(glm::vec3(min.x, min.y, min.z));
	corners.push_back(glm::vec3(min.x, max.y, min.z));
	corners.push_back(glm::vec3(max.x, max.y, min.z));
	corners.push_back(glm::vec3(max.x, min.y, min.z));
	corners.push_back(glm::vec3(max.x, max.y, max.z));
	corners.push_back(glm::vec3(min.x, max.y, max.z));
	corners.push_back(glm::vec3(min.x, min.y, max.z));
	corners.push_back(glm::vec3(max.x, min.y, max.z));
}

// ------------------------------------------------------
void AABB::transformBox(const glm::mat4& T) {
	vector<glm::vec3> newCorners;
	glm::vec3 new_min = glm::vec3(100, 100, -100);
	glm::vec3 new_max = glm::vec3(-100, -100, 100);

	// Apply transformation to every corner
	for (glm::vec3 v : corners) {
		newCorners.push_back(T * glm::vec4(v, 1.0));
	}

	// Adjust new min and max
	for (glm::vec3 v : newCorners) {
		new_min.x = glm::min(v.x, new_min.x);
		new_min.y = glm::min(v.y, new_min.y);
		new_min.z = glm::max(v.z, new_min.z);

		new_max.x = glm::max(v.x, new_max.x);
		new_max.y = glm::max(v.y, new_max.y);
		new_max.z = glm::min(v.z, new_max.z);
	}
	setParameters(new_min, new_max);
}

// ------------------------------------------------------
// Ray-box intersection based on Scratchapixel's ray-box intersection tutorial.
// Source: https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-box-intersection.html
bool AABB::intersect(const Ray& ray) const {
	float tmin, tmax, tymin, tymax, tzmin, tzmax;

	tmin = (bounds[ray.sign[0]].x - ray.p.x) * ray.inv_dir.x;
	tmax = (bounds[1 - ray.sign[0]].x - ray.p.x) * ray.inv_dir.x;
	tymin = (bounds[ray.sign[1]].y - ray.p.y) * ray.inv_dir.y;
	tymax = (bounds[1 - ray.sign[1]].y - ray.p.y) * ray.inv_dir.y;
	tzmin = (bounds[ray.sign[2]].z - ray.p.z) * ray.inv_dir.z;
	tzmax = (bounds[1 - ray.sign[2]].z - ray.p.z) * ray.inv_dir.z;

	// Exit if ray misses bounding box
	if ((tmin > tymax) || (tymin > tmax))
		return false;

	// Update tmin and tmax for Y plane
	if (tymin > tmin) tmin = tymin;
	if (tymax < tmax) tmax = tymax;

	// Check ray intersection with Z plane
	if ((tmin > tzmax) || (tzmin > tmax))
		return false;

	// Update tmin and tmax for Z plane
	if (tzmin > tmin) tmin = tzmin;
	if (tzmax < tmax) tmax = tzmax;

	return true;
}

// ------------------------------------------------------
void AABB::draw() {
	glm::vec3 size = getMax() - getMin();
	float w = size.x;
	float h = size.y;
	float d = size.z;

	// Set up and draw wireframe box
	ofDisableLighting();

	ofSetColor(ofColor::green);
	ofNoFill();
	ofDrawBox(getCenter(), w, h, d);
	ofFill();
	ofSetColor(ofColor::white);

	ofEnableLighting();
}