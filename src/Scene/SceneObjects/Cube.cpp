#include "Cube.h"

Cube::Cube(glm::vec3 p, float s, ofColor diffuse) {
	position = p;
	side = s;
	diffuseColor = diffuse;
}

bool Cube::intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal) {
	Ray rTemp = Ray(ray.p, ray.d);

	// Apply Transformation
	glm::mat4 m = glm::translate(glm::mat4(1.0), position);
	glm::mat4 M = glm::rotate(m, glm::radians(angle), glm::vec3(-axisR.x, axisR.y, axisR.z));
	glm::vec3 rdd = (glm::inverse(M) * glm::vec4(ray.d.x, ray.d.y, ray.d.z, 0.0));
	glm::vec3 roo = (glm::inverse(M) * glm::vec4(ray.p.x, ray.p.y, ray.p.z, 1.0));

	// Calculate intersection
	glm::vec3 invdir = glm::vec3(1 / rdd.x, 1 / rdd.y, 1 / rdd.z);
	glm::vec3 s = glm::vec3((rdd.x < 0.0) ? 1.0 : -1.0, (rdd.y < 0.0) ? 1.0 : -1.0, (rdd.z < 0.0) ? 1.0 : -1.0);
	glm::vec3 k = s * glm::vec3(side / 2, side / 2, side / 2);

	glm::vec3 ko = glm::vec3(-roo.x + k.x, -roo.y + k.y, -roo.z + k.z);
	glm::vec3 ka = glm::vec3(-roo.x - k.x, -roo.y - k.y, -roo.z - k.z);

	glm::vec3 t1 = invdir * ko;
	glm::vec3 t2 = invdir * ka;

	// Calculate entering and exiting points
	float tN = max(max(t1.x, t1.y), t1.z);
	float tF = min(min(t2.x, t2.y), t2.z);

	if (tN > tF || tF < 0.0 || isnan(tF)) 
		return false;

	// Point
	point = rTemp.evalPoint(tN);

	// Normal
	normal = -glm::sign(rdd) * glm::step(glm::vec3(t1.y, t1.z, t1.x), t1) * glm::step(glm::vec3(t1.z, t1.x, t1.y), t1);
	normal = (M * glm::vec4(normal.x, normal.y, normal.z, 0.0));	
	
	return true;
}

void Cube::draw() {
	ofPushMatrix();
		ofTranslate(position);
		ofRotate(angle, -axisR.x, axisR.y, axisR.z);
		ofDrawAxis(2);
		ofDrawBox(ofVec3f::zero(), side);
		ofSetColor(ofColor::azure);

	ofPopMatrix();
}

float Cube::sdf(const glm::vec3 p1) {
	glm::mat4 m = glm::translate(glm::mat4(1.0), position);
	glm::mat4 M = glm::rotate(m, glm::radians(angle), axisR);
	glm::vec4 p = glm::inverse(M) * glm::vec4(p1.x, p1.y, p1.z, 1.0);
	glm::vec3 q = abs(p) - side;
	return MIN(MAX(q.x, max(q.y, q.z)), 0.0) + length(max(q, 0.0f));
}