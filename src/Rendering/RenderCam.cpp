#include "RenderCam.h"

//--------------------------------------------------------------
RenderCam::RenderCam(const glm::vec2 &viewMin, const glm::vec2 &viewMax, const glm::vec3 &viewPos, const glm::vec3 &pos) 
	: view(viewMin, viewMax, viewPos) {
	position = pos;
	applyMatrix();
}

//--------------------------------------------------------------
// Similar intersect function to that of the Cube class
bool RenderCam::intersect(const Ray &ray, HitInfo &hitInfo) {
	// Apply Transformation
	glm::vec3 rayOriginLocal = glm::inverse(Transform) * glm::vec4(ray.p, 1.0f);
	glm::vec3 rayDirLocal = glm::inverse(Transform) * glm::vec4(ray.d, 0.0f);

	// Calculate intersection
	glm::vec3 invDir = 1.0f / rayDirLocal;
	glm::vec3 sign = glm::sign(-rayDirLocal);
	glm::vec3 offset = sign * glm::vec3(side, side, side);

	glm::vec3 tMinV = glm::vec3(-rayOriginLocal + offset) * invDir;
	glm::vec3 tMaxV = glm::vec3(-rayOriginLocal - offset) * invDir;
	glm::vec3 tMin = glm::min(tMinV, tMaxV);
	glm::vec3 tMax = glm::max(tMinV, tMaxV);

	// Calculate entering and exiting points
	float tNear = glm::max(glm::max(tMin.x, tMin.y), tMin.z);
	float tFar = glm::min(glm::min(tMax.x, tMax.y), tMax.z);

	// Cube distance
	glm::vec3 distToSides = glm::abs(rayOriginLocal) - side;
	float distToCube = glm::min(glm::max(distToSides.x, glm::max(distToSides.y, distToSides.z)), 0.0f) + glm::length(glm::max(distToSides, 0.0f));

	// No intersection
	if (tNear > tFar || tFar < 0.0 || isnan(tFar) || tNear < distToCube) {
		return false;
	}

	hitInfo.point = ray.evalPoint(tNear);
	return true;
}

//--------------------------------------------------------------
void RenderCam::draw() {
	applyMatrix();

	// check if object is selected
	if (isSelected) {
		ofDisableLighting();

		// Draw selection outline
		ofSetColor(SELECTED_COLOR);
		ofPushMatrix();
		ofMultMatrix(Transform);
		ofDrawAxis(side);
		drawFrustum();
		ofPopMatrix();

		ofEnableLighting();
	}

	ofPushMatrix();
	ofMultMatrix(Transform);
	ofSetColor(ofColor::orange);
	drawFrustum();
	ofPopMatrix();

}

//--------------------------------------------------------------
void RenderCam::drawFrustum() {
	ofDrawLine(glm::vec3(0, 0, 0), glm::vec3(view.bottomLeft().x, view.bottomLeft().y, view.getPosition().z));
	ofDrawLine(glm::vec3(0, 0, 0), glm::vec3(view.topLeft().x, view.topLeft().y, view.getPosition().z));
	ofDrawLine(glm::vec3(0, 0, 0), glm::vec3(view.topRight().x, view.topRight().y, view.getPosition().z));
	ofDrawLine(glm::vec3(0, 0, 0), glm::vec3(view.bottomRight().x, view.bottomRight().y, view.getPosition().z));
	ofDrawBox(glm::vec3(0, 0, 0), side * 2);
	view.draw();
}

//--------------------------------------------------------------
Ray RenderCam::getRay(float u, float v) const {
	glm::vec3 pointOnPlane = view.toWorld(u, v);

	glm::vec3 rayOriginLocal = ZERO_VECTOR;
	glm::vec3 rayDirLocal = glm::normalize(pointOnPlane - rayOriginLocal);

	glm::vec3 rayOrigin = Transform * glm::vec4(rayOriginLocal, 1.0f);
	glm::vec3 rayDir = Transform * glm::vec4(rayDirLocal, 0.0f);

	return Ray(rayOrigin, glm::normalize(rayDir));
}