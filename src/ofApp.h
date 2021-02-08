#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "Definitions.h"
#include "Rendering/RayTracer.h"
#include "Rendering/RayMarcher.h"
#include "Scene/SceneObject.h"
#include "Scene/ViewPlane.h"
#include "Scene/RenderCam.h"
#include "Scene/SceneObjects/Sphere.h"
#include "Scene/SceneObjects/Cube.h"
#include "Scene/SceneObjects/Plane.h"
#include "Scene/SceneObjects/Cylinder.h"
#include "Scene/SceneObjects/Cone.h"
#include "Scene/SceneObjects/Torus.h"
#include "Scene/SceneObjects/WaterPool.h"
#include "Scene/SceneObjects/Mesh.h"
#include "Scene/SceneObjects/LSystem.h"
#include "Scene/SceneObjects/Lights/Light.h"
#include "Scene/SceneObjects/Lights/SpotLight.h"
#include <typeinfo>
#include <chrono>

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();

	void updateSelected(SceneObject *s);
	void updateGUI(SceneObject *s);

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


	// Main GUIs
	ofxGuiGroup objectGUI;
	ofxGuiGroup sceneGUI;
	bool hideGUI = true;
	bool hideGrid = false;


	// GUI variables
	ofxVec3Slider slider_location;
	ofxVec3Slider slider_rotation;
	ofxVec3Slider slider_scale;
	ofxFloatSlider gui_value1, gui_value2, gui_value3;
	ofxIntSlider gui_ivalue1;
	ofxTextField gui_axiom, gui_rule1, gui_rule2, gui_rule3, gui_rule4;
	ofxIntSlider gui_angle1;
	ofxIntSlider gui_angle2;
	ofxColorSlider color;
	ofxGuiGroup group_scene;
	ofxGuiGroup group_create;
	ofxGuiGroup group_objects;
	ofxGuiGroup group_lights;
	ofxGuiGroup group_rotation;
	ofxIntSlider gui_angleX;
	ofxIntSlider gui_angleY;
	ofxIntSlider gui_angleZ;

	// GUI functions
	ofxButton button_rayTrace, button_rayMarch, button_saveImage, button_delete;
	ofxToggle toggle_image, toggle_grid;

	// SceneObject Buttons
	ofxButton button_sphere, button_cube, button_plane, button_cylinder, button_cone, button_torus,
		button_mesh, button_lsystem, button_point_light, button_spot_light, button_area_light;

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
		meshCount, lsystemCount, waterpoolCount, pointlightCount, spotlightCount;
	bool renderFinished;
};
