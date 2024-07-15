#include "Torus.h"

//--------------------------------------------------------------
Torus::Torus(const glm::vec3 &position, float radius, float majorRadius , const string &name, const ofColor &diffuse) {
	this->position = position;
	this->radius = radius;
	this->majorRadius = majorRadius;
	objName = name;
	objMaterial.setDiffuse(diffuse);
	setBounds();
}

//--------------------------------------------------------------
void Torus::setBounds() {
	applyMatrix();
	glm::vec3 min = glm::vec3(-(majorRadius + radius), -(majorRadius + radius), radius);
	glm::vec3 max = glm::vec3((majorRadius + radius), (majorRadius + radius), -radius);
	box.setParameters(min, max);
	box.transformBox(Transform);
}


//--------------------------------------------------------------
void Torus::draw() {
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
		ofDrawAxis(majorRadius * 1.5f);
		drawTorus();
		ofPopMatrix();
		ofFill();

		glLineWidth(1.0f);
		ofEnableLighting();
	}

	// Draw 3D solid torus
	ofSetColor(ofColor::white);
	sceneMaterial.begin();
	sceneMaterial.setDiffuseColor(objMaterial.getDiffuse());
	ofPushMatrix();
	ofMultMatrix(Transform);
	drawTorus();
	ofPopMatrix();
	sceneMaterial.end();
}


//--------------------------------------------------------------
void Torus::drawTorus() {
	float s, x, y, z;
	for (int i = 0; i < numc; i++) {
		glBegin(GL_QUAD_STRIP);
		for (int j = 0; j <= numt; j++) {
			for (int k = 1; k >= 0; k--) {
				s = (i + k) % numc;
				x = (majorRadius + radius * cos(s * 2 * PI / numc)) * cos(j * 2 * PI / numt);
				y = (majorRadius + radius * cos(s * 2 * PI / numc)) * sin(j * 2 * PI / numt);
				z = radius * sin(s * 2 * PI / numc);
				glVertex3f(x, y, z);

			}
		}
		glEnd();
	}
}

//--------------------------------------------------------------
// Cone Ray Intersection function modified from Inigo Quilez's version 
// Source: https://iquilezles.org/articles/intersectors/
bool Torus::intersect(const Ray &ray, HitInfo &hitInfo) {
	// Apply Transformation
	glm::vec3 rayOriginLocal = glm::inverse(Transform) * glm::vec4(ray.p, 1.0f);
	glm::vec3 rayDirLocal = glm::normalize(glm::inverse(Transform) * glm::vec4(ray.d, 0.0f));
	Ray localRay = Ray(rayOriginLocal, rayDirLocal);

	float m = dot(rayDirLocal, rayDirLocal);
	float n = dot(rayOriginLocal, rayDirLocal);
	float e = dot(rayOriginLocal, rayOriginLocal) - (majorRadius * majorRadius + radius * radius);
	float R = 4.0f * majorRadius * majorRadius;

	// Quartic variables
	float c4 = m * m;
	float c3 = 4.0f * m * n;
	float c2 = 2.0f * m * e + 4.0f * n * n + R * rayDirLocal.z * rayDirLocal.z;
	float c1 = 4.0f * n * e + 2.0f * R * rayOriginLocal.z * rayDirLocal.z;
	float c0 = e * e - R * (radius * radius - rayOriginLocal.z * rayOriginLocal.z);

	c4 /= c4;
	c3 /= c4;
	c2 /= c4;
	c1 /= c4;
	c0 /= c4;

	// Calculate Roots
	double roots[4];
	int realRoots = SolveP4(roots, c3, c2, c1, c0);

	if (realRoots == 0) {
		return false;
	}

	float root = FLT_MAX;
	for (int i = 0; i < realRoots; i++) {
		if (root > static_cast<float>(roots[i])) {
			root = static_cast<float>(roots[i]);
		}
	}

	if (root < EPSILON) {
		return false;
	}

	hitInfo.hit = true;
	hitInfo.point = localRay.evalPoint(root);
	hitInfo.surfaceColor = objTexture.getTextureColor(getUV(hitInfo.point), objMaterial.getDiffuse());
	hitInfo.normal = hitInfo.point * (glm::dot(hitInfo.point, hitInfo.point) - radius * radius - majorRadius * majorRadius * glm::vec3(1.0f, 1.0f, -1.0f));
	hitInfo.normal = glm::normalize(getRotateMatrix() * glm::vec4(hitInfo.normal, 1.0f));
	hitInfo.point = Transform * glm::vec4(hitInfo.point, 1.0f);

	return hitInfo.hit;
}


//--------------------------------------------------------------
// Torus Signed Distance Function modified from Inigo Quilez's version 
// Source: https://www.iquilezles.org/www/articles/distfunctions/distfunctions.htm
float Torus::sdf(const glm::vec3 &point) {
	glm::vec3 pointLocal = glm::inverse(Transform) * glm::vec4(point, 1.0f);
	glm::vec2 dist = glm::vec2(glm::length(glm::vec2(pointLocal.x, pointLocal.y)) - majorRadius, pointLocal.z);
	return glm::length(dist) - radius;
}


//--------------------------------------------------------------
glm::vec2 Torus::getUV(const glm::vec3 &point) const {
	glm::vec3 localPoint = glm::vec3(getTranslateMatrix() * glm::vec4(point, 1.0));
	glm::vec3 hitPoint = localPoint - position;
	
	float u = 1.0f - (atan2(hitPoint.x, hitPoint.y) / (2 * PI));
	float v = 0.5f + atan2(-hitPoint.z, glm::length(glm::vec2(hitPoint.x, hitPoint.y)) - majorRadius) / (2 * PI);

	return glm::vec2(glm::abs(u), glm::abs(v));
}