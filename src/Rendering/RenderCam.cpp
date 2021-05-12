#include "RenderCam.h"

RenderCam::RenderCam() {
	position = glm::vec3(0, 0, 12);
}

bool RenderCam::intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal, glm::vec2 &uv) {
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

	return true;
}

void RenderCam::draw() {
	applyMatrix();
	if (isSelected) {
		ofSetColor(ofColor::yellow);
		ofNoFill();
		ofPushMatrix();
			ofMultMatrix(Transform);
			ofDrawAxis(side);
			drawFrustum();
		ofPopMatrix();
		ofFill();
	}

	ofPushMatrix();
		ofMultMatrix(Transform);
		ofSetColor(ofColor::orange);
		drawFrustum();
	ofPopMatrix();

}

void RenderCam::drawFrustum() {
	ofDrawLine(glm::vec3(0,0,0), glm::vec3(view.bottomLeft().x, view.bottomLeft().y, view.position.z));
	ofDrawLine(glm::vec3(0, 0, 0), glm::vec3(view.topLeft().x, view.topLeft().y, view.position.z));
	ofDrawLine(glm::vec3(0, 0, 0), glm::vec3(view.topRight().x, view.topRight().y, view.position.z));
	ofDrawLine(glm::vec3(0, 0, 0), glm::vec3(view.bottomRight().x, view.bottomRight().y, view.position.z));
	ofNoFill();
	ofDrawBox(glm::vec3(0, 0, 0), side * 2);
	ofFill();
	view.draw();
	ofSetColor(ofColor::white);
}


Ray RenderCam::getRay(float u, float v) {
	glm::vec3 pointOnPlane = view.toWorld(u, v);

	glm::vec3 roo, rdd;
	Ray tempR = Ray(glm::vec3(0, 0, 0), glm::normalize(pointOnPlane - glm::vec3(0, 0, 0)));

	glm::vec4 p = Transform * glm::vec4(tempR.p.x, tempR.p.y, tempR.p.z, 1.0);
	glm::vec4 p1 = Transform * glm::vec4(tempR.p + tempR.d, 1.0);
	roo = glm::vec4(p.x, p.y, p.z, 1.0);
	rdd = glm::normalize(p1 - p);

	return (Ray(roo, rdd));
}
