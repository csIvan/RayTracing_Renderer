#include "Cube.h"

Cube::Cube(glm::vec3 p, float s, string name, ofColor diffuse) {
	position = p;
	side = s / 2;
	objName = name;
	objMaterial.diffuseColor = diffuse;
	box = new Box();
	applyMatrix();
	setBounds();
}

void Cube::setBounds() {
	applyMatrix();
	glm::vec3 min = glm::vec3(-side, -side, side);
	glm::vec3 max = glm::vec3(side, side, -side);
	box->setParameters(min, max);
	box->transformBox(Transform);
}


bool Cube::intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal, glm::vec2 &uv) {
	glm::vec3 rdd, roo, invdir, sign, t, tMinV, tMaxV, tMin, tMax;

	// Apply Transformation
	glm::vec4 p = glm::inverse(Transform) * glm::vec4(ray.p.x, ray.p.y, ray.p.z, 1.0);
	glm::vec4 p1 = glm::inverse(Transform) * glm::vec4(ray.p + ray.d, 1.0);
	roo = glm::vec4(p.x, p.y, p.z, 1.0);
	rdd = glm::normalize(p1 - p);
	Ray r = Ray(roo, rdd);


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
	point = r.evalPoint(tN);
	uv = getUV(point);

	point = Transform * glm::vec4(point.x, point.y, point.z, 1.0);
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
	sceneMaterial.begin();
	sceneMaterial.setDiffuseColor(objMaterial.diffuseColor);
	ofPushMatrix();
		ofMultMatrix(Transform);
		ofDrawBox(ofVec3f::zero(), side*2);
	ofPopMatrix();
	sceneMaterial.end();
}

float Cube::sdf(const glm::vec3 p1) {
	glm::vec4 p = glm::inverse(Transform) * glm::vec4(p1.x, p1.y, p1.z, 1.0);
	glm::vec3 q = abs(p) - side;
	return MIN(MAX(q.x, max(q.y, q.z)), 0.0) + length(max(q, 0.0f));
}

glm::vec2 Cube::getUV(glm::vec3 p) {
	glm::vec4 pp = getTranslateMatrix() * glm::vec4(p.x, p.y, p.z, 1.0);
	glm::vec3 hit = glm::vec4(pp.x, pp.y, pp.z, 1.0);

	glm::vec3 np = hit - position;
	float u, v;
	float eps = 0.001f;
	if (abs((np.x / side)) >= (1.0f - eps) && abs((np.x / side)) <= (1.0f + eps)) {
		u = (np.z + 1.0f) / 2.0f;
		v = (np.y + 1.0f) / 2.0f;
	} 
	else if (abs((np.y / side)) >= (1.0f - eps) && abs((np.y / side)) <= (1.0f + eps)) {
		u = (np.x + 1.0f) / 2.0f;
		v = (np.z + 1.0f) / 2.0f;
	}
	else if (abs((np.z / side)) >= (1.0f - eps) && abs((np.z / side)) <= (1.0f + eps)) {
		u = (np.x + 1.0f) / 2.0f;
		v = (np.y + 1.0f) / 2.0f;
	}


	return  glm::vec2(glm::abs(u), glm::abs(v));
}