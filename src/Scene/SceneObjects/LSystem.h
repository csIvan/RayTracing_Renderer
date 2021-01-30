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
	string axiom = "F";
	string sentence = axiom;
	Rule rule1, rule2, rule3;
	vector<Rule> rules;
	int n = 1;
	vector<float> xVec, yVec, zVec, turnAngVec, pitchAngVec, rollAngVec;
	vector<glm::mat4> mats, jmats;
	float x, y, z;
	//float mainAngle = 28.0f;
	float turnAng = 90.0f;
	float pitchAng = 40.0f;
	float rollAng = 20.0f;
	float jointRadius = .15f;

	float temp = 1.0f;

	LSystem(glm::vec3 p, float t, string name, ofColor diffuse = ofColor::lightGray);
	LSystem() {};
	~LSystem() {};

	bool intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal);
	void draw();

	void generate(int n);
	float sdf(glm::vec3 p);
};