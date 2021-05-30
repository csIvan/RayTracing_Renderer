#include "Torus.h" 
#define EPSILON 0.05

Torus::Torus(glm::vec3 p, float r1, float r2, string name, ofColor diffuse) {
	position = p;
	R = r1;
	r = r2;
	objName = name;
	objMaterial.diffuseColor = diffuse;
}

bool Torus::intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal, glm::vec2 &uv) {
	glm::vec3 rdd, roo;

	// Apply Transformation
	glm::vec4 p = glm::inverse(Transform) * glm::vec4(ray.p.x, ray.p.y, ray.p.z, 1.0);
	glm::vec4 p1 = glm::inverse(Transform) * glm::vec4(ray.p + ray.d, 1.0);
	roo = glm::vec4(p.x, p.y, p.z, 1.0);
	rdd = glm::normalize(p1 - p);

	Ray ray1 = Ray(roo, rdd);


	float m = dot(rdd, rdd);
	float n = dot(roo, rdd);
	float e = dot(roo, roo) - (R * R + r * r);
	float four_a_sqrd = 4.0 * R * R;

	// Quartic variables
	float c4 = m * m;
	float c3 = 4.0 * m * n;
	float c2 = 2.0 * m * e + 4.0 * n * n + four_a_sqrd * rdd.z * rdd.z;
	float c1 = 4.0 * n * e + 2.0 * four_a_sqrd * roo.z * rdd.z;
	float c0 = e * e - four_a_sqrd * (r * r - roo.z * roo.z);

	c4 /= c4;
	c3 /= c4;
	c2 /= c4;
	c1 /= c4;
	c0 /= c4;

	// Calculate roots
	double roots[4];
	int realRoots = SolveP4(roots, c3, c2, c1, c0);		

	if (realRoots == 0)
		return false;

	float root = FLT_MAX;
	for (int i = 0; i < realRoots; i++) {
		if(root > static_cast<float>(roots[i]))
			root = static_cast<float>(roots[i]);
	}

	if (root < EPSILON) 
		return false;

	point = ray1.evalPoint(root);

	uv = getUV(point);
	normal = point * (dot(point, point) - r * r - R * R * glm::vec3(1.0, 1.0, -1.0));
	normal = glm::normalize(getRotateMatrix() * glm::vec4(normal.x, normal.y, normal.z, 1.0));
	point = Transform * glm::vec4(point.x, point.y, point.z, 1.0);


	// For Debugging
	points.push_back(point);
	normals.push_back((point + normal/2));

	return true;
}



void Torus::draw() {
	applyMatrix();
	ofDisableLighting();
	ofSetColor(ofColor::red);
	if (debugMode) {
		for (int i = 0; i < points.size(); i++) {
			ofSetColor(ofColor::blue);
			ofDrawSphere(points[i], 0.025);
			ofSetColor(ofColor::yellow);
			ofDrawLine(points[i], normals[i]);
		}
	}

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

	sceneMaterial.begin();
	sceneMaterial.setDiffuseColor(objMaterial.diffuseColor);
	ofPushMatrix();
		ofMultMatrix(Transform);
		drawTorus();
	ofPopMatrix();
	sceneMaterial.end();
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
	glm::vec4 p = glm::inverse(Transform) * glm::vec4(p1.x, p1.y, p1.z, 1.0);
	glm::vec2 q = glm::vec2(glm::length(glm::vec2(p.x, p.y)) - R, p.z);
	return glm::length(q) - r;
}

glm::vec2 Torus::getUV(glm::vec3 p) {
	glm::vec4 pp = glm::vec4(p.x, p.y, p.z, 1.0) * getTranslateMatrix();
	glm::vec3 hit = glm::vec4(pp.x, pp.y, pp.z, 1.0);

	float u = 1.0f - (atan2(hit.x, hit.y) / (2 * PI));
	float v = 0.5f + atan2(-hit.z, glm::length(glm::vec2(hit.x, hit.y)) - R) / (2 * PI);

	return  glm::vec2(glm::abs(u), glm::abs(v));
}