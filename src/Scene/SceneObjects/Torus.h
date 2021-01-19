#pragma once

#include "ofMain.h"
#include "../SceneObject.h"
#include <math.h>

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
	int   SolveP2(double *x, double a, double b);			// solve equation x^2 + a*x + b = 0

// x - array of size 3
// return 3: 3 real roots x[0], x[1], x[2]
// return 1: 1 real root x[0] and pair of complex roots: x[1]켲*x[2]
	int   SolveP3(double *x, double a, double b, double c);			// solve cubic equation x^3 + a*x^2 + b*x + c = 0

	// x - array of size 4
	// return 4: 4 real roots x[0], x[1], x[2], x[3], possible multiple roots
	// return 2: 2 real roots x[0], x[1] and complex x[2]켲*x[3], 
	// return 0: two pair of complex roots: x[0]켲*x[1],  x[2]켲*x[3], 
	int   SolveP4(double *x, double a, double b, double c, double d);	// solve equation x^4 + a*x^3 + b*x^2 + c*x + d = 0 by Dekart-Euler method

	int   SolveP4Bi(double *x, double b, double d);				// solve equation x^4 + b*x^2 + d = 0
	int   SolveP4De(double *x, double b, double c, double d);	// solve equation x^4 + b*x^2 + c*x + d = 0
	void  CSqrt(double x, double y, double &a, double &b);		// returns as a+i*s,  sqrt(x+i*y)
	double N4Step(double x, double a, double b, double c, double d);// one Newton step for x^4 + a*x^3 + b*x^2 + c*x + d


};
