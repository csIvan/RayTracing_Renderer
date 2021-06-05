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
Box::Box(const glm::vec3 &min, const glm::vec3 &max) {
	parameters[0] = min;
	parameters[1] = max;
}

void Box::setParameters(const glm::vec3 &min, const glm::vec3 &max) {
	parameters[0] = min;
	parameters[1] = max;
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

void Box::transformBox(glm::mat4 T) {
	vector<glm::vec3> newCorners;
	for (glm::vec3 v : corners) {
		newCorners.push_back(T * glm::vec4(v.x, v.y, v.z, 1.0));
	}

	glm::vec3 new_min = glm::vec3(100, 100, -100);
	glm::vec3 new_max = glm::vec3(-100, -100, 100);
	for (glm::vec3 v : newCorners) {
		new_min.x = (v.x < new_min.x) ? v.x : new_min.x;
		new_min.y = (v.y < new_min.y) ? v.y : new_min.y;
		new_min.z = (v.z > new_min.z) ? v.z : new_min.z;

		new_max.x = (v.x > new_max.x) ? v.x : new_max.x;
		new_max.y = (v.y > new_max.y) ? v.y : new_max.y;
		new_max.z = (v.z < new_max.z) ? v.z : new_max.z;
	}
	parameters[0] = new_min;
	parameters[1] = new_max;
}

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

bool Box::intersect(const Ray &ray) {
	glm::vec3 rdd, roo, invdir, sign, t, tMinV, tMaxV, tMin, tMax;

	glm::mat4 mtranslate = glm::translate(glm::mat4(1.0), center());

	glm::vec4 p = glm::inverse(mtranslate) * glm::vec4(ray.p.x, ray.p.y, ray.p.z, 1.0);
	glm::vec4 p1 = glm::inverse(mtranslate) * glm::vec4(ray.p + ray.d, 1.0);
	roo = glm::vec4(p.x, p.y, p.z, 1.0);
	rdd = glm::normalize(p1 - p);
	Ray r = Ray(roo, rdd);


	// Calculate intersection
	invdir = 1.0f / rdd;
	sign = glm::vec3((rdd.x < 0.0) ? 1.0 : -1.0, (rdd.y < 0.0) ? 1.0 : -1.0, (rdd.z < 0.0) ? 1.0 : -1.0);
	t = sign * glm::vec3(glm::abs(max().x - min().x)/2, glm::abs(max().y - min().y)/2, glm::abs(max().z - min().z)/2);

	tMinV = glm::vec3(-roo.x + t.x, -roo.y + t.y, -roo.z + t.z);
	tMaxV = glm::vec3(-roo.x - t.x, -roo.y - t.y, -roo.z - t.z);
	tMin = invdir * tMinV;
	tMax = invdir * tMaxV;

	// Calculate entering and exiting points
	float tN = glm::max(glm::max(tMin.x, tMin.y), tMin.z);
	float tF = glm::min(glm::min(tMax.x, tMax.y), tMax.z);

	// Cube distance
	glm::vec3 d = abs(roo) - glm::vec3(glm::abs(max().x - min().x)/2, glm::abs(max().y - min().y)/2, glm::abs(max().z - min().z)/2);
	float cubeDist = MIN(MAX(d.x, glm::max(d.y, d.z)), 0.0) + length(glm::max(d, 0.0f));

	// No intersection
	if (tN > tF || tF < 0.0 || isnan(tF) || tN < cubeDist)
		return false;

	// Point
	//points.push_back(r.evalPoint(tN));

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
	ofSetColor(ofColor::green);
	for (int i = 0; i < points.size(); i++) {
		ofDrawSphere(points[i], 0.1);
	}
	ofSetColor(ofColor::gray);



	ofEnableLighting();
	
}