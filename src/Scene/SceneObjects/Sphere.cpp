#include "Sphere.h"

Sphere::Sphere(glm::vec3 p, float r, string name, ofColor diffuse) {
	position = p;
	radius = r;
	objName = name;
	objMaterial.diffuseColor = diffuse;
}

bool Sphere::intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal, glm::vec2 &uv) {
	glm::vec3 rdd, roo;

	// Apply Transformation
	glm::vec4 p = glm::inverse(Transform) * glm::vec4(ray.p.x, ray.p.y, ray.p.z, 1.0);
	glm::vec4 p1 = glm::inverse(Transform) * glm::vec4(ray.p + ray.d, 1.0);
	roo = glm::vec4(p.x, p.y, p.z, 1.0);
	rdd = glm::normalize(p1 - p);

	bool hit = (glm::intersectRaySphere(roo, rdd, glm::vec3(0, 0, 0), radius, point, normal));

	point = Transform * glm::vec4(point.x, point.y, point.z, 1.0);
	normal = glm::normalize(getRotateMatrix() * glm::vec4(normal.x, normal.y, normal.z, 1.0));
	uv = getUV(point);
	return hit;
}

void Sphere::draw() {
	applyMatrix();
	ofDisableLighting();
	//for (int i = 0; i < points.size(); i++) {
	//	//ofSetColor(ofColor::red);
	//	//ofDrawSphere(points[i], 0.025);		
	//	ofSetColor(ofColor::blue);
	//	ofDrawSphere(points2[i], 0.025);
	//	ofSetColor(ofColor::yellow);
	//	ofDrawLine(points[i], normals[i]);
	//	//cout << points2.size() << points2[i] << endl;
	//}


	ofEnableLighting();
	if (isSelected) {
		ofDisableLighting();
		ofSetColor(ofColor::yellow);
		ofNoFill();
		ofPushMatrix();
			ofMultMatrix(Transform);
			ofDrawAxis(radius * 1.5);
			ofDrawSphere(ofVec3f::zero(), radius);
		ofPopMatrix();
		ofFill();
		ofEnableLighting();
	}
	ofSetColor(ofColor::white);
	sceneMaterial.begin();
	sceneMaterial.setDiffuseColor(objMaterial.diffuseColor);
	ofPushMatrix();
		ofMultMatrix(Transform);
		ofDrawSphere(ofVec3f::zero(), radius);
	ofPopMatrix();
	sceneMaterial.end();
}

float Sphere::sdf(glm::vec3 p1) {
	glm::vec3 p = glm::inverse(Transform) * glm::vec4(p1.x, p1.y, p1.z, 1.0);
	float distance = glm::distance(p, glm::vec3(0, 0, 0)) - radius;
	return distance;
}

glm::vec2 Sphere::getUV(glm::vec3 p) {
	glm::vec3 n = glm::normalize(p - position);
	float u = 0.5f + (atan2(n.x, n.z) / (2 * PI));
	float v = 0.5f - (asin(n.y) / PI);

	return  glm::vec2(u, v);
}