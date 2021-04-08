#pragma once

#include "ofMain.h"
#include "../Application/Definitions.h"
#include "../Rendering/RayTracer.h"
#include "../Rendering/RayMarcher.h"
#include "../Rendering/ViewPlane.h"
#include "../Rendering/RenderCam.h"
#include "SceneObject.h"
#include "SceneObjects/Sphere.h"
#include "SceneObjects/Cube.h"
#include "SceneObjects/Plane.h"
#include "SceneObjects/Cylinder.h"
#include "SceneObjects/Cone.h"
#include "SceneObjects/Torus.h"
#include "SceneObjects/WaterPool.h"
#include "SceneObjects/Mesh.h"
#include "SceneObjects/LSystem.h"
#include "SceneObjects/Lights/Light.h"
#include "SceneObjects/Lights/SpotLight.h"
#include "SceneObjects/Lights/AreaLight.h"

class Scene {

public:

	Scene() {};
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
	bool FileLoader(const char *path);

	RayTracer rayTracer;
	RayMarcher rayMarcher;

	RenderCam renderCam;
	ofImage image;
	ofImage texture;
	ofImage sphereTexture;

	vector<SceneObject *> selected;
	vector<SceneObject *> objects;
	vector<Light *> lights;
	float Power = 10;
	float nearestDistance;

	int imageWidth = 1200;
	int imageHeight = 800;
	//int imageWidth = 90;
	//int imageHeight = 60;
	//int imageWidth = 150;
	//int imageHeight = 100;


	int count;
	int indexHit;
	int sphereCount, cubeCount, planeCount, cylinderCount, coneCount, torusCount,
		meshCount, lsystemCount, waterpoolCount, pointlightCount, spotlightCount, arealightCount;
	bool renderFinished;

	int samples;
};