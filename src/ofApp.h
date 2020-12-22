#pragma once

#include "ofMain.h"
#include "Definitions.h"
#include "Scene/SceneObject.h"
#include "Scene/ViewPlane.h"
#include "Scene/RenderCam.h"
#include "Scene/SceneObjects/Sphere.h"
#include "Scene/SceneObjects/LSystem.h"
#include "Scene/SceneObjects/Cube.h"
#include "Scene/SceneObjects/WaterPool.h"
#include "Scene/SceneObjects/Torus.h"
#include "Scene/SceneObjects/Mesh.h"
#include "Scene/SceneObjects/Plane.h"
#include "Scene/SceneObjects/Light.h"
#include "Scene/SceneObjects/Lights/SpotLight.h"
#include <typeinfo>

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
	LSystem f1;
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
