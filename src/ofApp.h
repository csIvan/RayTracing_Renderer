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
	bool FileLoader(char * path);

	bool bHide = true;
	bool bShowImage = false;

	ofEasyCam  mainCam;
	ofCamera sideCam;
	ofCamera previewCam;
	ofCamera  *theCam;    // set to current camera either mainCam or sideCam

	RayTracer rayTracer;
	RayMarcher rayMarcher;

	ofxVec3Slider slider_location;
	ofxVec3Slider slider_rotation;
	ofxVec3Slider slider_scale;
	ofxFloatSlider gui_radius;
	ofxColorSlider color;
	ofxGuiGroup group_create;
	ofxGuiGroup group_lights;
	ofxLabel label_sphere;
	ofxLabel label_cube;
	ofxLabel label_plane;
	ofxLabel label_cylinder;
	ofxLabel label_cone;
	ofxLabel label_torus;
	ofxLabel label_mesh;
	ofxLabel label_lsystem;
	ofxLabel label_point_light;
	ofxLabel label_spot_light;
	ofxLabel label_area_light;

	ofxGuiGroup objectGUI;
	ofxGuiGroup sceneGUI;
	ofxPanel p;
	bool hideGUI = true;


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
	SpotLight sp1;

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
