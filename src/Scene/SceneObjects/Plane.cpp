#include "Plane.h"

//--------------------------------------------------------------
Plane::Plane(const glm::vec3 &position, const glm::vec3 &normal, const string &name, float width, float height, const ofColor &diffuse) {
	this->position = position;
	this->normal = normal;
	this->width = width;
	this->height = height;
	objName = name;
	objMaterial.setDiffuse(diffuse);
	setBounds();

}

//--------------------------------------------------------------
void Plane::setBounds() {
	applyMatrix();
	min = glm::vec4(-width / 2, 0.0f, height / 2, 1.0f);
	max = glm::vec4(width / 2, 0.0f, -height / 2, 1.0f);
	box.setParameters(min, max);
	box.transformBox(Transform);
}


//--------------------------------------------------------------
void Plane::draw() {
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
		ofDrawAxis(1.0f);
		plane.drawWireframe();
		ofPopMatrix();
		ofFill();

		glLineWidth(1.0f);
		ofEnableLighting();
	}

	// Draw 3D solid plane
	plane.setPosition(ZERO_VECTOR);
	plane.setWidth(width);
	plane.setHeight(height);
	plane.setResolution(4, 4);
	plane.setOrientation(glm::vec3(270, 0, 0));

	ofSetColor(ofColor::white);
	sceneMaterial.begin();
	sceneMaterial.setDiffuseColor(objMaterial.getDiffuse());
	ofPushMatrix();
	ofMultMatrix(Transform);
	plane.drawFaces();
	ofPopMatrix();
	sceneMaterial.end();
}


//--------------------------------------------------------------
bool Plane::intersect(const Ray &ray, HitInfo &hitInfo) {
	// Apply Transformation
	glm::vec3 rayOriginLocal = glm::inverse(Transform) * glm::vec4(ray.p, 1.0f);
	glm::vec3 rayDirLocal = glm::normalize(glm::inverse(Transform) * glm::vec4(ray.d, 0.0f));
	Ray localRay = Ray(rayOriginLocal, rayDirLocal);

	// Calculate intersection
	glm::vec3 sign = glm::vec3((rayDirLocal.x < 0.0) ? 1.0 : -1.0, (rayDirLocal.y < 0.0) ? 1.0 : -1.0, (rayDirLocal.z < 0.0) ? 1.0 : -1.0);
	glm::vec3 t = sign * glm::vec3(glm::abs(max.x - min.x) / 2, glm::abs(max.y - min.y) / 2, glm::abs(max.z - min.z) / 2);
	glm::vec3 tMin = glm::vec3(-rayOriginLocal + t) * localRay.inv_dir;
	glm::vec3 tMax = glm::vec3(-rayOriginLocal - t) * localRay.inv_dir;
	float tEntry = glm::compMax(tMin);
	float tExit = glm::compMin(tMax);

	// Plane distance
	glm::vec3 dist = abs(rayOriginLocal) - glm::vec3(glm::abs(max.x - min.x) / 2, glm::abs(max.y - min.y) / 2, glm::abs(max.z - min.z) / 2);
	float planeDist = glm::min(glm::compMax(dist), 0.0f) + glm::length(glm::max(dist, 0.0f));

	// No intersection
	if (tEntry > tExit || tExit < 0.0 || isnan(tExit) || tEntry < planeDist)
		return false;

	// Point
	hitInfo.hit = true;
	hitInfo.point = localRay.evalPoint(tEntry);
	hitInfo.surfaceColor = objTexture.getTextureColor(getUV(hitInfo.point), objMaterial.getDiffuse());
	hitInfo.point = Transform * glm::vec4(hitInfo.point, 1.0);

	// Normal
	if (tEntry == tMin.x)
		hitInfo.normal = glm::vec3(Transform[0]) * sign.x;
	else if (tEntry == tMin.y)
		hitInfo.normal = glm::vec3(Transform[1]) * sign.y;
	else
		hitInfo.normal = glm::vec3(Transform[2]) * sign.z;


	return hitInfo.hit;
}


//--------------------------------------------------------------
// Infinite Plane Signed Distance Function modified from Inigo Quilez's version 
// Source: https://www.iquilezles.org/www/articles/distfunctions/distfunctions.htm
float Plane::sdf(const glm::vec3 &point) {
	glm::vec3 pointLocal = glm::inverse(Transform) * glm::vec4(point, 1.0f);
	return pointLocal.y;
}


//--------------------------------------------------------------
glm::vec2 Plane::getUV(const glm::vec3 &point) const {
	glm::vec3 hitPoint = glm::vec3(getTranslateMatrix() * glm::vec4(point, 1.0));
	float u = (hitPoint.x + width * 0.5f) / width;
	float v = (hitPoint.z + height * 0.5f) / height;
	return glm::vec2(glm::abs(u), glm::abs(v));
}