#pragma once

#include "ofMain.h"
#include "../SceneObject.h"
#include "Cylinder.h"
#include "Sphere.h"

class Rule {
public:
	char a;
	string b;
	//F[+F[+FX]+FX]+F[+FX]+FX
};

//************************************ LSystem Class **************************************
class LSystem : public SceneObject {
public:
	//Cylinder tube;
	//Sphere joint;
	string axiom, sentence;
	Rule rule1, rule2, rule3;
	vector<Rule> rules;
	int iterations;
	float angle = 22.5f;
	float tubeRadius = 0.2f;
	float tubeHeight = 0.5f;
	vector<glm::vec3> points;
	vector<glm::vec3> normals;
	vector<Box *> boxes;
	vector<SceneObject *> objs;

	vector<float> xVec, yVec, zVec, turnAngVec, pitchAngVec, rollAngVec;
	vector<glm::mat4> mats, jmats;

	void setBounds();
	LSystem(glm::vec3 p, int n, string ax, string name, ofColor diffuse = ofColor::lightGray);
	LSystem() {};
	~LSystem() {};

	bool intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal, ofColor &surfaceColor);
	void draw();
	void drawLSystem();

	void generate();
	void build();
	float sdf(glm::vec3 p);
};