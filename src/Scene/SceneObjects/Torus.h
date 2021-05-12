#pragma once

#include "ofMain.h"
#include "../../Math/poly34.h"
#include "../SceneObject.h"

//************************************** Torus Class *********************************
class Torus : public SceneObject {
public:
	float R;	// Larger radius; from center of torus to center of tube
	float r;	// Smaller radius; radius of tube

	vector<glm::vec3> points;
	vector<glm::vec3> normals;
	bool debugMode = false;

	int numc = 20;
	int numt = 35;

	Torus(glm::vec3 p, float r1, float r2, string name, ofColor diffuse = ofColor::azure);
	Torus() {};
	~Torus() {};

	bool intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal, glm::vec2 &uv);
	void draw();
	void drawTorus();
	float sdf(glm::vec3 p1);

};
