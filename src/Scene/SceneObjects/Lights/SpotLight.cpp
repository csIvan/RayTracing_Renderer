#include "SpotLight.h"

SpotLight::SpotLight(glm::vec3 p, glm::vec3 d, float angle, float foAngle, string name) {
	position = p;
	direction = d;
	lightAngle = glm::radians(angle);
	falloffAngle = glm::radians(foAngle);
	objName = name;
}

void SpotLight::draw() {
	ofSetColor(ofColor::cyan);
	ofDrawSphere(position, radius);
}

// Calculate the light depending on the constraint angle and falloff angle
float SpotLight::falloff(float spotAngle) {
	spotAngle = 1 - spotAngle;
	if (spotAngle < lightAngle) {
		return 1;
	}
	if (spotAngle > falloffAngle) {
		return 0;
	}

	float delta = (spotAngle - falloffAngle) / (lightAngle - falloffAngle);
	return delta;
}