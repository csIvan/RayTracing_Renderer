#include "UI.h"


void UI::setup(Scene *s) {
	scene = s;

	// Scene Button Listeners
	button_rayTrace.addListener(scene, &Scene::handleRayTrace);
	button_rayMarch.addListener(scene, &Scene::handleRayMarch);
	button_saveImage.addListener(scene, &Scene::handleSaveImage);
	button_rename_selected.addListener(scene, &Scene::handleRename);
	button_delete.addListener(scene, &Scene::handleDelete);
	button_clear_scene.addListener(scene, &Scene::handleClearScene);
	button_sphere.addListener(scene, &Scene::addSphere);
	button_cube.addListener(scene, &Scene::addCube);
	button_plane.addListener(scene, &Scene::addPlane);
	button_cylinder.addListener(scene, &Scene::addCylinder);
	button_cone.addListener(scene, &Scene::addCone);
	button_torus.addListener(scene, &Scene::addTorus);
	button_mesh.addListener(scene, &Scene::addMesh);
	button_lsystem.addListener(scene, &Scene::addLSystem);
	button_point_light.addListener(scene, &Scene::addPointLight);
	button_spot_light.addListener(scene, &Scene::addSpotLight);
	button_area_light.addListener(scene, &Scene::addAreaLight);
	button_remove_texture.addListener(scene, &Scene::handleRemoveTexture);
	button_texture.addListener(this, &UI::LoadTextureFile);
	toggle_matte.addListener(this, &UI::setMatte);
	toggle_mirror.addListener(this, &UI::setMirror);
	toggle_glass.addListener(this, &UI::setGlass);
	toggle_reflective.addListener(this, &UI::setReflective);


	// Setup Scene User Interface
	sceneGUI.setHeaderBackgroundColor(ofColor(50, 50, 50));
	sceneGUI.setDefaultHeight(30);
	sceneGUI.setDefaultWidth(240);
	sceneGUI.loadFont("fonts/Verdana.ttf", 10);
	sceneGUI.setup("Render");
	sceneGUI.setBorderColor(ofColor::black);
	group_create.setBorderColor(ofColor(25, 25, 25));
	group_create.setHeaderBackgroundColor(ofColor(50, 50, 50));
	button_rayTrace.setTextColor(ofColor(0,153, 76));
	button_rayMarch.setTextColor(ofColor(111, 169, 255));
	sceneGUI.add(button_rayTrace.setup(" RayTrace"));
	sceneGUI.add(button_rayMarch.setup(" RayMarch"));
	sceneGUI.add(gui_samples.setup("Samples", scene->samples, 1, 64));
	sceneGUI.add(group_create.setup("Add"));
	group_objects.setBorderColor(ofColor(25, 25, 25));
	group_objects.setHeaderBackgroundColor(ofColor::black);

	group_create.add(group_objects.setup(" Objects"));
	group_objects.add(button_sphere.setup(" Sphere					     (a)"));
	group_objects.add(button_cube.setup(" Cube					        (0)"));
	group_objects.add(button_plane.setup(" Plane				  	     (1)"));
	group_objects.add(button_cylinder.setup(" Cylinder				  	  (2)"));
	group_objects.add(button_cone.setup(" Cone					        (3)"));
	group_objects.add(button_torus.setup(" Torus					       (4)"));
	group_objects.add(button_mesh.setup(" Mesh (.obj)				   (5)"));
	group_objects.add(button_lsystem.setup(" LSystem				       (6)"));

	group_create.add(group_lights.setup(" Lights"));;
	group_lights.setHeaderBackgroundColor(ofColor::black);
	group_lights.setBorderColor(ofColor(20, 20, 20));
	group_lights.add(button_point_light.setup(" Point Light				     (7)"));
	group_lights.add(button_spot_light.setup(" Spot Light				     (8)"));
	group_lights.add(button_area_light.setup(" Area Light				     (9)"));

	group_scene.setHeaderBackgroundColor(ofColor(50, 50, 50));
	group_scene.setBorderColor(ofColor(25, 25, 25));
	sceneGUI.add(group_scene.setup("Scene"));
	toggle_bvh.setFillColor(ofColor(0, 153, 76));
	button_rename_selected.setTextColor(ofColor(111, 169, 255));
	button_saveImage.setTextColor(ofColor(255, 192, 81));
	toggle_image.setFillColor(ofColor(108, 176, 94));
	toggle_grid.setFillColor(ofColor(94, 132, 176));
	toggle_render_cam.setFillColor(ofColor(123, 60, 230));
	button_delete.setTextColor(ofColor(255, 63, 63));
	button_clear_scene.setTextColor(ofColor(255, 63, 63));
	group_scene.add(button_rename_selected.setup(" Rename Selected Object  (n)"));
	group_scene.add(button_saveImage.setup(" Save Image			  	 (s)"));
	group_scene.add(button_clear_scene.setup(" Clear Scene			  	 (d)"));
	group_scene.add(toggle_bvh.setup(" Show BVH				     (b)", true));
	group_scene.add(toggle_grid.setup(" Show Grid			 	    (g)", true));
	group_scene.add(toggle_image.setup(" Show Render	             (r)", true));
	group_scene.add(toggle_render_cam.setup(" Render Cam View	       (.)", false));
	group_scene.add(button_delete.setup(" Delete Selected Object    (x)"));
	

	objectGUI.setup("Sphere");
	objectGUI.setBorderColor(ofColor::black);
}

void UI::update() {
	sceneGUI.maximize();	// Always maximixed

	if ((int)gui_samples < 3)
		gui_samples = 1;
	else if ((int)gui_samples >= 3 && (int)gui_samples < 10)
		gui_samples = 4;
	else if ((int)gui_samples >= 10 && (int)gui_samples < 25)
		gui_samples = 16;
	else if ((int)gui_samples >= 25 && (int)gui_samples < 50)
		gui_samples = 36;
	else
		gui_samples = 64;

	scene->samples = (int)gui_samples;

	if (scene->selected.size() > 0) {
		label_material = ofToString(selectedMaterial);
		updateSelected(scene->selected[0]);
		objectGUI.maximize();
	}
	else {
		hideGUI = true;
	}

	scene->showBVH = (bool)toggle_bvh;

}

void UI::draw() {
	sceneGUI.draw();
	if (!hideGUI) {
		objectGUI.setPosition(ofGetWidth() - sceneGUI.getWidth() - 10, 10);
		for (int i = 0; i < objectGUI.getNumControls(); i++) {
			string objectName = objectGUI.getControlNames()[i];
			objectGUI.getControl(objectName)->setPosition(glm::vec3(objectGUI.getPosition().x, objectGUI.getControl(objectName)->getPosition().y, 0));
		}
		objectGUI.draw();
	}


}

void UI::drawGrid() {
	if ((bool)toggle_grid) {
		ofSetColor(ofColor(111, 169, 255));
		ofDrawLine(glm::vec3(0, 0, GRID_LINES), glm::vec3(0, 0, -GRID_LINES));
		ofSetColor(ofColor(255, 81, 81));
		ofDrawLine(glm::vec3(GRID_LINES, 0, 0), glm::vec3(-GRID_LINES, 0, 0));
		ofSetColor(ofColor(178, 178, 178, 50));
		for (int i = -GRID_LINES; i < GRID_LINES; i++) {
			ofDrawLine(glm::vec3(GRID_LINES, 0, i), glm::vec3(-GRID_LINES, 0, i));
			ofDrawLine(glm::vec3(i, 0, GRID_LINES), glm::vec3(i, 0, -GRID_LINES));
		}
	}
}

void UI::updateMaterial() {
	if (selectedMaterial == "Matte") {
		group_material.add(toggle_matte.setup(" Matte", true));
		group_material.add(toggle_mirror.setup(" Mirror", false));
		group_material.add(toggle_glass.setup(" Glass", false));
		group_material.add(toggle_reflective.setup(" Reflective", false));
	}
	else if (selectedMaterial == "Mirror") {
		group_material.add(toggle_matte.setup(" Matte", false));
		group_material.add(toggle_mirror.setup(" Mirror", true));
		group_material.add(toggle_glass.setup(" Glass", false));
		group_material.add(toggle_reflective.setup(" Reflective", false));
	}
	else if (selectedMaterial == "Glass") {
		group_material.add(toggle_matte.setup(" Matte", false));
		group_material.add(toggle_mirror.setup(" Mirror", false));
		group_material.add(toggle_glass.setup(" Glass", true));
		group_material.add(toggle_reflective.setup(" Reflective", false));
	}
	else if (selectedMaterial == "Reflective") {
		group_material.add(toggle_matte.setup(" Matte", false));
		group_material.add(toggle_mirror.setup(" Mirror", false));
		group_material.add(toggle_glass.setup(" Glass", false));
		group_material.add(toggle_reflective.setup(" Reflective", true));
	}
}

// Use the interface to manipulate scene object attributes
void UI::updateSelected(SceneObject *s) {
	s->objMaterial.setString(selectedMaterial);
	s->position = static_cast<glm::vec3>(slider_location);
	s->rotation.x = static_cast<int>(gui_angleX);
	s->rotation.y = static_cast<int>(gui_angleY);
	s->rotation.z = static_cast<int>(gui_angleZ);
	s->setRotate();
	s->objMaterial.diffuseColor = (ofColor)color;
	s->objTexture.uvTileFactor = gui_uvTile;

	if (dynamic_cast<Sphere*>(s) != nullptr) {
		Sphere *sphereSelected = (Sphere*)s;
		sphereSelected->radius = (float)gui_value1;
	}
	else if (dynamic_cast<Cube*>(s) != nullptr) {
		Cube *cubeSelected = (Cube*)s;
		cubeSelected->side = (float)gui_value1;
	}
	else if (dynamic_cast<Plane*>(s) != nullptr) {
		Plane *planeSelected = (Plane*)s;
		planeSelected->width = (float)gui_value1;
		planeSelected->height = (float)gui_value2;
	}
	else if (dynamic_cast<Cylinder*>(s) != nullptr) {
		Cylinder *cylinderSelected = (Cylinder*)s;
		cylinderSelected->height = (float)gui_value1;
		cylinderSelected->radius = (float)gui_value2;
	}
	else if (dynamic_cast<Cone*>(s) != nullptr) {
		Cone *coneSelected = (Cone*)s;
		coneSelected->height = (float)gui_value1;
		coneSelected->radius = (float)gui_value2;
	}
	else if (dynamic_cast<Torus*>(s) != nullptr) {
		Torus *torusSelected = (Torus*)s;
		torusSelected->R = (float)gui_value1;
		torusSelected->r = (float)gui_value2;
	}
	else if (dynamic_cast<LSystem*>(s) != nullptr) {
		LSystem *lsystemSelected = (LSystem*)s;
		lsystemSelected->iterations = (int)gui_ivalue1;
		lsystemSelected->angle = (float)gui_value1;
		lsystemSelected->tubeRadius = (float)gui_value2;
		lsystemSelected->tubeHeight = (float)gui_value3;
		lsystemSelected->axiom = (string)gui_axiom;
		lsystemSelected->rule1.b = (string)gui_rule1;
		lsystemSelected->rule2.b = (string)gui_rule2;
		lsystemSelected->rule3.b = (string)gui_rule3;
		lsystemSelected->generate();
	}
	else if (dynamic_cast<Light*>(s) != nullptr) {
		Light *lightSelected = (Light*)s;
		lightSelected->intensity = (float)gui_value1;
		if (dynamic_cast<SpotLight*>(s) != nullptr) {
			SpotLight *spotLightSelected = (SpotLight*)s;
			spotLightSelected->heightRef = (float)gui_value2;
			spotLightSelected->totalWidth = (float)gui_value3;
			spotLightSelected->falloffStart = (float)gui_value4;
		}
		else if (dynamic_cast<AreaLight*>(s) != nullptr) {
			AreaLight *areaLightSelected = (AreaLight*)s;
		}
	}

	s->setBounds();

	scene->bvh.create(scene->objects);

	// Don't need scale for lights
	if (dynamic_cast<Light*>(s) == nullptr) {
		s->scale = static_cast<glm::vec3>(slider_scale);
	}

}

// Load scene object attributes to interface
void UI::updateGUI(SceneObject *s) {
	objectGUI.setDefaultHeight(24);
	string buffer = "";
	for (int i = 0; i < (20 - (s->objName.length() / 2)); i++) {
		buffer += " ";
	}
	objectGUI.setName(buffer + s->objName);
	if (dynamic_cast<Sphere*>(s) != nullptr) {
		Sphere *sphereSelected = (Sphere*)s;
		objectGUI.add(gui_value1.setup("Radius", sphereSelected->radius, 0.2, 3));
	}
	else if (dynamic_cast<Cube*>(s) != nullptr) {
		Cube *cubeSelected = (Cube*)s;
		objectGUI.add(gui_value1.setup("Length", cubeSelected->side, 0.2, 3));
	}
	else if (dynamic_cast<Plane*>(s) != nullptr) {
		Plane *planeSelected = (Plane*)s;
		objectGUI.add(gui_value1.setup("Width", planeSelected->width, 0.2, 40));
		objectGUI.add(gui_value2.setup("Height", planeSelected->height, 0.2, 40));
	}
	else if (dynamic_cast<Cylinder*>(s) != nullptr) {
		Cylinder *cylinderSelected = (Cylinder*)s;
		objectGUI.add(gui_value1.setup("Height", cylinderSelected->height, 1, 5));
		objectGUI.add(gui_value2.setup("Radius", cylinderSelected->radius, 0.2, 3));
	}
	else if (dynamic_cast<Cone*>(s) != nullptr) {
		Cone *coneSelected = (Cone*)s;
		objectGUI.add(gui_value1.setup("Height", coneSelected->height, 1, 5));
		objectGUI.add(gui_value2.setup("Radius", coneSelected->radius, 0.2, 3));
	}
	else if (dynamic_cast<Torus*>(s) != nullptr) {
		Torus *torusSelected = (Torus*)s;
		objectGUI.add(gui_value1.setup("Major Radius", torusSelected->R, 0.5, 5));
		objectGUI.add(gui_value2.setup("Minor Radius", torusSelected->r, 0.1, 5));
	}
	else if (dynamic_cast<LSystem*>(s) != nullptr) {
		LSystem *lsystemSelected = (LSystem*)s;
		objectGUI.add(gui_ivalue1.setup("Iterations", lsystemSelected->iterations, 1, 10));
		objectGUI.add(gui_value1.setup("Angle", lsystemSelected->angle, -180, 180));
		objectGUI.add(gui_value2.setup("Tube Radius", lsystemSelected->tubeRadius, 0.02, 2));
		objectGUI.add(gui_value3.setup("Tube Height", lsystemSelected->tubeHeight, 0.1, 2));
		objectGUI.add(gui_axiom.setup("Axiom", lsystemSelected->axiom));
		objectGUI.add(gui_rule1.setup("F", lsystemSelected->rule1.b));
		objectGUI.add(gui_rule2.setup("A", lsystemSelected->rule2.b));
		objectGUI.add(gui_rule3.setup("B", lsystemSelected->rule3.b));
		//objectGUI.add(gui_value1.setup("Major Radius", torusSelected->R, 0.5, 5));
	}
	else if (dynamic_cast<Light*>(s) != nullptr) {
		Light *lightSelected = (Light*)s;
		objectGUI.add(gui_value1.setup("Intensity", lightSelected->intensity, 0, 10.0));
		if (dynamic_cast<SpotLight*>(s) != nullptr) {
			SpotLight *spotLightSelected = (SpotLight*)s;
			objectGUI.add(gui_value2.setup("Height Reference", spotLightSelected->heightRef, 0.5, 30));
			objectGUI.add(gui_value3.setup("Total Width", spotLightSelected->totalWidth, 0.5, 75));
			objectGUI.add(gui_value4.setup("Falloff Start", spotLightSelected->falloffStart, 0.5, 75));
		}
		else if (dynamic_cast<AreaLight*>(s) != nullptr) {
			AreaLight *areaLightSelected = (AreaLight*)s;
		}
	}


	objectGUI.add(slider_location.setup("Location", s->position, glm::vec3(-20, -20, -20), glm::vec3(20, 20, 20)));
	group_rotation.setBorderColor(ofColor(25, 25, 25));
	objectGUI.add(group_rotation.setup("Rotation"));
	group_rotation.add(gui_angleX.setup("Angle X", s->rotation.x, -180, 180));
	group_rotation.add(gui_angleY.setup("Angle Y", s->rotation.y, -180, 180));
	group_rotation.add(gui_angleZ.setup("Angle Z", s->rotation.z, -180, 180));
	slider_scale.setBorderColor(ofColor(25, 25, 25));
	slider_location.setBorderColor(ofColor(25, 25, 25));


	// Don't need scale or materials for lights
	if (dynamic_cast<Light*>(s) == nullptr) {
		objectGUI.add(slider_scale.setup("Scale", s->scale, glm::vec3(1, 1, 1), glm::vec3(10, 10, 10)));
		
		group_texture.setBorderColor(ofColor(25, 25, 25));
		group_material.setBorderColor(ofColor(25, 25, 25));
		toggle_matte.setFillColor(ofColor(45, 138, 86));
		toggle_mirror.setFillColor(ofColor(45, 138, 86));
		toggle_glass.setFillColor(ofColor(45, 138, 86));
		toggle_reflective.setFillColor(ofColor(45, 138, 86));

		if (dynamic_cast<LSystem*>(s) == nullptr && dynamic_cast<Mesh*>(s) == nullptr) {
			objectGUI.add(group_texture.setup("Texture"));
			group_texture.add(gui_uvTile.setup("Tile Factor", s->objTexture.uvTileFactor, 1, 10));
			group_texture.add(button_texture);
			group_texture.add(button_remove_texture);
		}

		if (dynamic_cast<LSystem*>(s) == nullptr) {
			objectGUI.add(group_material.setup("Material"));
			group_material.add(label_material.setup("Current Material ", selectedMaterial));
			updateMaterial();
		}
		objectGUI.add(color.setup("Color", s->objMaterial.diffuseColor, ofColor(0, 0), ofColor(255, 255)));
		color.setBorderColor(ofColor(25, 25, 25));
		objectGUI.getGroup("Color").maximize();
	}

}


void UI::setMatte(bool & value) {
	if (value) {
		selectedMaterial = "Matte";
		toggle_mirror = false;
		toggle_glass = false;
		toggle_reflective = false;
	}
	else if (!value && selectedMaterial == "Matte") {
		toggle_matte = true;
	}
}
void UI::setMirror(bool & value) {
	if (value) {
		selectedMaterial = "Mirror";
		toggle_matte = false;
		toggle_glass = false;
		toggle_reflective = false;
	}
	else if (!value && selectedMaterial == "Mirror") {
		toggle_mirror = true;
	}
}
void UI::setGlass(bool & value) {
	if (value) {
		selectedMaterial = "Glass";
		toggle_matte = false;
		toggle_mirror = false;
		toggle_reflective = false;
	}
	else if (!value && selectedMaterial == "Glass") {
		toggle_glass = true;
	}
}
void UI::setReflective(bool & value) {
	if (value) {
		selectedMaterial = "Reflective";
		toggle_matte = false;
		toggle_glass = false;
		toggle_mirror = false;
	}
	else if (!value && selectedMaterial == "Metal") {
		toggle_reflective = true;
	}
}

void UI::LoadTextureFile() {
	cout << "Textured" << endl;
	if (scene->selected.size() > 0) {
		ofFileDialogResult result = ofSystemLoadDialog("Choose image(.jpg, .png) texture file");
		if (result.bSuccess) {
			scene->selected[0]->objTexture.addTexture(result.getPath());
		}
	}
}
