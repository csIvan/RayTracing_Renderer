#pragma once

#include "ofMain.h"
#include "UIManager.h"
#include "Definitions.h"
#include "../Rendering/RayTracer.h"
#include "../Rendering/RayMarcher.h"
#include "../Rendering/ViewPlane.h"
#include "../Rendering/RenderCam.h"
#include "../Scene/SceneObject.h"
#include "../Scene/SceneObjects/Sphere.h"
#include "../Scene/SceneObjects/Cube.h"
#include "../Scene/SceneObjects/Plane.h"
#include "../Scene/SceneObjects/Cylinder.h"
#include "../Scene/SceneObjects/Cone.h"
#include "../Scene/SceneObjects/Torus.h"
#include "../Scene/SceneObjects/WaterPool.h"
#include "../Scene/SceneObjects/Mesh.h"
#include "../Scene/SceneObjects/LSystem.h"
#include "../Scene/SceneObjects/Lights/Light.h"
#include "../Scene/SceneObjects/Lights/SpotLight.h"
#include "../Scene/SceneObjects/Lights/AreaLight.h"
#include <typeinfo>
#include <chrono>

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();

	void handleRayTrace();
	void handleRayMarch();
	void handleSaveImage();
	void handleDelete();
	void addObject(SceneObject *s);
	void addLight(Light *light);
	void addSphere();
	void addCube();
	void addPlane(); 
	void addCylinder();
	void addCone();
	void addTorus();  
	void addMesh();
	void addLSystem();
	void addWaterPool();
	void addPointLight();
	void addSpotLight();
	void addAreaLight();
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void keyPressed(int key);
	void dragEvent(ofDragInfo dragInfo);
	bool FileLoader(const char * path);

	bool bHide = true;
	bool bShowImage = false;

	ofEasyCam  mainCam;
	ofCamera sideCam;
	ofCamera previewCam;
	ofCamera  *theCam;    // set to current camera either mainCam or sideCam

	RayTracer rayTracer;
	RayMarcher rayMarcher;

	UIManager ui;

	RenderCam renderCam;
	ofImage image;
	ofImage texture;
	ofImage sphereTexture;

	vector<SceneObject *> selected;
	vector<SceneObject *> scene;
	vector<Light *> lights;
	float Power = 10;

	int imageWidth = 1200;
	int imageHeight = 800;
	//int imageWidth = 90;
	//int imageHeight = 60;
	//int imageWidth = 150;
	//int imageHeight = 100;

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
	SpotLight sp1;

	float nearestDistance;
	ofLight lightScene;
	ofMaterial material;
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
	int mouseX, mouseY;
	int sphereCount, cubeCount, planeCount, cylinderCount, coneCount, torusCount, 
		meshCount, lsystemCount, waterpoolCount, pointlightCount, spotlightCount, arealightCount;
	bool renderFinished;
};
