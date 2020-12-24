#pragma once

#include "ofMain.h"
#include "../SceneObject.h"

//************************************ Fractal Class **************************************
class LSystem : public SceneObject {
public:
	string axiom = "F";
	string sentence = axiom;
	struct Rule {
		char a;
		string b;
		//F[+F[+FX]+FX]+F[+FX]+FX
	};
	Rule rule1;
	Rule rule2;
	Rule rule3;
	vector<Rule> rules;
	int n = 1;
	vector<float> xVec, yVec, zVec, turnAngVec, pitchAngVec, rollAngVec;
	vector<glm::mat4> mats;
	vector<glm::mat4> jmats;
	float x, y, z;
	//float mainAngle = 28.0f;
	float turnAng = 90.0f;
	float pitchAng = 40.0f;
	float rollAng = 20.0f;
	float jointRadius = .15f;

	float Iterations = 4.0f;
	float Scale = 2.0f;
	float Offset = 8.0f;
	float temp = 1.0f;

	LSystem(glm::vec3 p, float t, ofColor diffuse = ofColor::lightGray);
	LSystem() {};
	bool intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal);
	void draw();

	void generate(int n);
	float sdf(glm::vec3 p);
};