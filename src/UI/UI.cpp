#include "UI.h"

//--------------------------------------------------------------
void UI::setup() {
	// Set Button Listeners
	button_rayTrace.addListener(sceneRef, &Scene::HandleRayTrace);
	button_rayMarch.addListener(sceneRef, &Scene::HandleRayMarch);
	button_rename.addListener(sceneRef, &Scene::HandleRename);
	button_save.addListener(sceneRef, &Scene::HandleSaveImage);
	button_delete.addListener(sceneRef, &Scene::HandleDelete);
	button_clear.addListener(sceneRef, &Scene::HandleClearScene);
	button_loadTexture.addListener(sceneRef, &Scene::HandleLoadTexture);
	button_removeTexture.addListener(sceneRef, &Scene::HandleRemoveTexture);
	button_addSphere.addListener(sceneRef, &Scene::AddSphere);
	button_addCube.addListener(sceneRef, &Scene::AddCube);
	button_addPlane.addListener(sceneRef, &Scene::AddPlane);
	button_addCylinder.addListener(sceneRef, &Scene::AddCylinder);
	button_addCone.addListener(sceneRef, &Scene::AddCone);
	button_addTorus.addListener(sceneRef, &Scene::AddTorus);
	button_addLsystem.addListener(sceneRef, &Scene::AddLSystem);
	button_addMesh.addListener(sceneRef, &Scene::AddMesh);
	button_addPointLight.addListener(sceneRef, &Scene::AddPointLight);
	button_addSpotLight.addListener(sceneRef, &Scene::AddSpotLight);
	button_addAreaLight.addListener(sceneRef, &Scene::AddAreaLight);
	toggle_materialMatte.addListener(this, &UI::setMatte);
	toggle_materialMirror.addListener(this, &UI::setMirror);
	toggle_materialGlass.addListener(this, &UI::setGlass);
	toggle_materialReflective.addListener(this, &UI::setReflective);


	// Initialize GUI tabs
	currentTab = 0;
	tabs.push_back(Tab(20, 13, 110, 30, " Modeling ", (currentTab == 0)));
	tabs.push_back(Tab(145, 13, 110, 30, " Rendering ", (currentTab == 1)));


	// Initialize Modeling GUI and its elements
	modelingGUI.setHeaderBackgroundColor(ofColor(50, 50, 50));
	group_create.setHeaderBackgroundColor(ofColor(50, 50, 50));	
	group_modelingOptions.setHeaderBackgroundColor(ofColor(50, 50, 50));
	group_objects.setHeaderBackgroundColor(ofColor(10, 10, 10));
	group_lights.setHeaderBackgroundColor(ofColor(10, 10, 10));
	modelingGUI.setBorderColor(ofColor(25, 25, 25));
	group_create.setBorderColor(ofColor(25, 25, 25));
	group_modelingOptions.setBorderColor(ofColor(25, 25, 25));
	group_objects.setBorderColor(ofColor(25, 25, 25));
	group_lights.setBorderColor(ofColor(25, 25, 25));
	toggle_objectMode.setTextColor(ofColor(111, 168, 255));
	toggle_editMode.setTextColor(ofColor(111, 168, 255));
	button_rename.setTextColor(ofColor(111, 169, 255));
	button_save.setTextColor(ofColor(255, 192, 81));
	button_clear.setTextColor(ofColor(255, 63, 63));
	button_delete.setTextColor(ofColor(255, 63, 63));
	toggle_grid.setFillColor(ofColor(94, 132, 176));
	modelingGUI.setDefaultHeight(30);
	modelingGUI.setDefaultWidth(260);
	modelingGUI.loadFont("Fonts/Poppins/Poppins-Medium.ttf", 9);
	modelingGUI.setup("Modeling");
	modelingGUI.add(toggle_objectMode.setup(" Object Mode", true));
	modelingGUI.add(toggle_editMode.setup(" Edit Mode", false));
	modelingGUI.add(group_create.setup("Create"));
	group_create.add(group_objects.setup(" Objects"));
	group_objects.add(button_addSphere.setup(" Sphere \t\t\t\t\t\t\t\t\t\t\t\t(a)"));
	group_objects.add(button_addCube.setup(" Cube\t\t\t\t\t\t\t\t\t\t\t\t\t(0)"));
	group_objects.add(button_addPlane.setup(" Plane \t\t\t\t\t\t\t\t\t\t\t\t\t(1)"));
	group_objects.add(button_addCylinder.setup(" Cylinder  \t\t\t\t\t\t\t\t\t\t\t(2)"));
	group_objects.add(button_addCone.setup(" Cone\t\t\t\t\t\t\t\t\t\t\t\t\t(3)"));
	group_objects.add(button_addTorus.setup(" Torus   \t\t\t\t\t\t\t\t\t\t\t\t(4)"));
	group_objects.add(button_addLsystem.setup(" Lsystem \t\t\t\t\t\t\t\t\t\t\t(5)"));
	group_objects.add(button_addMesh.setup(" Mesh\t\t\t\t\t\t\t\t\t\t\t\t\t(6)"));
	group_create.add(group_lights.setup(" Lights"));
	group_lights.add(button_addPointLight.setup(" Point Light \t\t\t\t\t\t\t\t\t\t(7)"));
	group_lights.add(button_addSpotLight.setup(" Spot Light  \t\t\t\t\t\t\t\t\t\t(8)"));
	group_lights.add(button_addAreaLight.setup(" Area Light  \t\t\t\t\t\t\t\t\t\t(9)"));
	modelingGUI.add(group_modelingOptions.setup("Options"));
	group_modelingOptions.add(button_rename.setup(" Rename Selected Object\t\t\t(n)"));
	group_modelingOptions.add(toggle_grid.setup(" Show Grid\t\t\t\t\t\t\t\t\t\t\t(g)", true));
	group_modelingOptions.add(button_delete.setup(" Delete Selected Object \t\t\t\t(x)"));
	group_modelingOptions.add(button_clear.setup(" Clear Scene\t\t\t\t\t\t\t\t\t\t(d)"));
	modelingGUI.setPosition(10, 50);

	// Initialize Rendering GUI and its elements
	renderingGUI.setHeaderBackgroundColor(ofColor(50, 50, 50));
	group_renderingOptions.setHeaderBackgroundColor(ofColor(50, 50, 50));
	renderingGUI.setBorderColor(ofColor(25, 25, 25));
	group_renderingOptions.setBorderColor(ofColor(25, 25, 25));
	button_rayTrace.setTextColor(ofColor(0, 153, 76));
	button_rayMarch.setTextColor(ofColor(111, 168, 255));
	toggle_bvh.setFillColor(ofColor(0, 153, 76));
	toggle_image.setFillColor(ofColor(108, 176, 84));
	toggle_renderView.setFillColor(ofColor(123, 60, 230));
	renderingGUI.setDefaultHeight(30);
	renderingGUI.setDefaultWidth(260);
	renderingGUI.setup("Rendering");
	renderingGUI.add(button_rayTrace.setup(" RayTrace"));
	renderingGUI.add(button_rayMarch.setup(" RayMarch"));
	renderingGUI.add(i_slider_renderSamples.setup("Samples", 1, 1, 64));
	renderingGUI.add(i_slider_resolutionWidth.setup("Resolution Width", 1200, 30, 1920));
	renderingGUI.add(i_slider_resolutionHeight.setup("Resolution Height", 800, 20, 1080));
	renderingGUI.add(group_renderingOptions.setup("Options"));
	group_renderingOptions.add(button_save.setup(" Save Image \t\t\t\t\t\t\t\t\t\t(s)"));
	group_renderingOptions.add(toggle_bvh.setup(" Show BVH \t\t\t\t\t\t\t\t\t\t\t(b)", false));
	group_renderingOptions.add(toggle_image.setup(" Show Render    \t\t\t\t\t\t\t\t\t(r)", true));
	group_renderingOptions.add(toggle_renderView.setup(" Toggle Render View  \t\t\t\t\t\t(.)", false));
	renderingGUI.setPosition(10, 50);

	objectGUI.setup("Sphere");
	objectGUI.setBorderColor(ofColor::black);
}

//--------------------------------------------------------------
void UI::update() {
	if (i_slider_renderSamples < 3) {
		i_slider_renderSamples = 1;
	}
	else if (i_slider_renderSamples < 6) {
		i_slider_renderSamples = 4;
	}
	else if (i_slider_renderSamples < 12) {
		i_slider_renderSamples = 8;
	}
	else if (i_slider_renderSamples < 25) {
		i_slider_renderSamples = 16;
	}
	else if (i_slider_renderSamples < 50) {
		i_slider_renderSamples = 32;
	}
	else {
		i_slider_renderSamples = 64;
	}
	sceneRef->setSamples(static_cast<int>(i_slider_renderSamples));
	sceneRef->setResolution(static_cast<int>(i_slider_resolutionWidth), static_cast<int>(i_slider_resolutionHeight));


	if (sceneRef->getObjectSelected() != nullptr) {
		updateObject(sceneRef->getObjectSelected());
	}

	sceneRef->setShowBVH(static_cast<bool>(toggle_bvh));
	sceneRef->setShowImage(static_cast<bool>(toggle_image));
}

//--------------------------------------------------------------
void UI::draw() {
	for (Tab tab : tabs) {
		tab.draw();
	}

	// Draw corresponding GUI
	if (currentTab == 0) {
		modelingGUI.draw();
	}
	else if (currentTab == 1) {
		renderingGUI.draw();
	}

	if (showObjectGUI && sceneRef->getObjectSelected() != nullptr) {
		objectGUI.setPosition(ofGetWidth() - modelingGUI.getWidth() - 10, 10);
		objectGUI.draw();
	}

}

//--------------------------------------------------------------
void UI::drawGrid() {
	if (static_cast<bool>(toggle_grid)) {	
		float minAlpha = 10;
		float maxAlpha = 50;

		// Z-axis (blue)
		ofSetColor(ofColor(111, 169, 255));
		ofDrawLine(glm::vec3(0, 0, GRID_LINES), glm::vec3(0, 0, -GRID_LINES));

		// X-axis (red)
		ofSetColor(ofColor(255, 81, 81));
		ofDrawLine(glm::vec3(GRID_LINES, 0, 0), glm::vec3(-GRID_LINES, 0, 0));

		// White Grid lines
		for (int i = -GRID_LINES; i < GRID_LINES; i++) {
			float distance = glm::abs(i);
			float alpha = maxAlpha - (maxAlpha - minAlpha) * (distance / GRID_LINES);
			alpha = glm::max(alpha, minAlpha);
			
			ofSetColor(ofColor(178, 178, 178, alpha));
			ofDrawLine(glm::vec3(GRID_LINES, 0, i), glm::vec3(-GRID_LINES, 0, i));
			ofDrawLine(glm::vec3(i, 0, GRID_LINES), glm::vec3(i, 0, -GRID_LINES));
		}
	}
}

//--------------------------------------------------------------
void UI::updateObject(SceneObject *object) {
	object->setPosition(static_cast<glm::vec3>(vec_slider_position));
	object->setRotation(glm::vec3(static_cast<float>(i_slider_angleX), static_cast<float>(i_slider_angleY), static_cast<float>(i_slider_angleZ)));

	// Don't need scale for lights
	if (dynamic_cast<Light *>(object) == nullptr && dynamic_cast<RenderCam *>(object) == nullptr) {
		object->setScale(static_cast<glm::vec3>(vec_slider_scale));
	}

	object->setTileFactor(static_cast<int>(i_slider_uvTile));
	object->setColor(static_cast<ofColor>(slider_objectColor));


	// Set object specific variables
	if (dynamic_cast<Sphere *>(object) != nullptr) {
		Sphere *sphere = dynamic_cast<Sphere *>(object);
		sphere->setRadius(static_cast<float>(f_slider_radius));
	}
	else if (dynamic_cast<Cube *>(object) != nullptr) {
		Cube *cube = dynamic_cast<Cube *>(object);
		cube->setSide(static_cast<float>(f_slider_height));
	}
	else if (dynamic_cast<Plane *>(object) != nullptr) {
		Plane *plane = dynamic_cast<Plane *>(object);
		plane->setWidth(static_cast<float>(f_slider_width));
		plane->setHeight(static_cast<float>(f_slider_height));
	}	
	else if (dynamic_cast<Cylinder *>(object) != nullptr) {
		Cylinder *cylinder = dynamic_cast<Cylinder *>(object);
		cylinder->setRadius(static_cast<float>(f_slider_radius));
		cylinder->setHeight(static_cast<float>(f_slider_height));
	}
	else if (dynamic_cast<Cone *>(object) != nullptr) {
		Cone *cone = dynamic_cast<Cone *>(object);
		cone->setRadius(static_cast<float>(f_slider_radius));
		cone->setHeight(static_cast<float>(f_slider_height));
	}
	else if (dynamic_cast<Torus *>(object) != nullptr) {
		Torus *torus = dynamic_cast<Torus *>(object);
		torus->setRadius(static_cast<float>(f_slider_radius));
		torus->setMajorRadius(static_cast<float>(f_slider_majorRadius));
	}
	else if (dynamic_cast<LSystem *>(object) != nullptr) {
		LSystem *lsystem = dynamic_cast<LSystem *>(object);
		lsystem->setIterations(static_cast<int>(i_slider_LsystemIterations));
		lsystem->setAngle(static_cast<float>(f_slider_angle));
		lsystem->setTubeRadius(static_cast<float>(f_slider_radius));
		lsystem->setTubeHeight(static_cast<float>(f_slider_height));
		lsystem->setAxiom(static_cast<string>(textfield_LsystemAxiom));
		lsystem->setRule1(static_cast<string>(textfield_LsystemRule1));
		lsystem->setRule2(static_cast<string>(textfield_LsystemRule2));
		lsystem->setRule3(static_cast<string>(textfield_LsystemRule3));
		lsystem->generate();
	}
	else if (dynamic_cast<Light *>(object) != nullptr) {
		Light *light = dynamic_cast<Light *>(object);
		light->setIntensity(static_cast<float>(f_slider_lightIntensity));
		if (dynamic_cast<SpotLight *>(object) != nullptr) {
			SpotLight *spotLight = dynamic_cast<SpotLight *>(object);
			spotLight->setHeightRef(static_cast<float>(f_slider_height));
			spotLight->setTotalWidth(static_cast<float>(f_slider_lightWidth));
			spotLight->setFalloff(static_cast<float>(f_slider_lightFalloff));
		}
	}
	object->setMaterialType(selectedMaterial);
	object->setBounds();
	sceneRef->buildBVH();
}

//--------------------------------------------------------------
void UI::updateGUI(SceneObject *object) {
	if (object == nullptr) {
		objectGUI.clear();
		showObjectGUI = false;
		return;
	}

	selectedMaterial = object->getMaterial().toString();
	showObjectGUI = true;
	objectGUI.clear();
	objectGUI.setDefaultHeight(25);
	objectGUI.setDefaultWidth(260);
	objectGUI.setName(" " + object->getName());
	if (dynamic_cast<Sphere *>(object) != nullptr) {
		Sphere *sphere = dynamic_cast<Sphere *>(object);
		objectGUI.add(f_slider_radius.setup("Radius", sphere->getRadius(), 0.2f, 3.0f));
	}
	else if (dynamic_cast<Cube *>(object) != nullptr) {
		Cube *cube = dynamic_cast<Cube *>(object);
		objectGUI.add(f_slider_height.setup("Side", cube->getSide(), 0.2f, 3.0f));
	}
	else if (dynamic_cast<Plane *>(object) != nullptr) {
		Plane *plane = dynamic_cast<Plane *>(object);
		objectGUI.add(f_slider_width.setup("Width", plane->getWidth(), 0.2f, 40.0f));
		objectGUI.add(f_slider_height.setup("Height", plane->getHeight(), 0.2f, 40.0f));
	}
	else if (dynamic_cast<Cylinder *>(object) != nullptr) {
		Cylinder *cylinder = dynamic_cast<Cylinder *>(object);
		objectGUI.add(f_slider_radius.setup("Radius", cylinder->getRadius(), 0.2f, 5.0f));
		objectGUI.add(f_slider_height.setup("Height", cylinder->getHeight(), 0.2f, 5.0f));
	}
	else if (dynamic_cast<Cone *>(object) != nullptr) {
		Cone *cone = dynamic_cast<Cone *>(object);
		objectGUI.add(f_slider_radius.setup("Radius", cone->getRadius(), 0.2f, 5.0f));
		objectGUI.add(f_slider_height.setup("Height", cone->getHeight(), 0.2f, 5.0f));
	}
	else if (dynamic_cast<Torus *>(object) != nullptr) {
		Torus *torus = dynamic_cast<Torus *>(object);
		objectGUI.add(f_slider_radius.setup("Radius", torus->getRadius(), 0.1f, 5.0f));
		objectGUI.add(f_slider_majorRadius.setup("Major Radius", torus->getMajorRadius(), 0.5f, 5.0f));
	}
	else if (dynamic_cast<LSystem *>(object) != nullptr) {
		LSystem *lsystem = dynamic_cast<LSystem *>(object);
		objectGUI.add(i_slider_LsystemIterations.setup("Iterations", lsystem->getIterations(), 1, 10));
		objectGUI.add(f_slider_angle.setup("Angle", lsystem->getAngle(), -180, 180));
		objectGUI.add(f_slider_radius.setup("Tube Radius", lsystem->getTubeRadius(), 0.05f, 1.0f));
		objectGUI.add(f_slider_height.setup("Tube Height", lsystem->getTubeHeight(), 0.1f, 3.0f));
		objectGUI.add(textfield_LsystemAxiom.setup("Axiom", lsystem->getAxiom()));
		objectGUI.add(textfield_LsystemRule1.setup("F", lsystem->getRule1()));
		objectGUI.add(textfield_LsystemRule2.setup("A", lsystem->getRule2()));
		objectGUI.add(textfield_LsystemRule3.setup("B", lsystem->getRule3()));
	}
	else if (dynamic_cast<Light *>(object) != nullptr) {
		Light *light = dynamic_cast<Light *>(object);
		objectGUI.add(f_slider_lightIntensity.setup("Intensity", light->getIntensity(), 0, 10.0f));
		if (dynamic_cast<SpotLight *>(object) != nullptr) {
			SpotLight *spotLight = dynamic_cast<SpotLight *>(object);
			objectGUI.add(f_slider_height.setup("Height Reference", spotLight->getHeightRef(), 0.5f, 30.0f));
			objectGUI.add(f_slider_lightWidth.setup("Total Width", spotLight->getTotalWidth(), 0.5f, 75.0f));
			objectGUI.add(f_slider_lightFalloff.setup("Falloff Start", spotLight->getFalloff(), 0.5f, 75.0f));
		}
	}

	vec_slider_position.setBorderColor(ofColor(25, 25, 25));
	vec_slider_scale.setBorderColor(ofColor(25, 25, 25));
	group_objectRotation.setBorderColor(ofColor(25, 25, 25));
	objectGUI.add(vec_slider_position.setup("Position", object->getPosition(), glm::vec3(-20, -20, -20), glm::vec3(20, 20, 20)));
	objectGUI.add(group_objectRotation.setup("Rotation"));
	group_objectRotation.add(i_slider_angleX.setup("Angle X", object->getRotation().x, -180, 180));
	group_objectRotation.add(i_slider_angleY.setup("Angle Y", object->getRotation().y, -180, 180));
	group_objectRotation.add(i_slider_angleZ.setup("Angle Z", object->getRotation().z, -180, 180));

	// Don't need scale or materials for lights
	if (dynamic_cast<Light *>(object) == nullptr && dynamic_cast<RenderCam *>(object) == nullptr) {
		objectGUI.add(vec_slider_scale.setup("Scale", object->getScale(), glm::vec3(0.05, 0.05, 0.05), glm::vec3(5, 5, 5)));
		
		group_objectTexture.setBorderColor(ofColor(25, 25, 25));
		group_objectMaterial.setBorderColor(ofColor(25, 25, 25));
		toggle_materialMatte.setFillColor(ofColor(45, 138, 86));
		toggle_materialMirror.setFillColor(ofColor(45, 138, 86));
		toggle_materialGlass.setFillColor(ofColor(45, 138, 86));
		toggle_materialReflective.setFillColor(ofColor(45, 138, 86));

		if (dynamic_cast<LSystem *>(object) == nullptr && dynamic_cast<Mesh *>(object) == nullptr) {
			objectGUI.add(group_objectTexture.setup("Texture"));
			group_objectTexture.add(i_slider_uvTile.setup("Tile Factor", object->getTexture().getTileFactor(), 1, 10));
			group_objectTexture.add(button_loadTexture);
			group_objectTexture.add(button_removeTexture);

		}

		if (dynamic_cast<LSystem *>(object) == nullptr) {
			objectGUI.add(group_objectMaterial.setup("Material"));
			bool isMatte = (selectedMaterial == "Matte");
			bool isMirror = (selectedMaterial == "Mirror");
			bool isGlass = (selectedMaterial == "Glass");
			bool isReflective = (selectedMaterial == "Reflective");

			group_objectMaterial.add(toggle_materialMatte.setup(" Matte", isMatte));
			group_objectMaterial.add(toggle_materialMirror.setup(" Mirror", isMirror));
			group_objectMaterial.add(toggle_materialGlass.setup(" Glass", isGlass));
			group_objectMaterial.add(toggle_materialReflective.setup(" Reflective", isReflective));
		}
		
		objectGUI.add(slider_objectColor.setup("Color", object->getMaterial().getDiffuse(), ofColor(0, 0), ofColor(255, 255)));
		slider_objectColor.setBorderColor(ofColor(25, 25, 25));
		objectGUI.getGroup("Color").maximize();
	}
}

//--------------------------------------------------------------
void UI::mousePressed(int x, int y, int button) {
	// Handle GUI switch between Rendering and Modeling Tabs
	for (int i = 0; i < tabs.size(); i++) {
		if (tabs[i].isMousePressed(x, y)) {
			currentTab = i;
			tabs[i].setActive(true);
			tabs[(i == 0) ? 1 : 0].setActive(false);
		}
	}
}

//--------------------------------------------------------------
void UI::setMaterialToggle(const string &mat, bool &value) {
	bool isMatte = (mat == "Matte");
	bool isMirror = (mat == "Mirror");
	bool isGlass = (mat == "Glass");
	bool isReflective = (mat == "Reflective");
	if (value) {
		selectedMaterial = mat;
		toggle_materialMatte = isMatte;
		toggle_materialMirror = isMirror;
		toggle_materialGlass = isGlass;
		toggle_materialReflective = isReflective;
	}
	else if (!value && selectedMaterial == mat) {
		if (isMatte) toggle_materialMatte = true;
		if (isMirror) toggle_materialMirror = true;
		if (isGlass) toggle_materialGlass = true;
		if (isReflective) toggle_materialReflective = true;
	}
}
