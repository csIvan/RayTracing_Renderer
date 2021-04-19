#include "UI.h"


void UI::setup(Scene *s) {
	scene = s;

	// Scene Button Listeners
	button_rayTrace.addListener(scene, &Scene::handleRayTrace);
	button_rayMarch.addListener(scene, &Scene::handleRayMarch);
	button_saveImage.addListener(scene, &Scene::handleSaveImage);
	button_delete.addListener(scene, &Scene::handleDelete);
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
	toggle_matte.addListener(this, &UI::setMatte);
	toggle_mirror.addListener(this, &UI::setMirror);
	toggle_glass.addListener(this, &UI::setGlass);
	toggle_metal.addListener(this, &UI::setMetal);

	// Setup Scene User Interface
	sceneGUI.setHeaderBackgroundColor(ofColor(50, 50, 50));
	sceneGUI.setDefaultHeight(30);
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
	group_objects.add(button_sphere.setup(" Sphere"));
	group_objects.add(button_cube.setup(" Cube"));
	group_objects.add(button_plane.setup(" Plane"));
	group_objects.add(button_cylinder.setup(" Cylinder"));
	group_objects.add(button_cone.setup(" Cone"));
	group_objects.add(button_torus.setup(" Torus"));
	group_objects.add(button_mesh.setup(" Mesh (.obj)"));
	group_objects.add(button_lsystem.setup(" LSystem"));

	group_create.add(group_lights.setup(" Lights"));;
	group_lights.setHeaderBackgroundColor(ofColor::black);
	group_lights.setBorderColor(ofColor(20, 20, 20));
	group_lights.add(button_point_light.setup(" Point Light"));
	group_lights.add(button_spot_light.setup(" Spot Light"));
	group_lights.add(button_area_light.setup(" Area Light"));

	group_scene.setHeaderBackgroundColor(ofColor(50, 50, 50));
	group_scene.setBorderColor(ofColor(25, 25, 25));
	sceneGUI.add(group_scene.setup("Scene"));
	button_saveImage.setTextColor(ofColor(255, 192, 81));
	toggle_image.setFillColor(ofColor(108, 176, 94));
	toggle_grid.setFillColor(ofColor(94, 132, 176));
	toggle_render_cam.setFillColor(ofColor(123, 60, 230));
	button_delete.setTextColor(ofColor(255, 63, 63));
	group_scene.add(button_saveImage.setup(" Save Image"));
	group_scene.add(toggle_grid.setup(" Toggle Grid", true));
	group_scene.add(toggle_render_cam.setup(" Toggle Render Cam", false));
	group_scene.add(toggle_image.setup(" Show Render", true));
	group_scene.add(button_delete.setup(" Delete Selected Object"));

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
		group_material.add(toggle_metal.setup(" Metal", false));
	}
	else if (selectedMaterial == "Mirror") {
		group_material.add(toggle_matte.setup(" Matte", false));
		group_material.add(toggle_mirror.setup(" Mirror", true));
		group_material.add(toggle_glass.setup(" Glass", false));
		group_material.add(toggle_metal.setup(" Metal", false));
	}
	else if (selectedMaterial == "Glass") {
		group_material.add(toggle_matte.setup(" Matte", false));
		group_material.add(toggle_mirror.setup(" Mirror", false));
		group_material.add(toggle_glass.setup(" Glass", true));
		group_material.add(toggle_metal.setup(" Metal", false));
	}
	else if (selectedMaterial == "Metal") {
		group_material.add(toggle_matte.setup(" Matte", false));
		group_material.add(toggle_mirror.setup(" Mirror", false));
		group_material.add(toggle_glass.setup(" Glass", false));
		group_material.add(toggle_metal.setup(" Metal", true));
	}
}

// Use the interface to manipulate scene object attributes
void UI::updateSelected(SceneObject *s) {
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
		lightSelected->intensity = (int)gui_ivalue1;
		if (dynamic_cast<SpotLight*>(s) != nullptr) {
			SpotLight *spotLightSelected = (SpotLight*)s;
			spotLightSelected->heightRef = (float)gui_value1;
			spotLightSelected->totalWidth = (float)gui_value2;
			spotLightSelected->falloffStart = (float)gui_value3;
		}
		else if (dynamic_cast<AreaLight*>(s) != nullptr) {
			AreaLight *areaLightSelected = (AreaLight*)s;
			areaLightSelected->height = (float)gui_value1;
			areaLightSelected->width = (float)gui_value2;
		}
	}

	//s->objMaterial.reflection = (float)gui_reflect;
	s->objMaterial.setString(selectedMaterial);
	s->position = static_cast<glm::vec3>(slider_location);
	s->rotation.x = static_cast<int>(gui_angleX);
	s->rotation.y = static_cast<int>(gui_angleY);
	s->rotation.z = static_cast<int>(gui_angleZ);
	s->objMaterial.diffuseColor = (ofColor)color;

	// Don't need scale for lights
	if (dynamic_cast<Light*>(s) == nullptr) {
		s->scale = static_cast<glm::vec3>(slider_scale);
	}

}

// Load scene object attributes to interface
void UI::updateGUI(SceneObject *s) {
	objectGUI.setDefaultHeight(24);
	objectGUI.setName("			" + s->objName);
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
	}
	else if (dynamic_cast<LSystem*>(s) != nullptr) {
		LSystem *lsystemSelected = (LSystem*)s;
		objectGUI.add(gui_ivalue1.setup("Iterations", lsystemSelected->iterations, 1, 10));
		objectGUI.add(gui_value1.setup("Angle", lsystemSelected->angle, -90, 90));
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
		objectGUI.add(gui_ivalue1.setup("Intensity", lightSelected->intensity, 0, 150));
		if (dynamic_cast<SpotLight*>(s) != nullptr) {
			SpotLight *spotLightSelected = (SpotLight*)s;
			objectGUI.add(gui_value1.setup("Height Reference", spotLightSelected->heightRef, 0.5, 30));
			objectGUI.add(gui_value2.setup("Total Width", spotLightSelected->totalWidth, 0.5, 75));
			objectGUI.add(gui_value3.setup("Falloff Start", spotLightSelected->falloffStart, 0.5, 75));
		}
		else if (dynamic_cast<AreaLight*>(s) != nullptr) {
			AreaLight *areaLightSelected = (AreaLight*)s;
			objectGUI.add(gui_value1.setup("Height", areaLightSelected->height, 0.5, 10));
			objectGUI.add(gui_value2.setup("Width", areaLightSelected->width, 0.5, 10));
		}
	}


	objectGUI.add(slider_location.setup("Location", s->position, glm::vec3(-5, -5, -5), glm::vec3(5, 10, 5)));
	//objectGUI.add(slider_rotation.setup("Angle Rotation", s->rotation, glm::vec3(-90, -90, -90), glm::vec3(90, 90, 90)));
	group_rotation.setBorderColor(ofColor(25, 25, 25));
	objectGUI.add(group_rotation.setup("Rotation"));
	group_rotation.add(gui_angleX.setup("Angle X", s->rotation.x, -90, 90));
	group_rotation.add(gui_angleY.setup("Angle Y", s->rotation.y, -90, 90));
	group_rotation.add(gui_angleZ.setup("Angle Z", s->rotation.z, -90, 90));
	slider_scale.setBorderColor(ofColor(25, 25, 25));
	slider_location.setBorderColor(ofColor(25, 25, 25));


	// Don't need scale or materials for lights
	if (dynamic_cast<Light*>(s) == nullptr) {
		objectGUI.add(slider_scale.setup("Scale", s->scale, glm::vec3(1, 1, 1), glm::vec3(10, 10, 10)));

		group_material.setBorderColor(ofColor(25, 25, 25));
		toggle_matte.setFillColor(ofColor(45, 138, 86));
		toggle_mirror.setFillColor(ofColor(45, 138, 86));
		toggle_glass.setFillColor(ofColor(45, 138, 86));
		toggle_metal.setFillColor(ofColor(45, 138, 86));
		objectGUI.add(group_material.setup("Material"));
		group_material.add(label_material.setup("Current Material ", selectedMaterial));
		updateMaterial();
		//group_material.add(gui_reflect.setup("Reflection", s->objMaterial.reflection, 0.0, 1.0));
	}

	objectGUI.add(color.setup("Color", s->objMaterial.diffuseColor, ofColor(0, 0), ofColor(255, 255)));
	color.setBorderColor(ofColor(25, 25, 25));
	objectGUI.getGroup("Color").maximize();

}


void UI::setMatte(bool & value) {
	if (value) {
		selectedMaterial = "Matte";
		toggle_mirror = false;
		toggle_glass = false;
		toggle_metal = false;
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
		toggle_metal = false;
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
		toggle_metal = false;
	}
	else if (!value && selectedMaterial == "Glass") {
		toggle_glass = true;
	}
}
void UI::setMetal(bool & value) {
	if (value) {
		selectedMaterial = "Metal";
		toggle_matte = false;
		toggle_glass = false;
		toggle_mirror = false;
	}
	else if (!value && selectedMaterial == "Metal") {
		toggle_metal = true;
	}
}