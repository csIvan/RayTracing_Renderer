#pragma once

#include "../SceneObject.h"
#include "Cylinder.h"
#include "Sphere.h"

struct Rule {
	char symbol;
	string sequence;
	//F[+F[+FX]+FX]+F[+FX]+FX
};

// 3D LSystem scene object class
class LSystem : public SceneObject {
private:
	string axiom;
	string sentence;
	Rule rule1;
	Rule rule2;
	Rule rule3;
	vector<Rule> rules;
	int iterations = 1;
	float angle = 22.5f;
	float tubeRadius = 0.2f;
	float tubeHeight = 0.5f;

	vector<SceneObject *> geometryObjects;

public:
	LSystem(const glm::vec3 &position, int iterations, const string &axiom, const string &name, const ofColor &diffuse = DEFAULT_COLOR);
	~LSystem() {};


	// Override functions from SceneObject
	void setBounds();
	void draw();
	bool intersect(const Ray &ray, HitInfo &hitInfo);
	float sdf(const glm::vec3 &point);

	
	// Custom draw function for Lsystem
	void drawLSystem();


	// Generate full Lsystem grammar string
	void generate();


	// Build LSystem shape based on full sentence string
	void build();


	// Setter and Getters
	void setIterations(int iterations) { this->iterations = iterations; }
	void setAngle(float angle) { this->angle = angle; }
	void setTubeRadius(float tubeRadius) { this->tubeRadius = tubeRadius; }
	void setTubeHeight(float tubeHeight) { this->tubeHeight = tubeHeight; }
	void setAxiom(string axiom) { this->axiom = axiom; }
	void setRule1(string rule) { rule1.sequence = rule; }
	void setRule2(string rule) { rule2.sequence = rule; }
	void setRule3(string rule) { rule3.sequence = rule; }
	int getIterations() const { return iterations; }
	float getAngle() const { return angle; }
	float getTubeRadius() const { return tubeRadius; }
	float getTubeHeight() const { return tubeHeight; }
	string getAxiom() const { return axiom; }
	string getSentence() const { return sentence; }
	string getRule1() const { return rule1.sequence; }
	string getRule2() const { return rule2.sequence; }
	string getRule3() const { return rule3.sequence; }
};