#pragma once

#include "ofMain.h"
#include <iostream>
#include <fstream>
#include <unordered_map>
#include "../Application/Definitions.h"
#include "../Rendering/RenderCam.h"
#include "../Rendering/Renderers/RayTracer.h"
#include "../Rendering/Renderers/RayMarcher.h"
#include "../Rendering/Acceleration/BVH.h"
#include "SceneObject.h"
#include "SceneObjects/Sphere.h"
#include "SceneObjects/Cube.h"
#include "SceneObjects/Plane.h"
#include "SceneObjects/Cylinder.h"
#include "SceneObjects/Cone.h"
#include "SceneObjects/Torus.h"
#include "SceneObjects/LSystem.h"
#include "SceneObjects/Mesh.h"
#include "SceneObjects/Lights/Light.h"
#include "SceneObjects/Lights/SpotLight.h"
#include "SceneObjects/Lights/AreaLight.h"

class Scene {
private:
	SceneObject *sphere1;
	SceneObject *sphere2;

	RenderCam renderCam;
	Renderer *rayTracer;
	Renderer *rayMarcher;
	ofImage renderedImage;
	int renderSamples = 1;

	unordered_map<string, int> objectCount;
	bool renderFinished = false;
	bool showImage;
	bool showBVH;

	SceneObject *selected = nullptr;
	vector<SceneObject *> objects;
	vector<Light *> lights;
	BVH bvh;

public:
	Scene(RenderCam &cam) : renderCam(cam) {};
	~Scene() {};

	void setup();
	void update();
	void draw();

	// Rendering
	void HandleRayTrace();
	void HandleRayMarch();
	void HandleSaveImage();
	void HandleRename();
	void HandleDelete();
	void HandleClearScene();


	// Load Texture file from given path
	void HandleLoadTexture();

	// Remove texture from a given scene object
	void HandleRemoveTexture();

	void buildBVH();

	// 3D Modeling
	void AddObject(SceneObject *object);
	void AddLight(Light *light);
	void AddSphere();
	void AddCube();
	void AddPlane();
	void AddCylinder();
	void AddCone();
	void AddTorus();
	void AddLSystem();
	void AddMesh();
	void AddPointLight();
	void AddSpotLight();
	void AddAreaLight();

	// Load .obj file from given path and create Mesh object
	void OBJFileLoader(const char *path);


	// Setters and Getters
	void setObjectSelected(SceneObject *object) { selected = object; }
	void setShowBVH(bool show) { showBVH = show; }
	void setShowImage(bool show) { showImage = show; }
	void setSamples(int renderSamples) { this->renderSamples = renderSamples; }
	void setResolution(int width, int height) { 
		rayTracer->setImageResolution(width, height);
		rayMarcher->setImageResolution(width, height);
	}
	SceneObject *getObjectSelected() const { return selected; }
	vector<SceneObject *> getObjects() const { return objects; }
	vector<Light *> getLights() const { return lights; }
	BVH getBVH() const { return bvh; }
	RenderCam *getRenderCam() { return &renderCam; }
	Renderer *getCurrentRenderer() const {
		return (rayTracer->IsRendering()) ? rayTracer : rayMarcher;
	}


};