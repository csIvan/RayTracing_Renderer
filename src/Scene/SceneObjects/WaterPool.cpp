#include "WaterPool.h"

WaterPool::WaterPool(glm::vec3 p, float r, string name, ofColor diffuse) {
	position = p;
	radius = r;
	objName = name;
	objMaterial.diffuseColor = diffuse;
}

bool WaterPool::intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal, ofColor &surfaceColor) {
	// Proxy plane intersect for waterpool
	glm::vec3 rdd, roo;
	glm::vec4 p = glm::inverse(Transform) * glm::vec4(ray.p.x, ray.p.y, ray.p.z, 1.0);
	glm::vec4 p1 = glm::inverse(Transform) * glm::vec4(ray.p + ray.d, 1.0);
	roo = glm::vec4(p.x, p.y, p.z, 1.0);
	rdd = glm::normalize(p1 - p);

	float dist;
	bool insidePlane = false;
	bool hit = glm::intersectRayPlane(roo, rdd, glm::vec3(0, 0, 0), this->normal, dist);
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

void WaterPool::draw() {
	// Proxy plane draw for waterpool
	applyMatrix();
	plane.setPosition(glm::vec3(0, 0, 0));
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
	sceneMaterial.begin();
	sceneMaterial.setDiffuseColor(objMaterial.diffuseColor);
		ofPushMatrix();
		ofMultMatrix(Transform);
		plane.drawFaces();
	ofPopMatrix();
	sceneMaterial.end();
}

float WaterPool::sdf(glm::vec3 p, int amp, int octaves) {
	float total = 0;
	float freq = 0.06f;
	float ampl = 1;
	float maxVal = 0;
	float noise = 0;
	float pers = 0.5;
	for (int i = 0; i < 4; i++) {
		total += (ampl / 2) * glm::perlin(freq * p);
		maxVal += ampl;
		ampl *= pers;
		freq *= 2;
	}
	//noise = total / maxVal;
	noise = total;
	return p.y - (position.y + noise);
}