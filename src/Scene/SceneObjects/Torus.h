#pragma once

#include "ofMain.h"
#include "../../Math/poly34.h"
#include "../SceneObject.h"

//************************************** Torus Class *********************************
class Torus : public SceneObject {
public:
	ofMaterial material;
	float R;	// Larger radius; from center of torus to center of tube
	float r;	// Smaller radius; radius of tube
	float angle = 0.0f;
	glm::vec3 axisR = glm::vec3(1, 0, 0);
	float imageX, imageY;
	vector<glm::vec3> points;
	vector<glm::vec3> normals;
	vector<glm::vec3> Fpoints;
	vector<glm::vec3> Fnormals;
	bool shading = false;

	int numc = 20;
	int numt = 35;

	Torus(glm::vec3 p, float r1, float r2, string name, ofColor diffuse = ofColor::azure);
	Torus(glm::vec3 p, float r1, float r2, float a, glm::vec3 rot, string name, ofColor diffuse = ofColor::azure);
	Torus() {};
	~Torus() {};

	bool intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal);
	void draw();
	void drawTorus();
	float sdf(glm::vec3 p1);

};
