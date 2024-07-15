#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "Tab.h"
#include "../Scene/Scene.h"

class UI {
private:
	// Reference to the 3D scene
	Scene *sceneRef;

	// Custom GUI Tabs
	vector<Tab> tabs;
	int currentTab;

	// Main Interfaces
	ofxGuiGroup objectGUI;
	ofxGuiGroup modelingGUI;
	ofxGuiGroup renderingGUI;

	// Sub GUI sections
	ofxGuiGroup group_modelingOptions;
	ofxGuiGroup group_renderingOptions;
	ofxGuiGroup group_create;
	ofxGuiGroup group_objects;
	ofxGuiGroup group_lights;
	ofxGuiGroup group_objectRotation;
	ofxGuiGroup group_objectMaterial;
	ofxGuiGroup group_objectTexture;

	// GUI variables
	ofxVec3Slider vec_slider_position;
	ofxVec3Slider vec_slider_rotation;
	ofxVec3Slider vec_slider_scale;
	ofxFloatSlider f_slider_radius;
	ofxFloatSlider f_slider_majorRadius;
	ofxFloatSlider f_slider_height;
	ofxFloatSlider f_slider_width;
	ofxFloatSlider f_slider_angle;
	ofxFloatSlider f_slider_lightIntensity;
	ofxFloatSlider f_slider_lightWidth;
	ofxFloatSlider f_slider_lightFalloff;
	ofxIntSlider i_slider_angleX;
	ofxIntSlider i_slider_angleY;
	ofxIntSlider i_slider_angleZ;
	ofxIntSlider i_slider_renderSamples;
	ofxIntSlider i_slider_resolutionWidth;
	ofxIntSlider i_slider_resolutionHeight;
	ofxIntSlider i_slider_lightSamples;
	ofxIntSlider i_slider_LsystemIterations;
	ofxIntSlider i_slider_uvTile;
	ofxTextField textfield_LsystemAxiom;
	ofxTextField textfield_LsystemRule1;
	ofxTextField textfield_LsystemRule2;
	ofxTextField textfield_LsystemRule3;
	ofxTextField textfield_LsystemRule4;
	ofxColorSlider slider_objectColor;

	// GUI buttons
	ofxButton button_rayTrace;
	ofxButton button_rayMarch;
	ofxButton button_rename;
	ofxButton button_save;
	ofxButton button_delete;
	ofxButton button_clear;
	ofxButton button_addSphere;
	ofxButton button_addCube;
	ofxButton button_addPlane;
	ofxButton button_addCylinder;
	ofxButton button_addCone;
	ofxButton button_addTorus;
	ofxButton button_addLsystem;
	ofxButton button_addMesh;
	ofxButton button_addPointLight;
	ofxButton button_addSpotLight;
	ofxButton button_addAreaLight;

	// Object GUI buttons
	ofParameter<void> button_loadTexture = { " Load Texture file" };
	ofParameter<void> button_removeTexture = { " Clear Texture" };

	// GUI Toggles
	ofxToggle toggle_objectMode;
	ofxToggle toggle_editMode;
	ofxToggle toggle_image;
	ofxToggle toggle_grid;
	ofxToggle toggle_renderView;
	ofxToggle toggle_bvh;
	ofxToggle toggle_materialMatte;
	ofxToggle toggle_materialMirror;
	ofxToggle toggle_materialGlass;
	ofxToggle toggle_materialReflective;

	// Flags for displaying object's GUI and 3D scene grid
	bool showObjectGUI = false;
	bool showGrid = true;

	// For indicating what material is currently selected on the Object's GUI
	string selectedMaterial = "Matte";


public:
	UI(Scene *scene) : sceneRef(scene), currentTab(0) {};
	~UI() {};
	
	void setup();
	void update();
	void draw();
	void drawGrid();

	// Update 3D scene object based on UI values
	void updateObject(SceneObject *object);

	// Update GUI values based on 3D scene object
	void updateGUI(SceneObject *object);

	// Detect mouse clicks on UI tabs
	void mousePressed(int x, int y, int button);

	// Update Object GUI material toggles
	void setMaterialToggle(const string &mat, bool &value);
	void setMatte(bool &value) { setMaterialToggle("Matte", value); }
	void setMirror(bool &value) { setMaterialToggle("Mirror", value); }
	void setGlass(bool &value) { setMaterialToggle("Glass", value); }
	void setReflective(bool &value) { setMaterialToggle("Reflective", value); }


	// Handle GUI toggles
	void toggleBVH() { toggle_bvh = !toggle_bvh; }
	void toggleImage() { toggle_image = !toggle_image; }
	void toggleRenderView() { toggle_renderView = !toggle_renderView; }
	void toggleGrid() { toggle_grid = !toggle_grid; }
	bool getToggleRenderCam() { return static_cast<bool>(toggle_renderView); }
};