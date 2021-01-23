#include "Torus.h"          
#define	TwoPi  6.28318530717958648
const double eps = 1e-14; 

Torus::Torus(glm::vec3 p, float r1, float r2, string name, ofColor diffuse) {
	position = p;
	R = r1;
	r = r2;
	objName = name;
	diffuseColor = diffuse;
}

// Constructor with angle and axis of ratation parameters
Torus::Torus(glm::vec3 p, float r1, float r2, float a, glm::vec3 rot, string name, ofColor diffuse) {
	position = p;
	R = r1;
	r = r2;
	objName = name;
	diffuseColor = diffuse;
	angle = a;
	axisR = rot;
}

bool Torus::intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal) {
	glm::vec2 tor = glm::vec2(R, r);
	//glm::mat4 Translate, Rotation;
	//glm::vec3 rdd, roo;
	//Ray temp = Ray(ray.p, ray.d);

	//// Apply Transformation
	//Translate = glm::translate(glm::mat4(1.0), position);
	//Rotation = glm::rotate(Translate, glm::radians(angle), glm::vec3(axisR.x, axisR.y, axisR.z));
	//rdd = (glm::inverse(Rotation) * glm::vec4(ray.d.x, ray.d.y, ray.d.z, 0.0));
	//roo = (glm::inverse(Rotation) * glm::vec4(ray.p.x, ray.p.y, ray.p.z, 1.0));
	//float po = 1.0;
	//float Ra2 = tor.x * tor.x;
	//float ra2 = tor.y * tor.y;

	//float m = dot(roo, roo);
	//float n = dot(roo, rdd);

	//float k = (m - ra2 - Ra2) / 2.0;
	//float k3 = n;
	//float k2 = n * n + Ra2 * rdd.z * rdd.z + k;
	//float k1 = k * n + Ra2 * roo.z * rdd.z;
	//float k0 = k * k + Ra2 * roo.z * roo.z - Ra2 * ra2;

	//if (abs(k3 * (k3 * k3 - k2) + k1) < 0.01) {
	//	po = -1.0;
	//	float tmp = k1;
	//	k1 = k3;
	//	k3 = tmp;

	//	k0 = 1.0 / k0;
	//	k1 = k1 * k0;
	//	k2 = k2 * k0;
	//	k3 = k3 * k0;
	//}

	//float c2 = 2.0 * k2 - 3.0 * k3 * k3;
	//float c1 = k3 * (k3 * k3 - k2) + k1;
	//float c0 = k3 * (k3 * (-3.0 * k3 * k3 + 4.0 * k2) - 8.0 * k1) + 4.0 * k0;

	//c2 /= 3.0;
	//c1 *= 2.0;
	//c0 /= 3.0;

	//float Q = c2 * c2 + c0;
	//float Ra = 3.0 * c0 * c2 - c2 * c2 * c2 - c1 * c1;

	//float h = Ra * Ra - Q * Q * Q;
	//float z = 0.0;
	//if (h < 0.0) {
	//	// 4 intersections
	//	float sQ = sqrt(Q);
	//	z = 2.0 * sQ * cos(acos(Ra / (sQ * Q)) / 3.0);
	//}
	//else {
	//	// 2 intersections
	//	float sQ = pow(sqrt(h) + abs(Ra), 1.0 / 3.0);
	//	z = glm::sign(Ra) * abs(sQ + Q / sQ);
	//}
	//z = c2 - z;

	//float d1 = z - 3.0 * c2;
	//float d2 = z * z - 3.0 * c0;

	//if (abs(d1) < 1.0e-4) {
	//	if (d2 < 0.0)
	//		return false;
	//	d2 = sqrt(d2);
	//}
	//else {
	//	if (d1 < 0.0)
	//		return false;
	//	d1 = sqrt(d1 / 2.0);
	//	d2 = c1 / d1;
	//}

	//float result = 1e20;

	//h = d1 * d1 - z + d2;
	//if (h > 0.0) {
	//	h = sqrt(h);
	//	float t1 = -d1 - h - k3;
	//	t1 = (po < 0.0) ? 2.0 / t1 : t1;
	//	float t2 = -d1 + k - k3;
	//	t2 = (po < 0.0) ? 2.0 / t2 : t2;
	//	
	//	if (t1 > 0.0) {
	//		result = t1;
	//	}
	//	if (t2 > 0.0) {
	//		result = min(result, t2);
	//	}
	//}

	//h = d1 * d1 - z - d2;
	//if (h > 0.0) {
	//	h = sqrt(h);
	//	float t1 = d1 - h - k3;
	//	t1 = (po < 0.0) ? 2.0 / t1 : t1;
	//	float t2 = d1 + h - k3;
	//	t2 = (po < 0.0) ? 2.0 / t2 : t2;
	//	
	//	if (t1 > 0.0)
	//		result = min(result, t1);
	//	if (t2 > 0.0)
	//		result = min(result, t2);
	//}
	glm::vec3 rdd, roo;
	Ray temp = Ray(ray.p, ray.d);

	// Apply Transformation
	//rdd = (glm::inverse(Transform) * glm::vec4(ray.d.x, ray.d.y, ray.d.z, 0.0));
	//roo = (glm::inverse(Transform) * glm::vec4(ray.p.x, ray.p.y, ray.p.z, 1.0));
	glm::vec4 p = glm::inverse(Transform) * glm::vec4(ray.p.x, ray.p.y, ray.p.z, 1.0);
	glm::vec4 p1 = glm::inverse(Transform) * glm::vec4(ray.p + ray.d, 1.0);
	roo = glm::vec4(p.x, p.y, p.z, 1.0);
	rdd = glm::normalize(p1 - p);

	//float t = result;
	float mm = dot(rdd, rdd);
	float nn = dot(roo, rdd);
	float ee = dot(roo, roo) - (R * R + r * r);
	float four_a_sqrd = 4.0 * R * R;

	float cc4 = mm * mm;
	float cc3 = 4.0 * mm * nn;
	float cc2 = 2.0 * mm * ee + 4.0 * nn * nn + four_a_sqrd * rdd.z * rdd.z;
	float cc1 = 4.0 * nn * ee + 2.0 * four_a_sqrd * roo.z * rdd.z;
	float cc0 = ee * ee - four_a_sqrd * (r * r - roo.z * roo.z);

	// Quartic variables
	cc4 /= cc4;
	cc3 /= cc4;
	cc2 /= cc4;
	cc1 /= cc4;
	cc0 /= cc4;

	double roots[4];
	int realRoots = SolveP4(roots, cc3, cc2, cc1, cc0);

	if (realRoots == 0)
		return false;

	float root = FLT_MAX;
	for (int i = 0; i < realRoots; i++) {
		if(root > static_cast<float>(roots[i]))
			root = static_cast<float>(roots[i]);
	}
	if (root < 0.05) return false;
	Ray ray1 = Ray(roo, rdd);
	point = ray1.evalPoint(root);
	//cout << "[" << imageY << ", " << imageX << "]	" << point << endl;
	//glm::vec3 pPrime = glm::vec3(point.x, point.y, 0.0);
	//float Qp = R / sqrt(point.x * point.x + point.y * point.y);

	//point = Rotation * glm::vec4(point.x, point.y, point.z, 0.0);
	//double an = 1.0 - (R / sqrt(point.x * point.x + point.y * point.y));
	//normal = glm::normalize(glm::vec3(an * point.x, an * point.y, point.z));
	//normal = glm::vec3(
	//	point.x * (pow(point.x, 2) + pow(point.y, 2) + pow(point.z, 2) - r * r - R * R),
	//	point.y * (pow(point.x, 2) + pow(point.y, 2) + pow(point.z, 2) - r * r - R * R),
	//	point.z * (pow(point.x, 2) + pow(point.y, 2) + pow(point.z, 2) - r * r - R * R * -1)
	//);
	//normal = glm::normalize(glm::vec4(normal.x, normal.y, normal.z, 0.0) *Rotation );
	//float paramSquared = R * R + r * r;
	//glm::vec3 pp = glm::inverse(Rotation) * glm::vec4(point.x, point.y, point.z, 1.0);
	//float sumSquared = pp.x * pp.x + pp.y * pp.y + pp.z * pp.z;

	//normal = glm::vec3(
	//	4.0 * pp.x * (sumSquared - paramSquared),
	//	4.0 * pp.y * (sumSquared - paramSquared),
	//	4.0 * pp.z * (sumSquared - paramSquared + 2.0 * R * R)
	//);

	normal = point * (dot(point, point) - r * r - R * R * glm::vec3(1.0, 1.0, -1.0));
	normal = glm::normalize(getRotateMatrix() * glm::vec4(normal.x, normal.y, normal.z, 1.0));
	point = Transform * glm::vec4(point.x, point.y, point.z, 1.0);
	//normal = glm::vec3(0, 0, 1);
	//if (shading) {
		points.push_back(point);
		normals.push_back((point + normal/2));
	//}
	//cout << "Num: " << points.size() << " \\ " << Fpoints.size() << endl;
	return true;
}



void Torus::draw() {
	applyMatrix();
	ofDisableLighting();
	ofSetColor(ofColor::red);
	//for (int i = 0; i < points.size(); i++) {
	//	ofSetColor(ofColor::blue);
	//	ofDrawSphere(points[i], 0.025);
	//	ofSetColor(ofColor::yellow);
	//	ofDrawLine(points[i], normals[i]);
	//}
	//for (int i = 0; i < Fpoints.size(); i++) {
	//	ofSetColor(ofColor::red);
		//ofDrawSphere(Fpoints[i], 0.025);
	//	ofSetColor(ofColor::yellow);
		//ofDrawLine(points[i], normals[i]);
	//}
	ofEnableLighting();
	if (isSelected) {
		ofDisableLighting();
		ofSetColor(ofColor::yellow);
		ofNoFill();
		ofPushMatrix();
			ofMultMatrix(Transform);
			ofDrawAxis(R * 1.5);
			drawTorus();
		ofPopMatrix();
		ofFill();
		ofEnableLighting();
	}

	ofSetColor(ofColor::white);

	material.begin();
	material.setDiffuseColor(diffuseColor);
	ofPushMatrix();
		ofMultMatrix(Transform);
		drawTorus();
	ofPopMatrix();
	material.end();
}

void Torus::drawTorus() {
	double s, x, y, z;

	for (int i = 0; i < numc; i++) {
		glBegin(GL_QUAD_STRIP);
		for (int j = 0; j <= numt; j++) {
			for (int k = 1; k >= 0; k--) {
				s = (i + k) % numc;
				x = (R + r * cos(s * 2 * PI / numc))*cos(j * 2 * PI / numt);
				y = (R + r * cos(s * 2 * PI / numc))*sin(j * 2 * PI / numt);
				z = r * sin(s * 2 * PI / numc);
				glVertex3f(x, y, z);

			}
		}
		glEnd();
	}
}

// Calculate the sdf of the torus scene object while applying transformations
float Torus::sdf(glm::vec3 p1) {
	// Transformation matrices(translate and rotate)
	glm::mat4 m = glm::translate(glm::mat4(1.0), position);
	glm::mat4 M = glm::rotate(m, glm::radians(angle), axisR);
	glm::vec4 p = glm::inverse(M) * glm::vec4(p1.x, p1.y, p1.z, 1.0);
	glm::vec2 q = glm::vec2(glm::length(glm::vec2(p.x, p.y)) - R, p.z);
	return glm::length(q) - r;
}
