#include "Cube.h"

Cube::Cube(glm::vec3 p, float s, ofColor diffuse) {
	position = p;
	side = s;
	diffuseColor = diffuse;
	minv = glm::vec3(0.0f - (side / 2), 0.0f - (side / 2), 0.0f + (side / 2));
	maxv = glm::vec3(0.0f + (side / 2), 0.0f + (side / 2), 0.0f - (side / 2));
	glm::mat4 m = glm::translate(glm::mat4(1.0), position);
	glm::mat4 M = glm::rotate(m, glm::radians(angle), axisR);
	minv = glm::inverse(M) * glm::vec4(minv.x, minv.y, minv.z, 1.0);
	maxv = glm::inverse(M) * glm::vec4(maxv.x, maxv.y, maxv.z, 1.0);
	bounds[0] = minv;
	bounds[1] = maxv;
	cout << minv << " : " << maxv << endl;
	
}

bool Cube::intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal) {
	// Placeholder intersect
	Ray rTemp = Ray(ray.p, ray.d);
	glm::mat4 m = glm::translate(glm::mat4(1.0), position);
	glm::mat4 M = glm::rotate(m, glm::radians(angle), axisR);



	glm::vec3 invdir = glm::vec3(1 / ray.d.x, 1 / ray.d.y, 1 / ray.d.z);
	int sign[3] = { (invdir.x < 0), (invdir.y < 0), (invdir.z < 0) };

	float tmin, tmax, tymin, tymax, tzmin, tzmax;

	tmin = (bounds[sign[0]].x - ray.p.x) * invdir.x;
	tmax = (bounds[1 - sign[0]].x - ray.p.x) * invdir.x;
	tymin = (bounds[sign[1]].y - ray.p.y) * invdir.y;
	tymax = (bounds[1 - sign[1]].y - ray.p.y) * invdir.y;

	if ((tmin > tymax) || (tymin > tmax))
		return false;
	if (tymin > tmin)
		tmin = tymin;
	if (tymax < tmax)
		tmax = tymax;

	tzmin = (bounds[1 - sign[2]].z - ray.p.z) * invdir.z;
	tzmax = (bounds[sign[2]].z - ray.p.z) * invdir.z;

	if ((tmin > tzmax) || (tzmin > tmax))
		return false;
	if (tzmin > tmin)
		tmin = tzmin;
	if (tzmax < tmax)
		tmax = tzmax;


	glm::vec3 q = abs(ray.p) - glm::vec3(side/2, side/2, side/2);
	float distCube = MIN(MAX(q.x, max(q.y, q.z)), 0.0) + length(max(q, 0.0f));
	if (tmin < distCube) 
		return false;


	point = rTemp.evalPoint(tmin);

	glm::vec3 c = (minv + maxv) * 0.5;
	glm::vec3 p = point - c;
	glm::vec3 d = (minv - maxv) * 0.5;
	float bias = 1.00001;

	normal = glm::normalize(glm::vec3((int)(p.x / abs(d.x) * bias),
		(int)(p.y / abs(d.y) * bias),
		(int)(p.z / abs(d.z) * bias)));
	cout << "[" << imageX << ", " << imageY << "] = Point: " << point << endl << "     TMin: " << tmin << ", TMax: " << distCube << endl;
	//cout << "     N: " << n << " /  K: " << invdir << " /  T1: " << ray.p << "  /  T2: " << ray.d << "  /  TN: " << tN << "  /  TF: " << tF << endl;
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
	ofDrawLine(minv, maxv);
}

float Cube::sdf(const glm::vec3 p1) {
	glm::mat4 m = glm::translate(glm::mat4(1.0), position);
	glm::mat4 M = glm::rotate(m, glm::radians(angle), axisR);
	glm::vec4 p = glm::inverse(M) * glm::vec4(p1.x, p1.y, p1.z, 1.0);
	glm::vec3 q = abs(p) - side;
	return MIN(MAX(q.x, max(q.y, q.z)), 0.0) + length(max(q, 0.0f));
}