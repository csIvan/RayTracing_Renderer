#include "Torus.h"

Torus::Torus(glm::vec3 p, float l1, float l2, string name, ofColor diffuse) {
	position = p;
	innerRadius = l1;
	outerRadius = l2;
	objName = name;
	diffuseColor = diffuse;
}

// Constructor with angle and axis of ratation parameters
Torus::Torus(glm::vec3 p, float l1, float l2, float a, glm::vec3 r, string name, ofColor diffuse) {
	position = p;
	innerRadius = l1;
	outerRadius = l2;
	objName = name;
	diffuseColor = diffuse;
	angle = a;
	axisR = r;
}

void Torus::draw() {
	if (isSelected) {
		ofDisableLighting();
		ofSetColor(ofColor::yellow);
		ofNoFill();
		ofPushMatrix();
			ofTranslate(position);
			ofRotate(angle, axisR.x, axisR.y, axisR.z);
			ofDrawAxis(outerRadius * 1.5);
			drawTorus();
		ofPopMatrix();
		ofFill();
		ofEnableLighting();
	}

	ofSetColor(ofColor::white);

	material.begin();
	material.setDiffuseColor(diffuseColor);
	ofPushMatrix();
		ofTranslate(position);
		ofRotate(angle, axisR.x, axisR.y, axisR.z);
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
				x = (innerRadius + outerRadius * cos(s * 2 * PI / numc))*cos(j * 2 * PI / numt);
				y = (innerRadius + outerRadius * cos(s * 2 * PI / numc))*sin(j * 2 * PI / numt);
				z = outerRadius * sin(s * 2 * PI / numc);
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
	glm::vec2 q = glm::vec2(glm::length(glm::vec2(p.x, p.y)) - innerRadius, p.z);
	return glm::length(q) - outerRadius;
}