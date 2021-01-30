#pragma once

#include "ofMain.h"
#include "../SceneObject.h"

class Rule {
public:
	char a;
	string b;
	//F[+F[+FX]+FX]+F[+FX]+FX
};

//************************************ LSystem Class **************************************
class LSystem : public SceneObject {
public:
	ofMaterial material;	//TODO: change later for sceneObject.h to have this variable
	string axiom, sentence;
	Rule rule1, rule2, rule3;
	vector<Rule> rules;
	int iterations;

	vector<float> xVec, yVec, zVec, turnAngVec, pitchAngVec, rollAngVec;
	vector<glm::mat4> mats, jmats;

	LSystem(glm::vec3 p, int n, string ax, string name, ofColor diffuse = ofColor::lightGray);
	LSystem() {};
	~LSystem() {};

	bool intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal);
	void draw();

	void generate();
	float sdf(glm::vec3 p);
};