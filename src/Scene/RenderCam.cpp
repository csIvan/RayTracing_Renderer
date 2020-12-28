#include "RenderCam.h"

RenderCam::RenderCam() {
	position = glm::vec3(0, 0, 12);
	aim = glm::vec3(0, 0, -1);
}

Ray RenderCam::getRay(float u, float v) {
	glm::vec3 pointOnPlane = view.toWorld(u, v);
	return (Ray(position, glm::normalize(pointOnPlane - position)));
}

void RenderCam::draw() {
	ofDrawBox(position, 1.0);
}

void RenderCam::drawFrustum() {
	// Currently draws proxy frustum
	view.draw();
}