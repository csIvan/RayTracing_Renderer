#include "Cube.h"

Cube::Cube(glm::vec3 p, float s, ofColor diffuse) {
	position = p;
	side = s;
	diffuseColor = diffuse;
	minv = glm::vec3(0.0f - (side / 2), 0.0f - (side / 2), 0.0f + (side / 2));
	maxv = glm::vec3(0.0f + (side / 2), 0.0f + (side / 2), 0.0f - (side / 2));
	bounds[0] = minv;
	bounds[1] = maxv;
	cout << minv << " : " << maxv << endl;
	
}

bool Cube::intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal) {
	// Placeholder intersect
	Ray rTemp = Ray(ray.p, ray.d);

	glm::mat4 m = glm::translate(glm::mat4(1.0), position);
	glm::mat4 M = glm::rotate(m, glm::radians(-angle), axisR);
	glm::vec3 rdd = (glm::inverse(M) * glm::vec4(ray.d.x, ray.d.y, ray.d.z, 0.0));
	glm::vec3 roo = (glm::inverse(M) * glm::vec4(ray.p.x, ray.p.y, ray.p.z, 1.0));


	glm::vec3 invdir = glm::vec3(1 / rdd.x, 1 / rdd.y, 1 / rdd.z);
	int sign[3] = { (invdir.x < 0), (invdir.y < 0), (invdir.z < 0) };

	float tmin, tmax, txmin, txmax, tymin, tymax, tzmin, tzmax;

	tmin = (bounds[sign[0]].x - roo.x) * invdir.x;
	tmax = (bounds[1 - sign[0]].x - roo.x) * invdir.x;
	tymin = (bounds[sign[1]].y - roo.y) * invdir.y;
	tymax = (bounds[1 - sign[1]].y - roo.y) * invdir.y;

	if ((tmin > tymax) || (tymin > tmax))
		return false;
	if (tymin > tmin)
		tmin = tymin;
	if (tymax < tmax)
		tmax = tymax;

	tzmin = (bounds[1 - sign[2]].z - roo.z) * invdir.z;
	tzmax = (bounds[sign[2]].z - roo.z) * invdir.z;

	if ((tmin > tzmax) || (tzmin > tmax))
		return false;
	if (tzmin > tmin)
		tmin = tzmin;
	if (tzmax < tmax)
		tmax = tzmax;


	glm::vec3 q = abs(glm::vec3(roo.x, roo.y, roo.z)) - glm::vec3(side/2, side/2, side/2);
	float distCube = MIN(MAX(q.x, max(q.y, q.z)), 0.0) + length(max(q, 0.0f));
	if (tmin < distCube) 
		return false;


	glm::vec3 s = glm::vec3((rdd.x < 0.0) ? 1.0 : -1.0,
		(rdd.y < 0.0) ? 1.0 : -1.0, (rdd.z < 0.0) ? 1.0 : -1.0);

	glm::vec3 n = invdir * roo;
	//glm::vec3 k = abs(invdir) * glm::vec3(side / 2, side / 2, side / 2);
	glm::vec3 k = s * glm::vec3(side / 2, side / 2, side / 2);
	glm::vec3 ko = glm::vec3(-roo.x + k.x, -roo.y + k.y, -roo.z + k.z);
	glm::vec3 ka = glm::vec3(-roo.x - k.x, -roo.y - k.y, -roo.z - k.z);
	glm::vec3 t1 = invdir * ko;
	glm::vec3 t2 = invdir * ka;
	//glm::vec3 t1 = -n - k;
	//glm::vec3 t2 = -n + k;

	float tN = max(max(t1.x, t1.y), t1.z);
	float tF = min(min(t2.x, t2.y), t2.z);

	if (tN > tF || tF < 0.0) return false;

	point = rTemp.evalPoint(distCube);
	//cout << "[" << imageX << ", " << imageY << "] = Point: " << point << endl << "     TMin: " << txmin << ", TMax: " << txmax << endl;

	//glm::vec3 c = (minv + maxv) * 0.5;
	//glm::vec3 p = point - c;
	//glm::vec3 d = (minv - maxv) * 0.5;
	//float bias = 1.00001;

	//normal = glm::normalize(glm::vec3((int)(p.x / abs(d.x) * bias),
	//	(int)(p.y / abs(d.y) * bias),
	//	(int)(p.z / abs(d.z) * bias)));

	if(t1.x > t1.y && t1.x >t1.z)
		normal = glm::vec3(M[0].x * s.x, M[0].y * s.x, M[0].z * s.x);
	else if(t1.y > t1.z)
		normal = glm::vec3(M[1].x * s.y, M[1].y * s.y, M[1].z * s.y);
	else
		normal = glm::vec3(M[2].x * s.z, M[2].y * s.z, M[2].z * s.z);
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