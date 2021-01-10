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
	drawFrustum();
}

void RenderCam::drawFrustum() {
	ofSetColor(ofColor::orange);
	ofDrawLine(position, glm::vec3(view.bottomLeft().x, view.bottomLeft().y, view.position.z));
	ofDrawLine(position, glm::vec3(view.topLeft().x, view.topLeft().y, view.position.z));
	ofDrawLine(position, glm::vec3(view.topRight().x, view.topRight().y, view.position.z));
	ofDrawLine(position, glm::vec3(view.bottomRight().x, view.bottomRight().y, view.position.z));
	view.draw();
	ofSetColor(ofColor::white);
}