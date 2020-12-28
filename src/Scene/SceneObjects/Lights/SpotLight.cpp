#include "SpotLight.h"

SpotLight::SpotLight(glm::vec3 d, float angle, float foAngle) {
	direction = d;
	lightAngle = glm::radians(angle);
	falloffAngle = glm::radians(foAngle);
}


SpotLight::SpotLight() {
	direction = glm::vec3(0, -1, 0);
	lightAngle = glm::radians(10.0f);
	falloffAngle = lightAngle - 0.1;
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