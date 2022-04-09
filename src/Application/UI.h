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
 	void setReflective(bool & value);  
	void LoadTextureFile();

	// Main GUIs
	ofxGuiGroup objectGUI;
	ofxGuiGroup sceneGUI;
	bool hideGUI = true;
	bool hideGrid = false;

	Scene *scene;

	// GUI variables
	ofxGuiGroup group_scene, group_create, group_objects, group_lights, group_rotation, group_material, group_texture, group_mat_variables;
	ofxVec3Slider slider_location, slider_rotation, slider_scale;
	ofxFloatSlider gui_value1, gui_value2, gui_value3, gui_value4, gui_reflect;
	ofxIntSlider gui_ivalue1, gui_uvTile, gui_angleX, gui_angleY, gui_angleZ, gui_samples, gui_area_samples;
	ofxTextField gui_axiom, gui_rule1, gui_rule2, gui_rule3, gui_rule4;
	ofxLabel label_material;
	ofxColorSlider color;
		
	ofParameter<void> button_texture = { " Load Texture file" };
	ofParameter<void> button_remove_texture = { " Clear Texture" };

	// SceneObject Buttons
	ofxButton button_rayTrace, button_rayMarch, button_rename_selected, button_saveImage, 
		button_delete, button_clear_scene, button_sphere, button_cube, button_plane, 
		button_cylinder, button_cone, button_torus, button_mesh, button_lsystem, 
		button_point_light, button_spot_light, button_area_light;

	ofxToggle toggle_image, toggle_grid, toggle_render_cam, toggle_matte, toggle_mirror,
		toggle_glass, toggle_reflective, toggle_bvh;

	string selectedMaterial = "Matte";
	int samples = 1;
};