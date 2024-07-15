#include "Sphere.h"

//--------------------------------------------------------------
Sphere::Sphere(const glm::vec3 &position, float radius, const string &name, const ofColor &diffuse) {
	this->position = position;
	this->radius = radius;
	objName = name;
	objMaterial.setDiffuse(diffuse);
	setBounds();
}


//--------------------------------------------------------------
void Sphere::setBounds() {
	applyMatrix();
	glm::vec3 min = glm::vec3(-radius, -radius, radius);
	glm::vec3 max = glm::vec3(radius, radius, -radius);
	box.setParameters(min, max);
	box.transformBox(Transform);
}


//--------------------------------------------------------------
void Sphere::draw() {
	applyMatrix();

	// check if object is selected
	if (isSelected) {
		ofDisableLighting();
		glLineWidth(0.1f);

		// Draw selection outline
		ofSetColor(SELECTED_COLOR);
		ofNoFill();
		ofPushMatrix();
		ofMultMatrix(Transform);
		ofDrawAxis(radius * 1.5f);
		ofDrawSphere(ZERO_VECTOR, radius);
		ofPopMatrix();
		ofFill();

		glLineWidth(1.0f);
		ofEnableLighting();
	}

	// Draw 3D solid sphere
	ofSetColor(ofColor::white);
	sceneMaterial.begin();
	sceneMaterial.setDiffuseColor(objMaterial.getDiffuse());
	ofPushMatrix();
	ofMultMatrix(Transform);
	ofDrawSphere(ZERO_VECTOR, radius);
	ofPopMatrix();
	sceneMaterial.end();
}


//--------------------------------------------------------------
bool Sphere::intersect(const Ray &ray, HitInfo &hitInfo) {
	// Apply Transformation
	glm::vec3 rayOriginLocal = glm::inverse(Transform) * glm::vec4(ray.p, 1.0f);
	glm::vec3 rayDirLocal = glm::inverse(Transform) * glm::vec4(ray.d, 0.0f);

	glm::vec3 point, normal;
	hitInfo.hit = glm::intersectRaySphere(rayOriginLocal, glm::normalize(rayDirLocal), ZERO_VECTOR, radius, point, normal);

	hitInfo.surfaceColor = objTexture.getTextureColor(getUV(point), objMaterial.getDiffuse());
	hitInfo.point = Transform * glm::vec4(point, 1.0f);
	hitInfo.normal = glm::normalize(getRotateMatrix() * glm::vec4(normal, 0.0f));
	return hitInfo.hit;
}


//--------------------------------------------------------------
// Sphere Signed Distance Function modified from Inigo Quilez's version 
// Source: https://www.iquilezles.org/www/articles/distfunctions/distfunctions.htm
float Sphere::sdf(const glm::vec3 &point) {
	glm::vec3 pointLocal = glm::inverse(Transform) * glm::vec4(point, 1.0f);
	float distance = glm::distance(pointLocal, ZERO_VECTOR) - radius;
	return distance;
}


//--------------------------------------------------------------
glm::vec2 Sphere::getUV(const glm::vec3 &point) const {
	glm::vec3 hitPoint = getTranslateMatrix() * glm::vec4(point, 1.0f);
	glm::vec3 normal = glm::normalize(hitPoint - position);
	float u = 0.5f + (atan2(normal.x, normal.z) / (2 * PI));
	float v = 0.5f - (asin(normal.y) / PI);

	return glm::vec2(u, v);
}