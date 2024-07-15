#include "Cube.h"

//--------------------------------------------------------------
Cube::Cube(const glm::vec3 &position, float side, const string &name, const ofColor &diffuse) {
	this->position = position;
	this->side = side;
	objName = name;
	objMaterial.setDiffuse(diffuse);
	setBounds();
}

//--------------------------------------------------------------
void Cube::setBounds() {
	applyMatrix();
	glm::vec3 min = glm::vec3(-side, -side, side);
	glm::vec3 max = glm::vec3(side, side, -side);
	box.setParameters(min, max);
	box.transformBox(Transform);
}


//--------------------------------------------------------------
void Cube::draw() {
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
		ofDrawAxis(side * 1.5f);
		ofDrawBox(ZERO_VECTOR, side * 2);
		ofPopMatrix();
		ofFill();

		glLineWidth(1.0f);
		ofEnableLighting();
	}

	// Draw 3D solid cube
	ofSetColor(ofColor::white);
	sceneMaterial.begin();
	sceneMaterial.setDiffuseColor(objMaterial.getDiffuse());
	ofPushMatrix();
	ofMultMatrix(Transform);
	ofDrawBox(ZERO_VECTOR, side * 2);
	ofPopMatrix();
	sceneMaterial.end();
}


//--------------------------------------------------------------
bool Cube::intersect(const Ray &ray, HitInfo &hitInfo) {
	// Apply Transformation
	glm::vec3 rayOriginLocal = glm::inverse(Transform) * glm::vec4(ray.p, 1.0f);
	glm::vec3 rayDirLocal = glm::normalize(glm::inverse(Transform) * glm::vec4(ray.d, 0.0f));
	Ray localRay = Ray(rayOriginLocal, rayDirLocal);

	// Calculate intersection
	glm::vec3 sign = glm::vec3((rayDirLocal.x < 0.0) ? 1.0 : -1.0, (rayDirLocal.y < 0.0) ? 1.0 : -1.0, (rayDirLocal.z < 0.0) ? 1.0 : -1.0);
	glm::vec3 t = sign * glm::vec3(side);
	glm::vec3 tMin = glm::vec3(-rayOriginLocal + t) * localRay.inv_dir;
	glm::vec3 tMax = glm::vec3(-rayOriginLocal - t) * localRay.inv_dir;
	float tEntry = glm::compMax(tMin);
	float tExit = glm::compMin(tMax);

	// Cube distance
	glm::vec3 dist = abs(rayOriginLocal) - side;
	float cubeDist = glm::min(glm::compMax(dist), 0.0f) + glm::length(glm::max(dist, 0.0f));

	// No intersection
	if (tEntry > tExit || tExit < 0.0 || isnan(tExit) || tEntry < cubeDist)
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
// Cube Signed Distance Function modified from Inigo Quilez's version 
// Source: https://www.iquilezles.org/www/articles/distfunctions/distfunctions.htm
float Cube::sdf(const glm::vec3 &point) {
	glm::vec3 pointLocal = glm::inverse(Transform) * glm::vec4(point, 1.0f);
	glm::vec3 dist = abs(pointLocal) - side;
	float distance = glm::min(glm::compMax(dist), 0.0f) + glm::length(glm::max(dist, 0.0f));
	return distance;
}


//--------------------------------------------------------------
glm::vec2 Cube::getUV(const glm::vec3 &point) const {
	glm::vec3 localPoint = glm::vec3(getTranslateMatrix() * glm::vec4(point, 1.0));
	glm::vec3 hitPoint = localPoint - position;
	float u = 0.0f;
	float v = 0.0f;
	float eps = 0.001f;

	if (abs((hitPoint.x)) >= (side - eps) && abs((hitPoint.x)) <= (side + eps)) {
		u = (hitPoint.z + side) / (side * 2.0f);
		v = (hitPoint.y + side) / (side * 2.0f);
	}
	else if (abs((hitPoint.y)) >= (side - eps) && abs((hitPoint.y)) <= (side + eps)) {
		u = (hitPoint.x + side) / (side * 2.0f);
		v = (hitPoint.z + side) / (side * 2.0f);
	}
	else if (abs((hitPoint.z)) >= (side - eps) && abs((hitPoint.z)) <= (side + eps)) {
		u = (hitPoint.x + side) / (side * 2.0f);
		v = (hitPoint.y + side) / (side * 2.0f);
	}
	else {	
		return glm::vec2(u, v);	// return if point is not on the surface of cube
	}

	v = 1.0f - v;	// Flip V coordinate

	return glm::vec2(glm::abs(u), glm::abs(v));
}