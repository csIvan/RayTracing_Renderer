#include "Cylinder.h"
#define EPSILON 0.05

Cylinder::Cylinder(glm::vec3 p, float h, float r, string name, ofColor diffuse) {
	position = p;
	height = h;
	radius = r;
	objName = name;;
	diffuseColor = diffuse;
}

bool Cylinder::intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal) {
	glm::vec3 rdd, roo;

	// Apply Transformation
	glm::vec4 p = glm::inverse(Transform) * glm::vec4(ray.p.x, ray.p.y, ray.p.z, 1.0);
	glm::vec4 p1 = glm::inverse(Transform) * glm::vec4(ray.p + ray.d, 1.0);
	roo = glm::vec4(p.x, p.y, p.z, 1.0);
	rdd = glm::normalize(p1 - p);

	Ray rTemp = Ray(roo, rdd);

	glm::vec3 capA = glm::vec3(0, -height / 2.0f, 0);
	glm::vec3 capB = glm::vec3(0, height / 2.0f, 0);
	float r2 = radius * radius;

	glm::vec3 ba = capB - capA;
	glm::vec3 oa = roo - capA;
	glm::vec3 ob = roo - capB;

	float m0 = glm::dot(ba, ba);
	float m1 = glm::dot(oa, ba);
	float m2 = glm::dot(ob, ba);
	float m3 = glm::dot(rdd, ba);
	float m4 = glm::dot(rdd, oa);
	float m5 = glm::dot(oa, oa);
	float t = FLT_MAX;

	// Caps
	if(m1 < 0.0) {
		if (glm::dot(oa * m3 - rdd * m1, oa * m3 - rdd * m1) < (r2 * m3 * m3)) {
			t = -m1 / m3;
			if (t < EPSILON) return false;

			point = Transform * glm::vec4(rTemp.evalPoint(t), 1.0);
			normal = glm::normalize(getRotateMatrix() * glm::vec4(-ba * glm::inversesqrt(m0), 1.0));
			return true;
		}
	}
	else if (m2 > 0.0) {
		if (glm::dot(ob * m3 - rdd * m2, ob * m3 - rdd * m2) < (r2 * m3 * m3)) {
			t = -m2 / m3;
			if (t < EPSILON) return false;

			point = Transform * glm::vec4(rTemp.evalPoint(t), 1.0);
			normal = glm::normalize(getRotateMatrix() * glm::vec4(ba * glm::inversesqrt(m0), 1.0));
			return true;
		}
	}

	// Body
	float k2 = m0 * m0 - m3 * m3 * m0;
	float k1 = m0 * m0 * m4 - m1 * m3* m0;
	float k0 = m0 * m0 * m5 - m1 * m1 * m0 + m0 * radius * (-m0 * radius);

	float h = k1 * k1 - k2 * k0;
	if (h < 0.0) return false;

	t = (-k1 - sqrt(h)) / k2;
	
	float y = m1 + t * m3;
	if (y > 0.0 && y < m0 && t >= EPSILON) {
		point = Transform * glm::vec4(rTemp.evalPoint(t), 1.0);
		normal = glm::normalize(getRotateMatrix() * glm::vec4((m0 * (m0 * (oa + t * rdd)) - ba * m0 * y), 1.0));
		return true;
	}

	return false;
}

void Cylinder::draw() {
	applyMatrix();
	ofDisableLighting();
	for (int i = 0; i < points.size(); i++) {
		ofSetColor(ofColor::red);
		//ofDrawSphere(points[i], 0.025);
		ofSetColor(ofColor::yellow);
		//ofDrawLine(points[i], normals[i]);
	}

	ofEnableLighting();
	if (isSelected) {
		ofDisableLighting();
		ofSetColor(ofColor::yellow);
		ofNoFill();
		ofPushMatrix();
			ofMultMatrix(Transform);
			ofDrawAxis(height * 1.5);
			ofDrawCylinder(glm::vec3(0, 0, 0), radius, height);
		ofPopMatrix();
		ofFill();
		ofEnableLighting();
	}

	ofSetColor(ofColor::white);
	material.begin();
	material.setDiffuseColor(diffuseColor);
	ofPushMatrix();
		ofMultMatrix(Transform);
		ofDrawCylinder(glm::vec3(0, 0, 0), radius, height);
	ofPopMatrix();
	material.end();
}

float Cylinder::sdf(const glm::vec3 p1) {
	glm::vec4 p = glm::inverse(Transform) * glm::vec4(p1.x, p1.y, p1.z, 1.0);
	glm::vec2 d = abs(glm::vec2(length(glm::vec2(p.x, p.z)), p.y)) - glm::vec2(radius, height/2);
	return glm::min(glm::max(d.x, d.y), 0.0f) + length(glm::max(d, 0.0f));
}