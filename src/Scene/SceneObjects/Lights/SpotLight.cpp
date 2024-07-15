#include "SpotLight.h"

//--------------------------------------------------------------
SpotLight::SpotLight(const glm::vec3 &position, const glm::vec3 &direction, float intensity, float angle, float foAngle, const string &name) 
					: Light(position, intensity, name) {
	this->direction = direction;
	totalWidth = angle;
	falloffStart = foAngle;
}

//--------------------------------------------------------------
void SpotLight::draw() {
	applyMatrix();

	// check if object is selected
	if (isSelected) {
		glLineWidth(0.1f);

		// Draw selection outline
		ofSetColor(SELECTED_COLOR);
		ofPushMatrix();
		ofMultMatrix(Transform);
		ofDrawAxis(radius * 1.5f);
		ofDrawSphere(ofVec3f::zero(), radius);
		ofTranslate(glm::vec3(0, -heightRef / 2, 0));
		ofRotateZ(180);
		ofDrawCone(heightRef * tan(totalWidth / 180.0 * PI), heightRef);
		ofPopMatrix();

		glLineWidth(1.0f);
	}

	ofSetColor(ofColor::cyan);
	ofPushMatrix();
	ofMultMatrix(Transform);
	ofDrawSphere(ZERO_VECTOR, radius);
	ofPopMatrix();
}

//--------------------------------------------------------------
float SpotLight::falloff(float spotAngle) {
	float cosTotalWidth = cos(ofDegToRad(totalWidth));
	float cosfalloffStart = cos(ofDegToRad(falloffStart));
	if (spotAngle <= cosTotalWidth) {
		return 0;
	}
	if (spotAngle >= cosfalloffStart) {
		return 1;
	}

	float delta = (spotAngle - cosTotalWidth) / (cosfalloffStart - cosTotalWidth);
	return delta;
}