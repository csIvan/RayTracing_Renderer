#include "AreaLight.h"

//--------------------------------------------------------------
AreaLight::AreaLight(const glm::vec3 &position, const glm::vec3 &direction, float intensity, float height, float width, const string &name) 
					: Light(position, intensity, name) {
	this->direction = direction;
	this->height = height;
	this->width = width;
	plane = new Plane(position, glm::vec3(0, 1, 0), objName, width, height);
	corner = glm::vec3(-width / 2, 0, height / 2);
	uSteps = 8;
	vSteps = 8;
	uVec = glm::vec3(width / uSteps, 0, 0);
	vVec = glm::vec3(0, 0, -height / vSteps);
	samples = uSteps * vSteps;
	applyMatrix();
}
//--------------------------------------------------------------
void AreaLight::draw() {
	applyMatrix();
	plane->setSelected(isSelected);
	plane->setPosition(position);
	plane->setRotation(rotation);
	plane->draw();
	ofNoFill();

}

//--------------------------------------------------------------
bool AreaLight::intersect(const Ray &ray, HitInfo &hitInfo) {
	return plane->intersect(ray, hitInfo);
}


//--------------------------------------------------------------
glm::vec3 AreaLight::pointOnLight(int u, int v) {
	glm::vec3 pos = corner + uVec * (u + (static_cast<float>(rand()) / RAND_MAX)) + vVec * (v + (static_cast<float>(rand()) / RAND_MAX));
	return glm::vec3(Transform * glm::vec4(pos, 1.0f));
}