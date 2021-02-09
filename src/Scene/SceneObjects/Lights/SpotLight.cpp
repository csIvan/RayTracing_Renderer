#include "SpotLight.h"

SpotLight::SpotLight(glm::vec3 p, glm::vec3 d, float total, float falloff, string name) {
	position = p;
	direction = d;
	totalWidth = total;
	falloffStart = falloff;
	objName = name;
}

void SpotLight::draw() {
	applyMatrix();
	if (isSelected) {
		ofSetColor(ofColor::yellow);
		ofNoFill();
		ofPushMatrix();
			ofMultMatrix(Transform);
			ofDrawAxis(radius * 1.5);;
			ofDrawSphere(ofVec3f::zero(), radius);
			ofTranslate(glm::vec3(0, -heightRef/2, 0));
			ofRotateZ(180);
			ofDrawCone(heightRef * tan(totalWidth / 180.0 * PI), heightRef);
		ofPopMatrix();
		ofFill();
	}
	ofSetColor(ofColor::cyan);
	ofPushMatrix();
		ofMultMatrix(Transform);
		ofDrawSphere(ofVec3f::zero(), radius);
	ofPopMatrix();
}

// Calculate the light depending on the constraint angle and falloff angle
float SpotLight::falloff(float spotAngle) {
	float cosTotalWidth = glm::cos(glm::radians(totalWidth));
	float cosfalloffStart = glm::cos(glm::radians(falloffStart));
	if (spotAngle < cosTotalWidth) {
		return 0;
	}
	if (spotAngle > cosfalloffStart) {
		return 1;
	}

	float delta = (spotAngle - cosTotalWidth) / (cosfalloffStart - cosTotalWidth);
	return delta;
}