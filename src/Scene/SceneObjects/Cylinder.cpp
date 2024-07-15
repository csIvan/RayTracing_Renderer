#include "Cylinder.h"

//--------------------------------------------------------------
Cylinder::Cylinder(const glm::vec3 &position, float height, float radius, const string &name, const ofColor &diffuse) {
	this->position = position;
	this->height = height;
	this->radius = radius;
	objName = name;
	objMaterial.setDiffuse(diffuse);
	setBounds();
}

//--------------------------------------------------------------
void Cylinder::setBounds() {
	applyMatrix();
	glm::vec3 min = glm::vec3(-radius, -height/2, radius);
	glm::vec3 max = glm::vec3(radius, height/2, -radius);
	box.setParameters(min, max);
	box.transformBox(Transform);
}


//--------------------------------------------------------------
void Cylinder::draw() {
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
		ofDrawAxis(height * 1.5f);
		ofDrawCylinder(ZERO_VECTOR, radius, height);
		ofPopMatrix();
		ofFill();

		glLineWidth(1.0f);
		ofEnableLighting();
	}

	// Draw 3D solid cylinder
	ofSetColor(ofColor::white);
	sceneMaterial.begin();
	sceneMaterial.setDiffuseColor(objMaterial.getDiffuse());
	ofPushMatrix();
	ofMultMatrix(Transform);
	ofDrawCylinder(ZERO_VECTOR, radius, height);
	ofPopMatrix();
	sceneMaterial.end();
}


//--------------------------------------------------------------
// Cylinder Ray Intersection function modified from Inigo Quilez's version 
// Source: https://iquilezles.org/articles/intersectors/
bool Cylinder::intersect(const Ray &ray, HitInfo &hitInfo) {
	// Apply Transformation
	glm::vec3 rayOriginLocal = glm::inverse(Transform) * glm::vec4(ray.p, 1.0f);
	glm::vec3 rayDirLocal = glm::normalize(glm::inverse(Transform) * glm::vec4(ray.d, 0.0f));
	Ray localRay = Ray(rayOriginLocal, rayDirLocal);

	glm::vec3 capA = glm::vec3(0, -height / 2.0f, 0);
	glm::vec3 capB = glm::vec3(0, height / 2.0f, 0);
	float r2 = radius * radius;

	glm::vec3 ba = capB - capA;
	glm::vec3 oa = rayOriginLocal - capA;
	glm::vec3 ob = rayOriginLocal - capB;

	float m0 = glm::dot(ba, ba);
	float m1 = glm::dot(oa, ba);
	float m2 = glm::dot(ob, ba);
	float m3 = glm::dot(rayDirLocal, ba);
	float m4 = glm::dot(rayDirLocal, oa);
	float m5 = glm::dot(oa, oa);
	float t = FLT_MAX;

	// Caps
	if (m1 < 0.0f) {
		if (glm::dot(oa * m3 - rayDirLocal * m1, oa * m3 - rayDirLocal * m1) < (r2 * m3 * m3)) {
			t = -m1 / m3;
			if (t < EPSILON) {
				return false;
			}
			hitInfo.surfaceColor = objTexture.getTextureColor(getUV(localRay.evalPoint(t)), objMaterial.getDiffuse());
			hitInfo.point = Transform * glm::vec4(localRay.evalPoint(t), 1.0f);
			hitInfo.normal = glm::normalize(getRotateMatrix() * glm::vec4(-ba * glm::inversesqrt(m0), 1.0f));
			return true;
		}
	}
	else if (m2 > 0.0f) {
		if (glm::dot(oa * m3 - rayDirLocal * m2, ob * m3 - rayDirLocal * m2) < (r2 * m3 * m3)) {
			t = -m2 / m3;
			if (t < EPSILON) {
				return false;
			}
			hitInfo.surfaceColor = objTexture.getTextureColor(getUV(localRay.evalPoint(t)), objMaterial.getDiffuse());
			hitInfo.point = Transform * glm::vec4(localRay.evalPoint(t), 1.0f);
			hitInfo.normal = glm::normalize(getRotateMatrix() * glm::vec4(ba * glm::inversesqrt(m0), 1.0f));
			return true;
		}
	}

	// Body
	float k2 = m0 * m0 - m3 * m3 * m0;
	float k1 = m0 * m0 * m4 - m1 * m3 * m0;
	float k0 = m0 * m0 * m5 - m1 * m1 * m0 + m0 * radius * (-m0 * radius);

	float h = k1 * k1 - k2 * k0;
	if (h < 0.0f) {
		return false;
	}

	t = (-k1 - sqrt(h)) / k2;

	float y = m1 + t * m3;
	if (y > 0.0f && y < m0 && t >= EPSILON) {
		hitInfo.surfaceColor = objTexture.getTextureColor(getUV(localRay.evalPoint(t)), objMaterial.getDiffuse());
		hitInfo.point = Transform * glm::vec4(localRay.evalPoint(t), 1.0f);
		hitInfo.normal = glm::normalize(getRotateMatrix() * glm::vec4((m0 * (m0 * (oa + t * rayDirLocal)) - ba * m0 * y), 1.0f));
		return true;
	}

	return false;
}


//--------------------------------------------------------------
// Cylinder Signed Distance Function modified from Inigo Quilez's version 
// Source: https://www.iquilezles.org/www/articles/distfunctions/distfunctions.htm
float Cylinder::sdf(const glm::vec3 &point) {
	glm::vec3 localPoint = glm::inverse(Transform) * glm::vec4(point, 1.0f);

	glm::vec3 capTop = glm::vec3(0, height / 2.0f, 0);
	glm::vec3 capBottom = glm::vec3(0, -height / 2.0f, 0);

	float capLength = glm::dot(capBottom - capTop, capBottom - capTop);
	float pointLength = glm::dot(localPoint - capTop, localPoint - capTop);
	float projection = glm::dot(localPoint - capTop, capBottom - capTop) / capLength;

	float xDist = sqrt(pointLength - projection * projection * capLength);
	float yDist = glm::clamp(projection, 0.0f, 1.0f);

	float distToSide = max(0.0f, xDist - radius);
	float distToCaps = abs(projection - 0.5f) - 0.5f;

	float outerDistX = xDist - radius;
	float outerDistY = projection - yDist;

	float sign = (outerDistX < 0.0f && distToCaps < 0.0f) ? -1.0f : 1.0f;


	return (sign * sqrt(min(distToSide * distToSide + distToCaps * distToCaps * capLength,
							outerDistX * outerDistX + outerDistY * outerDistY * capLength)));
}


//--------------------------------------------------------------
glm::vec2 Cylinder::getUV(const glm::vec3 &point) const {
	glm::vec3 localPoint = getTranslateMatrix() * glm::vec4(point, 1.0f);
	glm::vec3 hitPoint = glm::normalize(localPoint - position);

	float u = 0.5f + (atan2(hitPoint.x, hitPoint.z) / (2 * PI));
	float v = (-point.y + height / 2) / height;

	return glm::vec2(u, v);
}