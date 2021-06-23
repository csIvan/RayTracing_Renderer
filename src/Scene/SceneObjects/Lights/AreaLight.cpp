#include "AreaLight.h"

AreaLight::AreaLight(glm::vec3 p, glm::vec3 d, float h, float w, string name) {
	position = p;
	direction = d;
	height = h;
	width = w;
	objName = name;
	corner = glm::vec3(-width/2, 0, height/2);
	uv = glm::vec3(width, 0, 0);
	vv = glm::vec3(0, 0, -height);	
	uvec = glm::vec3(width/8, 0, 0);
	vvec = glm::vec3(0, 0, -height/8);
	usteps = 8;
	vsteps = 8;
	samples = 64;
}


bool AreaLight::intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal, ofColor &surfaceColor) {
	glm::vec3 rdd, roo;
	glm::vec4 p = glm::inverse(Transform) * glm::vec4(ray.p.x, ray.p.y, ray.p.z, 1.0);
	glm::vec4 p1 = glm::inverse(Transform) * glm::vec4(ray.p + ray.d, 1.0);
	roo = glm::vec4(p.x, p.y, p.z, 1.0);
	rdd = glm::normalize(p1 - p);
	Ray r = Ray(roo, rdd);

	float dist1, dist2;
	bool insidePlane = false;
	bool hit = false;
	float d = glm::dot(rdd, direction);
	if (glm::abs(d) > -0.001f) {
		float t = glm::dot(-roo, direction) / d;
		point = r.evalPoint(t);
		normal = direction;
		if(t >= 0)
			hit = true;
	}

	point = Transform * glm::vec4(point.x, point.y, point.z, 1.0);
	normal = glm::normalize(getRotateMatrix() * glm::vec4(normal.x, normal.y, normal.z, 1.0));
	glm::vec2 xRange = glm::vec2(position.x - width / 2, position.x + width / 2);
	glm::vec2 zRange = glm::vec2(position.z - height / 2, position.z + height / 2);
	if (hit && point.x < xRange[1] && point.x > xRange[0] && point.z < zRange[1] && point.z > zRange[0]) {
		insidePlane = true;
	}
	return insidePlane;
}

void AreaLight::draw() {
	applyMatrix();
	for (int i = 0; i < lightPositions.size(); i++) {
		ofSetColor(ofColor::red);
		ofDrawSphere(lightPositions[i], 0.025);
		ofSetColor(ofColor::yellow);
		//ofDrawLine(points[i], normals[i]);
	}
	plane.setPosition(glm::vec3(0, 0, 0));
	plane.setWidth(width);
	plane.setHeight(height);
	plane.setResolution(4, 4);
	plane.setOrientation(glm::vec3(270, 0, 0));

	if (isSelected) {
		ofSetColor(ofColor::yellow);
		ofPushMatrix();
			ofMultMatrix(Transform);
			ofDrawAxis(1);
			plane.drawWireframe();
		ofPopMatrix();
	}
	ofPushMatrix();
		ofMultMatrix(Transform);
		ofSetColor(ofColor(255, 179, 0));
		ofDrawArrow(glm::vec3(0, 0, 0), direction, 0.05);
		ofSetColor(ofColor::cyan);
		plane.drawFaces();
	ofPopMatrix();
}

glm::vec3 AreaLight::pointOnLight(int u, int v) {
	glm::vec3 pos = (corner + uvec * (u + ((float)rand() / (RAND_MAX))) + vvec * (v + ((float)rand() / (RAND_MAX))));
	glm::vec4 newPos = Transform * glm::vec4(pos.x, pos.y, pos.z, 1.0f);
	count++;
	return glm::vec3(newPos.x, newPos.y, newPos.z);
}