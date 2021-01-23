#include "Plane.h"

Plane::Plane() {
	normal = glm::vec3(0, 1, 0);
	plane.rotateDeg(90, 1, 0, 0);
}

Plane::Plane(glm::vec3 p, glm::vec3 n, string name, ofColor diffuse, float w, float h) {
	position = p;
	normal = n;
	width = w;
	height = h;
	objName = name;
	diffuseColor = diffuse;
	if (normal == glm::vec3(0, 1, 0)) {
		plane.rotateDeg(90, 1, 0, 0);
	}
}

bool Plane::intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal) {
	glm::vec3 rdd, roo;
	glm::vec4 p = glm::inverse(Transform) * glm::vec4(ray.p.x, ray.p.y, ray.p.z, 1.0);
	glm::vec4 p1 = glm::inverse(Transform) * glm::vec4(ray.p + ray.d, 1.0);
	roo = glm::vec4(p.x, p.y, p.z, 1.0);
	rdd = glm::normalize(p1 - p);

	float dist;
	bool insidePlane = false;
	bool hit = glm::intersectRayPlane(roo, rdd, position, this->normal, dist);
	if (hit) {
		Ray r = ray;
		point = r.evalPoint(dist);
		normal = this->normal;
		point = Transform * glm::vec4(point.x, point.y, point.z, 1.0);
		normal = glm::normalize(getRotateMatrix() * glm::vec4(normal.x, normal.y, normal.z, 1.0));
		glm::vec2 xRange = glm::vec2(position.x - width / 2, position.x + width / 2);
		glm::vec2 zRange = glm::vec2(position.z - height / 2, position.z + height / 2);
		if (point.x < xRange[1] && point.x > xRange[0] && point.z < zRange[1] && point.z > zRange[0]) {
			insidePlane = true;
		}
	}
	return insidePlane;
}

void Plane::draw() {
	applyMatrix();
	plane.setPosition(position);
	plane.setWidth(width);
	plane.setHeight(height);
	plane.setResolution(4, 4);
	plane.setOrientation(glm::vec3(270, 0, 0));

	if (isSelected) {
		ofDisableLighting();
		ofSetColor(ofColor::yellow);
		ofPushMatrix();
			ofMultMatrix(Transform);
			ofDrawAxis(width * 1.5);
			plane.drawWireframe();
		ofPopMatrix();
		ofEnableLighting();
	}
	ofSetColor(ofColor::white);
	material.begin();
	material.setDiffuseColor(diffuseColor);
	ofPushMatrix();
		ofMultMatrix(Transform);
		plane.drawFaces();
	ofPopMatrix();
	material.end();
}

float Plane::sdf(const glm::vec3 p) {
	return p.y - position.y;
}

glm::vec3 Plane::getNormal(const glm::vec3 &p) {
	return this->normal;
}