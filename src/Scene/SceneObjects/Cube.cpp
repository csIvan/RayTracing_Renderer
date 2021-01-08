#include "Cube.h"

Cube::Cube(glm::vec3 p, float s, ofColor diffuse) {
	position = p;
	side = s;
	diffuseColor = diffuse;
}

bool Cube::intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal) {
	glm::mat4 Translate, Rotation;
	glm::vec3 rdd, roo, invdir, s, k, ko, ka, t1, t2;

	Ray rTemp = Ray(ray.p, ray.d);

	// Apply Transformation
	Translate = glm::translate(glm::mat4(1.0), position);
	Rotation = glm::rotate(Translate, glm::radians(angle), glm::vec3(-axisR.x, axisR.y, axisR.z));

	glm::vec3 objOrigin = (Rotation * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	//if (angle != 0.0) {
		rdd = (glm::inverse(Rotation) * glm::vec4(ray.d.x, ray.d.y, ray.d.z, 0.0));
		roo = (glm::inverse(Rotation) * glm::vec4(ray.p.x, ray.p.y, ray.p.z, 1.0));
	//}
	//else {
	//	rdd = (glm::inverse(Translate) * glm::vec4(ray.d.x, ray.d.y, ray.d.z, 0.0));
	//	roo = (glm::inverse(Translate) * glm::vec4(ray.p.x, ray.p.y, ray.p.z, 1.0));
	//}

	// Calculate intersection
	invdir = glm::vec3(1 / rdd.x, 1 / rdd.y, 1 / rdd.z);
	s = glm::vec3((rdd.x < 0.0) ? 1.0 : -1.0, (rdd.y < 0.0) ? 1.0 : -1.0, (rdd.z < 0.0) ? 1.0 : -1.0);
	k = s * glm::vec3(side / 2, side / 2, side / 2);

	ko = glm::vec3(-roo.x + k.x, -roo.y + k.y, -roo.z + k.z);
	ka = glm::vec3(-roo.x - k.x, -roo.y - k.y, -roo.z - k.z);

	t1 = invdir * ko;
	t2 = invdir * ka;

	// Calculate entering and exiting points
	float tN = max(max(t1.x, t1.y), t1.z);
	float tF = min(min(t2.x, t2.y), t2.z);

	float d = glm::distance(abs(roo), objOrigin) - side;
	//glm::vec3 d = abs(roo) - sqrt(2.0f);
	//float cubeDist = MIN(MAX(d.x, max(d.y, d.z)), 0.0) + length(max(d, 0.0f));

	if (tN > tF || tF < 0.0 || isnan(tF) || tN < d)
		return false;

	//cout << "abs(roo) " <<objOrigin << "  //  new: " << d << "  //  D: " << d << endl;

	// Point
	point = rTemp.evalPoint(tN);

	// Normal
	normal = -glm::sign(rdd) * glm::step(glm::vec3(t1.y, t1.z, t1.x), t1) * glm::step(glm::vec3(t1.z, t1.x, t1.y), t1);

	if (angle != 0.0) {
		normal = (glm::inverse(Rotation) * glm::vec4(normal.x, normal.y, normal.z, 0.0));	
	}
	else {
		normal = (Translate * glm::vec4(normal.x, normal.y, normal.z, 0.0));
	}

	if (t1.x > t1.y && t1.x > t1.z) 
		normal = glm::vec3(Rotation[0].x * s.x, Rotation[0].y*s.x, Rotation[0].z*s.x);
	else if (t1.y > t1.z) 
		normal = glm::vec3(Rotation[1].x * s.y, Rotation[1].y*s.y, Rotation[1].z*s.y);
	else
		normal = glm::vec3(Rotation[2].x * s.z, Rotation[2].y*s.z, Rotation[2].z*s.z);
	//cout << "Point: " << point << ", Normal: " << normal << endl;
	//cout << "		tN: " << tN << ", tF: " << tF << ", t1: " << t1 << ", t2: " << t2 << endl;

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