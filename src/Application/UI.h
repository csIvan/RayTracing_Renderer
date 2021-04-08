#pragma once

#include "ofxGui.h"
#include "../Scene/Scene.h"

class UI {

public:

	UI() {};
	void setup(Scene *s);
	void update();
	void draw();
	void drawGrid();

	void updateSelected(SceneObject *s);
	void updateGUI(SceneObject *s);
	void updateMaterial();

	void setMatte(bool & value);
	void setMirror(bool & value);
	void setGlass(bool & value);
	void setMetal(bool & value);

	// Main GUIs
	ofxGuiGroup objectGUI;
	ofxGuiGroup sceneGUI;
	bool hideGUI = true;
	bool hideGrid = false;

	Scene *scene;

	// GUI variables
	ofxVec3Slider slider_location;
	ofxVec3Slider slider_rotation;
	ofxVec3Slider slider_scale;
	ofxFloatSlider gui_value1, gui_value2, gui_value3, gui_reflect;
	ofxIntSlider gui_ivalue1;
	ofxTextField gui_axiom, gui_rule1, gui_rule2, gui_rule3, gui_rule4;
	ofxIntSlider gui_angle1;
	ofxIntSlider gui_angle2;
	ofxIntSlider gui_samples;
	ofxIntSlider gui_area_samples;
	ofxColorSlider color;
	ofxGuiGroup group_scene;
	ofxGuiGroup group_create;
	ofxGuiGroup group_objects;
	ofxGuiGroup group_lights;
	ofxGuiGroup group_rotation;
	ofxGuiGroup group_material;
	ofxIntSlider gui_angleX;
	ofxIntSlider gui_angleY;
	ofxIntSlider gui_angleZ;

	// GUI functions
	ofxButton button_rayTrace, button_rayMarch, button_saveImage, button_delete;
	ofxToggle toggle_image, toggle_grid, toggle_render_cam, toggle_matte, toggle_mirror,
		toggle_glass, toggle_metal;;

	ofxLabel label_material;

	// SceneObject Buttons
	ofxButton button_sphere, button_cube, button_plane, button_cylinder, button_cone, button_torus,
		button_mesh, button_lsystem, button_point_light, button_spot_light, button_area_light;

	int samples = 1;

	string selectedMaterial = "Matte";
};