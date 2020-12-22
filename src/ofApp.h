#pragma once

#define MAX_RAY_STEPS 20
#define DIST_THRESHOLD .001
#define MAX_THRESHOLD 20

#include "ofMain.h"
#include <typeinfo>

//  General Purpose Ray class 
//
class Ray {
public:
	Ray(glm::vec3 p, glm::vec3 d) { this->p = p; this->d = d; }
	void draw(float t) { ofDrawLine(p, p + t * d); }

	glm::vec3 evalPoint(float t) {
		return (p + t * d);
	}

	glm::vec3 p, d;
};

//  Base class for any renderable object in the scene
//
class SceneObject {
public:
	virtual void draw() = 0;    // pure virtual funcs - must be overloaded
	virtual bool intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal) {
		//cout << "SceneObject::intersect" << endl; 
		return false;
	}
	virtual float sdf(glm::vec3 p) {
		return 0.0f;
	}


	// any data common to all scene objects goes here
	glm::vec3 position = glm::vec3(0, 0, 0);

	// material properties (we will ultimately replace this with a Material class - TBD)
	//
	ofColor diffuseColor = ofColor::grey;    // default colors - can be changed.
	ofColor specularColor = ofColor::lightGray;
};

//  General purpose sphere  (assume parametric)
//
class Sphere : public SceneObject {
public:
	Sphere(glm::vec3 p, float r, ofColor diffuse = ofColor::lightGray) { position = p; radius = r; diffuseColor = diffuse; }
	Sphere() {}
	bool intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal) {
		return (glm::intersectRaySphere(ray.p, ray.d, position, radius, point, normal));
	}
	float sdf(glm::vec3 p) {
		float distance = glm::distance(p, position) - radius;
		return  distance;
	}
	void draw() {
		ofDrawSphere(position, radius);
	}

	float radius = 1.0;
};


//************************************ Fractal Class **************************************
class Fractal : public SceneObject {
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
	int n = 4;
	vector<float> xVec, yVec, zVec, turnAngVec, pitchAngVec, rollAngVec;
	vector<glm::mat4> mats;
	vector<glm::mat4> jmats;
	float x, y, z;
	//float mainAngle = 28.0f;
	float turnAng = 90.0f;
	float pitchAng = 40.0f;
	float rollAng = 20.0f;
	float jointRadius = .15f;

	Fractal(glm::vec3 p, float t, ofColor diffuse = ofColor::lightGray) {
		position = p;
		temp = t;
		diffuseColor = diffuse;
		//rule1.a = 'X';
		rule1.a = 'A';
		rule1.b = "F-[[A]+A]+F[+FA][-A]";
		//rule1.b = "FF[+XZ++X-F[+ZX]][-X++F-X]";
		//rule1.a = 'A';
		//rule1.b = "[B]////[B]////[B]";
		rule2.a = 'F';
		//rule2.b = "FX[FX[+XF]]";
		rule2.b = "FF+[+F-F-F]-[-F+F+F]";
		//rule3.a = 'Z';
		//rule3.b = "[+F-X-F][++ZX]";
		rules.push_back(rule1);
		rules.push_back(rule2);
		generate(n);
		cout << "L-System: " << sentence << endl;
	};
	Fractal() {};
	bool intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal) {
		return (glm::intersectRaySphere(ray.p, ray.d, position, temp, point, normal));
	}
	void generate(int n) {
		while (n > 0) {
			string nextSentence = "";
			for (int i = 0; i < sentence.length(); i++) {

				bool found = false;
				for (Rule r : rules) {
					if (sentence[i] == r.a) {
						found = true;
						nextSentence += r.b;
						break;
					}
				}

				if (!found) {
					nextSentence += sentence[i];
				}
			}
			sentence = nextSentence;
			n--;
		}
	}

	//L-System sdf
	float sdf(glm::vec3 p) {
		vector<float> dists;
		//Initial Angles
		float xangle = 0.0f;	//yaw xz
		float yangle = 0.0f;	//roll
		float zangle = 90.0f;	//pitch yz, yx

		//Applied angles
		float tAngle = 22.5f;
		float pAngle = 28.0f;
		float rAngle = 28.0f;

		x = position.x;
		y = position.y;
		z = position.z;
		float thickness = .02f;
		float height = 0.05f;
		float thickness2 = .05f;

		vector<float> ztempAngs;
		vector<float> xtempAngs;
		vector<float> ytempAngs;

		float tempzangle = 0.0f;
		float tempxangle = 0.0f;
		float tempyangle = 0.0f;
		glm::mat4 objM = glm::mat4(1.0);
		glm::mat4 M3 = glm::translate(objM, glm::vec3(x, y, z));
		objM = glm::translate(objM, glm::vec3((-1.0)*x, (-1.0)*y, (-1.0)*z));

		glm::mat4 jointM = glm::mat4(1.0);

		for (int i = 0; i < sentence.length(); i++) {
			if (sentence[i] == 'F' || sentence[i] == 'A') {
				//move forward
				glm::mat4 m = glm::translate(glm::vec3(height, 0, 0));
				glm::mat4 M = glm::rotate(glm::radians(zangle), glm::vec3(0, 0, 1));
				glm::mat4 M2 = glm::rotate(glm::radians(xangle), glm::vec3(0, 1, 0));
				glm::mat4 M3 = glm::rotate(glm::radians(yangle), glm::vec3(1, 0, 0));
				objM = m * M2 * M3 * M  * objM;

				glm::vec4 pp = objM * glm::vec4(p.x, p.y, p.z, 1.0);
				objM = m * objM;
				glm::vec2 d = abs(glm::vec2(length(glm::vec2(pp.z, pp.y)), pp.x)) - glm::vec2(thickness, height);
				dists.push_back(glm::min(glm::max(d.x, d.y), 0.0f) + length(glm::max(d, 0.0f)));



				//joints
				glm::mat4 jm = glm::translate(glm::vec3(x, y, z));
				glm::mat4 jM = glm::rotate(glm::radians(tempzangle), glm::vec3(0, 0, 1));
				glm::mat4 jM2 = glm::rotate(glm::radians(tempxangle), glm::vec3(1, 0, 0));
				glm::mat4 jM3 = glm::rotate(glm::radians(yangle), glm::vec3(0, 1, 0));
				jointM = jm * jM * jointM  * jM3 * jM2;

				glm::vec4 pj = jointM * glm::vec4(0, height * 2.0f, 0, 1.0);
				x = pj.x;
				y = pj.y;
				z = pj.z;;

				jointM = glm::translate(jointM, glm::vec3(0, height * 2.0f, 0));
				glm::mat4 resetm = glm::translate(glm::vec3((-1.0f) * x, (-1.0f) * y, (-1.0f) * z));
				jointM = resetm * jointM;

				float sphereDist = glm::distance(p, glm::vec3(pj.x, pj.y, pj.z)) - thickness;
				dists.push_back(sphereDist);

				//Reset angles
				tempzangle = 0.0f;
				tempxangle = 0.0f;
				tempyangle = 0.0f;
				zangle = 0.0f;
				xangle = 0.0f;
				yangle = 0.0f;
			}
			else if (sentence[i] == '+') {
				zangle += tAngle;
				tempzangle -= tAngle;
			}
			else if (sentence[i] == '-') {
				zangle -= tAngle;
				tempzangle += tAngle;
			}
			else if (sentence[i] == '/') {
				// Rotate along the z axis clockwise
				yangle += rAngle;
				tempyangle -= rAngle;
			}
			else if (sentence[i] == '\\') {
				// Rotate along the z axis counterclockwise
				yangle -= rAngle;
				tempyangle += rAngle;
			}
			else if (sentence[i] == '^') {
				// Pitch up, split it?
				xangle -= pAngle;
				tempxangle += pAngle;
			}
			else if (sentence[i] == '&') {
				// Pitch down, split it?
				xangle += pAngle;
				tempxangle -= pAngle;
				//zangle = 45.0f;
			}
			else if (sentence[i] == '[') {
				//branched = true;
				xVec.push_back(x);
				yVec.push_back(y);
				zVec.push_back(z);
				turnAngVec.push_back(xangle);
				rollAngVec.push_back(yangle);
				pitchAngVec.push_back(zangle);
				ztempAngs.push_back(tempzangle);
				xtempAngs.push_back(tempxangle);
				ytempAngs.push_back(tempyangle);
				mats.push_back(objM);
				jmats.push_back(jointM);
				//objM = glm::inverse(M2);
				//thickness -= 0.005;
				//height -= 0.001;
			}
			else if (sentence[i] == ']') {
				//branched = false;
				x = xVec[xVec.size() - 1];
				y = yVec[yVec.size() - 1];
				z = zVec[zVec.size() - 1];
				xangle = turnAngVec[turnAngVec.size() - 1];
				yangle = rollAngVec[rollAngVec.size() - 1];
				zangle = pitchAngVec[pitchAngVec.size() - 1];
				tempzangle = ztempAngs[ztempAngs.size() - 1];
				tempxangle = xtempAngs[xtempAngs.size() - 1];
				tempyangle = ytempAngs[ytempAngs.size() - 1];
				objM = mats[mats.size() - 1];
				jointM = jmats[jmats.size() - 1];

				xVec.pop_back();
				yVec.pop_back();
				zVec.pop_back();
				turnAngVec.pop_back();
				pitchAngVec.pop_back();
				rollAngVec.pop_back();
				ztempAngs.pop_back();
				xtempAngs.pop_back();
				ytempAngs.pop_back();
				mats.pop_back();
				jmats.pop_back();
				//thickness += 0.005;
				//height += 0.001;
			}

		}
		return *min_element(dists.begin(), dists.end());
	}

	void draw() {
		ofDrawSphere(position, temp);
	}

	float Iterations = 4.0f;
	float Scale = 2.0f;
	float Offset = 8.0f;
	float temp = 1.0f;
};




//************************************ Cube Class ***************************************
class Cube : public SceneObject {
public:
	Cube(glm::vec3 p, float s, ofColor diffuse = ofColor::lightGray) { position = p; side = s; diffuseColor = diffuse; }
	Cube() {}
	bool intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal) {
		return (glm::intersectRaySphere(ray.p, ray.d, position, side, point, normal));
	}
	float sdf(const glm::vec3 p) {
		glm::vec3 q = abs(p) - side;
		return MIN(MAX(q.x, max(q.y, q.z)), 0.0) + length(max(q, 0.0f));
	}

	void draw() {
		ofDrawSphere(position, side);
	}


	float side = 1.0f;
};

//************************************ WaterPool Class *********************************
class WaterPool : public SceneObject {
public:
	WaterPool(glm::vec3 p, float r, ofColor diffuse = ofColor::lightGray) { position = p; radius = r; diffuseColor = diffuse; }
	WaterPool() {}
	bool intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal) {
		return (glm::intersectRaySphere(ray.p, ray.d, position, radius, point, normal));
	}
	float sdf(glm::vec3 p, int amp, int octaves) {
		float total = 0;
		float freq = .06f;
		float ampl = 1;
		float maxVal = 0;
		float noise = 0;
		float pers = .5;
		for (int i = 0; i < 4; i++) {
			total += (ampl / 2) * glm::perlin(freq * p);
			maxVal += ampl;
			ampl *= pers;
			freq *= 2;

		}
		//noise = total / maxVal;
		noise = total;
		return  p.y - (position.y + noise);
	}
	void draw() {
		ofDrawSphere(position, radius);
	}

	float radius = 1.0;
};


//************************************** Torus Class *********************************
class Torus : public SceneObject {
public:
	float innerRadius;
	float outerRadius;
	float angle = 45.0f;
	glm::vec3 axisR = glm::vec3(1, 0, 0);

	Torus() {};
	Torus(glm::vec3 p, float l1, float l2, ofColor diffuse = ofColor::azure) {
		position = p;
		innerRadius = l1;
		outerRadius = l2;
		diffuseColor = diffuse;
	}

	//constructor with angle and axis of rotation parameters
	Torus(glm::vec3 p, float l1, float l2, float a, glm::vec3 r, ofColor diffuse = ofColor::azure) {
		position = p;
		innerRadius = l1;
		outerRadius = l2;
		diffuseColor = diffuse;
		angle = a;
		axisR = r;
	}

	//Calculate the sdf of the torus scene object while applying transformations
	float sdf(glm::vec3 p1) {
		//Transformation matrices(translate and rotate)
		glm::mat4 m = glm::translate(glm::mat4(1.0), position);
		glm::mat4 M = glm::rotate(m, glm::radians(angle), axisR);
		glm::vec4 p = glm::inverse(M) * glm::vec4(p1.x, p1.y, p1.z, 1.0);
		glm::vec2 q = glm::vec2(glm::length(glm::vec2(p.x, p.z)) - innerRadius, p.y);
		return glm::length(q) - outerRadius;
	}


	void draw() {
		//draw proxy object in 3D view
		ofDrawSphere(position, innerRadius);
	}
};



class Triangle {
public:
	int i, j, k; //indices for triangles
	int in, jn, kn; //indices for normals;
};

class Mesh : public SceneObject {
public:
	bool intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal);
	//return glm::intersectRayTriangle(ray.p, ray.d, vertices[triangle.i], vertices[triangle.j], vertices[triangle.k], bary);

	void draw() {
		for (Triangle triangle : tris) {
			ofNoFill();
			ofDrawTriangle(vertices[triangle.i], vertices[triangle.j], vertices[triangle.k]);
		}
	}
	vector<glm::vec3> vertices;
	vector<Triangle> tris;
	vector<glm::vec3> vertNormals;
};

//  General purpose plane 
//
class Plane : public SceneObject {
public:
	Plane(glm::vec3 p, glm::vec3 n, ofColor diffuse = ofColor::darkOliveGreen, float w = 50, float h = 50) {
		position = p; normal = n;
		width = w;
		height = h;
		diffuseColor = diffuse;
		if (normal == glm::vec3(0, 1, 0)) plane.rotateDeg(90, 1, 0, 0);
	}
	Plane() {
		normal = glm::vec3(0, 1, 0);
		plane.rotateDeg(90, 1, 0, 0);
	}

	bool intersect(const Ray &ray, glm::vec3 & point, glm::vec3 & normal);
	float sdf(glm::vec3  p) {
		return p.y - position.y;
	}
	glm::vec3 getNormal(const glm::vec3 &p) { return this->normal; }
	void draw() {
		plane.setPosition(position);
		plane.setWidth(width);
		plane.setHeight(height);
		plane.setResolution(4, 4);
		plane.drawWireframe();
	}
	ofPlanePrimitive plane;
	glm::vec3 normal;

	float width = 20;
	float height = 20;
};

// view plane for render camera
// 
class  ViewPlane : public Plane {
public:
	ViewPlane(glm::vec2 p0, glm::vec2 p1) { min = p0; max = p1; }

	ViewPlane() {                         // create reasonable defaults (6x4 aspect)
		min = glm::vec2(-3, -2);
		max = glm::vec2(3, 2);
		position = glm::vec3(0, 0, 5);
		normal = glm::vec3(0, 0, 1);      // viewplane currently limited to Z axis orientation
	}

	void setSize(glm::vec2 min, glm::vec2 max) { this->min = min; this->max = max; }
	float getAspect() { return width() / height(); }

	glm::vec3 toWorld(float u, float v);   //   (u, v) --> (x, y, z) [ world space ]

	void draw() {
		ofDrawRectangle(glm::vec3(min.x, min.y, position.z), width(), height());
	}


	float width() {
		return (max.x - min.x);
	}
	float height() {
		return (max.y - min.y);
	}

	// some convenience methods for returning the corners
	//
	glm::vec2 topLeft() { return glm::vec2(min.x, max.y); }
	glm::vec2 topRight() { return max; }
	glm::vec2 bottomLeft() { return min; }
	glm::vec2 bottomRight() { return glm::vec2(max.x, min.y); }

	//  To define an infinite plane, we just need a point and normal.
	//  The ViewPlane is a finite plane so we need to define the boundaries.
	//  We will define this in terms of min, max  in 2D.  
	//  (in local 2D space of the plane)
	//  ultimately, will want to locate the ViewPlane with RenderCam anywhere
	//  in the scene, so it is easier to define the View rectangle in a local'
	//  coordinate system.
	//
	glm::vec2 min, max;
};

//  render camera  - currently must be z axis aligned (we will improve this in project 4)
//
class RenderCam : public SceneObject {
public:
	RenderCam() {
		position = glm::vec3(0, 0, 12);
		aim = glm::vec3(0, 0, -1);
	}
	Ray getRay(float u, float v);
	void draw() { ofDrawBox(position, 1.0); };
	void drawFrustum() {
		view.draw();
	};

	glm::vec3 aim;
	ViewPlane view;          // The camera viewplane, this is the view that we will render 
};


//Light class with an intensity variable
class Light : public SceneObject {
public:
	float intensity = 100;
	float radius = 0.1;
	Light(glm::vec3 p, float i) {
		position = p;
		intensity = i;
	}

	Light() {}
	void draw() { ofDrawSphere(position, radius); };
};

//Spotlight class. Inherits from Light and has variables for angles that constraint
// the light into a cone with some falloff light to provide smoother edges.
class Spotlight : public Light {
public:
	glm::vec3 direction;
	float lightAngle;
	float falloffAngle;

	Spotlight(glm::vec3 d, float angle, float foAngle) {
		direction = d;
		lightAngle = glm::radians(angle);
		falloffAngle = glm::radians(foAngle);
	}

	Spotlight() {
		direction = glm::vec3(0.0f, -1.0f, 0.0f);
		lightAngle = glm::radians(10.0);
		falloffAngle = lightAngle - .1;
	}
	float falloff(float spotAngle);
};

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void dragEvent(ofDragInfo dragInfo);
	void triRayTrace();
	bool rayMarch(Ray r, glm::vec3 &p);
	void RayMarching();
	glm::vec3 getNormal(const glm::vec3 &p, int i);
	glm::vec3 getNormalRM(const glm::vec3 &p);
	float sceneSDF(glm::vec3 p);
	float opRep(const glm::vec3 p, const glm::vec3 c, SceneObject &s);
	float opRound(const glm::vec3 p, SceneObject &s, float rad);
	ofColor lambert(const glm::vec3 &p, const glm::vec3 &norm, const ofColor diffuse);
	ofColor phong(const glm::vec3 &p, const glm::vec3 &norm, const ofColor diffuse, const ofColor specular, float power);
	bool inShadow(const Ray &r);
	bool FileLoader(char * path);

	bool bHide = true;
	bool bShowImage = false;

	ofEasyCam  mainCam;
	ofCamera sideCam;
	ofCamera previewCam;
	ofCamera  *theCam;    // set to current camera either mainCam or sideCam

	RenderCam renderCam;
	ofImage image;
	ofImage texture;
	ofImage sphereTexture;

	vector<SceneObject *> scene;
	vector<Light *> lights;
	float Power = 10;

	int imageWidth = 1200;
	int imageHeight = 800;

	//scene objects and lights
	Sphere sphere1;
	Sphere sphere2;
	Sphere sphere3;
	Cube cube1;
	Fractal f1;
	WaterPool wp1;
	Plane plane1;
	Torus torus1;
	Torus torus2;
	Light light1;
	Light light2;
	Light light3;
	Light light4;
	Spotlight sp1;

	float nearestDistance;


	Mesh mesh;
	Triangle tri;
	Triangle tri2;
	Triangle tri3;
	Triangle tri4;

	glm::vec3 v0;
	glm::vec3 v1;
	glm::vec3 v2;
	glm::vec3 v3;
	glm::vec3 v4;
	glm::vec3 v5;


	int count;
	int indexHit;

};
