#include "Cone.h"
#define EPSILON 0.05

Cone::Cone(glm::vec3 p, float h, float r, string name, ofColor diffuse) {
	position = p;
	height = h;
	radius = r;
	objName = name;;
	objMaterial.diffuseColor = diffuse;
	box = new Box();
	applyMatrix();
	setBounds();
}

void Cone::setBounds() {
	glm::vec3 min = glm::vec3(-radius, -height / 2, radius);
	glm::vec3 max = glm::vec3(radius, height / 2, -radius);
	box->setParameters(min, max);
	box->transformBox(Transform);
}

bool Cone::intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal, ofColor &surfaceColor) {
	glm::vec3 rdd, roo;

	// Apply Transformation
	glm::vec4 p = glm::inverse(Transform) * glm::vec4(ray.p.x, ray.p.y, ray.p.z, 1.0);
	glm::vec4 p1 = glm::inverse(Transform) * glm::vec4(ray.p + ray.d, 1.0);
	roo = glm::vec4(p.x, p.y, p.z, 1.0);
	rdd = glm::normalize(p1 - p);

	Ray rTemp = Ray(roo, rdd);
	glm::vec3 capA = glm::vec3(0, -height / 2.0f, 0);
	glm::vec3 capB = glm::vec3(0, height / 2.0f, 0);

	glm::vec3 ba = capB - capA;
	glm::vec3 oa = roo - capA;
	glm::vec3 ob = roo - capB;

	float m0 = glm::dot(ba, ba);
	float m1 = glm::dot(oa, ba);
	float m2 = glm::dot(ob, ba);
	float m3 = glm::dot(rdd, ba);
	float t = FLT_MAX;

	// Caps
	if (m1 < 0.0) {
		if (glm::dot(oa * m3 - rdd * m1, oa * m3 - rdd * m1) < (radius * radius * m3 * m3)) {
			t = -m1 / m3;
			if (t < EPSILON) return false;
			surfaceColor = objTexture.getTextureColor(getUV(rTemp.evalPoint(t)), objMaterial.diffuseColor);
			point = Transform * glm::vec4(rTemp.evalPoint(t), 1.0);
			normal = glm::normalize(getRotateMatrix() * glm::vec4(-ba * glm::inversesqrt(m0), 1.0));
			return true;
		}
	}
	else if (m2 > 0.0) {
		if (glm::dot(ob * m3 - rdd * m2, ob * m3 - rdd * m2) < 0.0) {
			t = -m2 / m3;
			if (t < EPSILON) return false;
			surfaceColor = objTexture.getTextureColor(getUV(rTemp.evalPoint(t)), objMaterial.diffuseColor);
			point = Transform * glm::vec4(rTemp.evalPoint(t), 1.0);
			normal = glm::normalize(getRotateMatrix() * glm::vec4(ba * glm::inversesqrt(m0), 1.0));
			return true;
		}
	}

	// Body
	float m4 = glm::dot(rdd, oa);
	float m5 = glm::dot(oa, oa);
	float hy = m0 + radius * radius;

	float k2 = m0 * m0 - m3 * m3 * hy;
	float k1 = m0 * m0 * m4 - m1 * m3* hy + m0 * radius * (radius * m3 * 1.0);
	float k0 = m0 * m0 * m5 - m1 * m1 * hy + m0 * radius * (radius * m1 * 2.0 - m0 * radius);

	float h = k1 * k1 - k2 * k0;
	if (h < 0.0) return false;

	t = (-k1 - sqrt(h)) / k2;

	float y = m1 + t * m3;
	if (y > 0.0 && y < m0 && t >= EPSILON) {
		surfaceColor = objTexture.getTextureColor(getUV(rTemp.evalPoint(t)), objMaterial.diffuseColor);
		point = Transform * glm::vec4(rTemp.evalPoint(t), 1.0);
		normal = glm::normalize(getRotateMatrix() * 
			glm::vec4((m0 * (m0 * (oa + t * rdd) + radius * ba * radius) - ba * hy * y), 1.0));

		return true;
	}

	return false;
}

void Cone::draw() {
	applyMatrix();
	ofDisableLighting();
	for (int i = 0; i < points.size(); i++) {
		ofSetColor(ofColor::red);
		//ofDrawSphere(points[i], 0.025);
		ofSetColor(ofColor::yellow);
		//ofDrawLine(points[i], normals[i]);
	}
	

	ofEnableLighting();
	if (isSelected) {
		ofDisableLighting();
		ofSetColor(ofColor::yellow);
		ofNoFill();
		ofPushMatrix();
			ofMultMatrix(Transform);
			ofDrawAxis(height * 1.5);
			ofRotateZ(180);
			ofDrawCone(radius, height);
		ofPopMatrix();
		ofFill();
		ofEnableLighting();
	}

	ofSetColor(ofColor::white);
	sceneMaterial.begin();
	sceneMaterial.setDiffuseColor(objMaterial.diffuseColor);
	ofPushMatrix();
		ofMultMatrix(Transform);
		ofRotateZ(180);
		ofDrawCone(radius, height);
	ofPopMatrix();
	sceneMaterial.end();
}


// sdf modified from Inigo Quilez's version found in https://www.iquilezles.org/www/articles/distfunctions/distfunctions.htm
float Cone::sdf(const glm::vec3 p1) {
	glm::vec4 pp = glm::inverse(Transform) * glm::vec4(p1.x, p1.y, p1.z, 1.0);
	glm::vec3 p = glm::vec3(pp.x, pp.y, pp.z);

	glm::vec3 capB = glm::vec3(0, -height / 2.0f, 0);
	glm::vec3 capA = glm::vec3(0, height / 2.0f, 0);

	float ra = 0.0;
	float rb = radius;
	float rba = rb - ra;

	float m0 = glm::dot(capB - capA, capB - capA);
	float m1 = glm::dot(p - capA, p - capA);
	float m2 = glm::dot(p - capA, capB - capA) / m0;

	float x = sqrt(m1 - m2 * m2 * m0);
	float k = rba * rba + m0;
	float f = glm::clamp((rba * (x - ra) + m2 * m0) / k, 0.0f, 1.0f);

	float cax = max(0.0f, x - ((m1 < 0.5) ? ra : rb));
	float cay = abs(m2 - 0.5) - 0.5;

	float cbx = x - ra - f * rba;
	float cby = m2 - f;

	float s = (cbx < 0.0 && cay < 0.0) ? -1.0 : 1.0;

	return (s * sqrt(min(cax * cax + cay * cay * m0, cbx * cbx + cby * cby * m0)));
}

glm::vec2 Cone::getUV(glm::vec3 p) {
	glm::vec4 pp = getTranslateMatrix() * glm::vec4(p.x, p.y, p.z, 1.0);
	glm::vec3 hit = glm::vec4(pp.x, pp.y, pp.z, 1.0);

	glm::vec3 n = glm::normalize(hit - position);
	float u = 0.5f + (atan2(n.x, n.z) / (2 * PI));
	float v = (-p.y + height / 2) / (height);

	return  glm::vec2(u, v);
}