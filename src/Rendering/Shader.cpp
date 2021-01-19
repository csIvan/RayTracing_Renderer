#include "Shader.h"

Shader::Shader(vector<Light *> lights, vector<SceneObject *> objects) {
	this->lights = lights;
	this->objects = objects;
}

ofColor Shader::lambert(const glm::vec3 &point, const glm::vec3 &normal, const ofColor diffuse) {
	float ambientCo = 0.08;
	ofColor lambertColor = diffuse * ambientCo;
	for (int i = 0; i < lights.size(); i++) {
		glm::vec3 l = glm::normalize(lights[i]->position - point);
		glm::vec3 lightToPixel = glm::normalize(point - lights[i]->position);
		//float SpotFactor = glm::dot(lightToPixel, lights[i]->direction);
		float distance = glm::distance(lights[i]->position, point);
		Ray shadRay = Ray(glm::vec3(point.x, point.y, point.z + .001f), l);

		//call to calculate the falloff factor for the spotlight
		//float falloff = lights[i]->falloff(SpotFactor);

		ofColor lambertCalculation = diffuse * (lights[i]->intensity / glm::pow(distance, 2)) * glm::max(0.0f, glm::dot(normal, l));

		if (!inShadow(shadRay)) {
			lambertColor += lambertCalculation;
			//lambertColor += lambertCalculation * falloff;
		}

	}
	return lambertColor;
}

ofColor Shader::phong(const glm::vec3 &point, const glm::vec3 &normal, const glm::vec3 camPos, const ofColor diffuse, const ofColor specular, float power) {
	float ambientCo = 0.05;
	ofColor phongColor = diffuse * ambientCo;
	for (int i = 0; i < lights.size(); i++) {
		glm::vec3 l = glm::normalize(lights[i]->position - point);
		glm::vec3 lightToPixel = glm::normalize(point - lights[i]->position);
		//float SpotFactor = glm::dot(lightToPixel, lights[i]->direction);
		float distance = glm::distance(lights[i]->position, point);
		glm::vec3 v = -glm::normalize(point - camPos);
		glm::vec3 h = glm::normalize(v + l);
		Ray shadRay = Ray(glm::vec3(point.x, point.y, point.z + 0.001f), l);;
		//if (normal == glm::vec3(0, 1, 0))
			//shadRay = Ray(glm::vec3(point.x, point.y + .01f, point.z), l);


		//call to calculate the falloff factor for the spotlight
		//float falloff = lights[i]->falloff(SpotFactor);

		ofColor phongCalculation = diffuse * (lights[i]->intensity / glm::pow(distance, 2)) * glm::max(0.0f, glm::dot(normal, l)) +
			specular * (lights[i]->intensity / glm::pow(distance, 2)) * (glm::pow(glm::max(0.0f, glm::dot(normal, h)), power));

		//if (!inShadow(shadRay)) {
			phongColor += phongCalculation;
			//phongColor += phongCalculation * falloff;
		//}
	}
	return phongColor;
}

bool Shader::inShadow(const Ray &r) {
	bool blocked = false;
	for (int index = 0; index < objects.size(); index++) {
		glm::vec3 point;
		glm::vec3 normal;

		//calculate shadows using rayMarch()
		//if (rayMarch(r, point)) {
		//	blocked = true;
		//}

		//if (objects[index]->intersect(r, point, normal)) {
		//	blocked = true;
		//}
	}
	return blocked;
}
