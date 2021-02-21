#include "ofApp.h"

/*************************************************************************
* ------------------- RAY TRACING / RAY MARCHING -------------------------
* Date Created: Fall 2019
* Author: Ivan Hernandez
* Description: This project adds the functionality to perform ray
* marching on a 3D scene. It also has the sceneSDF() function that
* calls each primitive's sdf and returns the closest distance to the
* point. This program also has a new primitive, a torus. The torus
* can be be rendered using the ray marching algorithm and has matrices
* that can translate or rotate the object.
*
* (3/28/2020) Update: Added functionality to repeat an SDF primitive
* infinitely in 3 dimensions by using the opRep() function. Also, added the
* opRound() function, which makes it possible to round the hard edges of some
* scene objects. This update also introduces a new primitive, the Cube class.
* It has it's own sdf function so that the program can render cube shapes.
***************************************************************************/


//--------------------------------------------------------------
void ofApp::setup() {
	ofSetBackgroundColor(ofColor(30, 30, 30));
	theCam = &mainCam;
	mainCam.setDistance(12);
	mainCam.setNearClip(.1);
	sideCam.setPosition(glm::vec3(5, 0, 0));
	sideCam.lookAt(glm::vec3(0, 0, 0));
	sideCam.setNearClip(.1);
	previewCam.setNearClip(.1);
	ofSetSmoothLighting(true);
	renderCam.objName = "Render_Cam";

	//spotlight
	//sp1 = SpotLight(glm::vec3(-3, 4, 1), glm::vec3(.5, -1, -.55), .1, 4);

	//sp1 = Spotlight(glm::vec3(.5, -1, -.55), .3, 3);
	//sp1.position = glm::vec3(-3, 4, 1);

	//point light 
	//light1 = Light(glm::vec3(-4, 3, 5), 50);
	//light2 = Light(glm::vec3(0, 5, -25), 50);
	//light3 = Light(glm::vec3(0, 5, -50), 50);
	//light4 = Light(glm::vec3(0, 5, -75), 50);

	lightScene.enable();
	lightScene.setDiffuseColor(ofColor(255.f, 255.f, 255.f));
	lightScene.setSpecularColor(ofColor(255.f, 255.f, 255.f));


	image.allocate(imageWidth, imageHeight, ofImageType::OF_IMAGE_COLOR);

	rayTracer = RayTracer(imageWidth, imageHeight, image, renderCam);
	rayMarcher = RayMarcher(imageWidth, imageHeight, image);

	nearestDistance = FLT_MAX;
	//f1 = LSystem(glm::vec3(0, -2.5, 0), 1, ofColor::seaGreen);
	//wp1 = WaterPool(glm::vec3(1.5, -3, -1), 1, ofColor::mediumPurple);
	//torus1 = Torus(glm::vec3(-1.3, -1.2, 0), 1, 0.5, ofColor::seaGreen);
	//torus2 = Torus(glm::vec3(2.2, -0.4, -2), 2, 0.2, 40.0f, glm::vec3(1, -1, 0), ofColor::orangeRed);

	// Scene Button Listeners
	button_rayTrace.addListener(this, &ofApp::handleRayTrace);
	button_rayMarch.addListener(this, &ofApp::handleRayMarch);
	button_saveImage.addListener(this, &ofApp::handleSaveImage);
	button_delete.addListener(this, &ofApp::handleDelete);
	button_sphere.addListener(this, &ofApp::addSphere);
	button_cube.addListener(this, &ofApp::addCube);
	button_plane.addListener(this, &ofApp::addPlane);	
	button_cylinder.addListener(this, &ofApp::addCylinder);
	button_cone.addListener(this, &ofApp::addCone);
	button_torus.addListener(this, &ofApp::addTorus);
	button_mesh.addListener(this, &ofApp::addMesh);
	button_lsystem.addListener(this, &ofApp::addLSystem);
	button_point_light.addListener(this, &ofApp::addPointLight);
	button_spot_light.addListener(this, &ofApp::addSpotLight);
	button_area_light.addListener(this, &ofApp::addAreaLight);
	
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
	sceneGUI.add(gui_samples.setup("Samples", samples, 1, 64));
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
	//objectGUI.add(gui_angle1.setup("angle", torus1.angle, -90, 90));
	//objectGUI.add(slider_rotation.setup("Rotation", torus1.axisR, glm::vec3(-1, -1, -1),
	//	glm::vec3(1, 1, 1)));
	//slider_rotation.setBorderColor(ofColor(25, 25, 25));
	//objectGUI.add(gui_angle2.setup("angle", torus2.angle, -90, 90));
	//objectGUI.add(slider_scale.setup("Scale", torus2.axisR, glm::vec3(-1, -1, -1),
	//	glm::vec3(1, 1, 1)));
	//slider_scale.setBorderColor(ofColor(25, 25, 25));
	//objectGUI.setPosition(ofGetWidth() - sceneGUI.getWidth() - 10, 10);
}

//--------------------------------------------------------------
void ofApp::update() {
	previewCam.setPosition(renderCam.position);
	previewCam.setOrientation(renderCam.getRotateMatrix());
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


	samples = (int)gui_samples;

	lightScene.setGlobalPosition(theCam->getPosition());
	if (selected.size() > 0) {
		updateSelected(selected[0]);
		objectGUI.maximize();
		mainCam.setControlArea(ofRectangle(sceneGUI.getWidth(), 0, ofGetWidth() - sceneGUI.getWidth() - objectGUI.getWidth(), ofGetHeight()));

	}
	mainCam.setControlArea(ofRectangle(sceneGUI.getWidth(), 0, ofGetWidth() - sceneGUI.getWidth(), ofGetHeight()));
}

// Use the interface to manipulate scene object attributes
void ofApp::updateSelected(SceneObject *s) {
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
	
	s->reflectCoeff = (float)gui_reflect;
	s->position = static_cast<glm::vec3>(slider_location);
	s->rotation.x = static_cast<int>(gui_angleX);
	s->rotation.y = static_cast<int>(gui_angleY);
	s->rotation.z = static_cast<int>(gui_angleZ);
	s->diffuseColor = (ofColor)color;

	// Don't need scale for lights
	if (dynamic_cast<Light*>(s) == nullptr) {
		s->scale = static_cast<glm::vec3>(slider_scale);
	}

}

// Load scene object attributes to interface
void ofApp::updateGUI(SceneObject *s) {
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

	
	objectGUI.add(gui_reflect.setup("Reflection", s->reflectCoeff, 0.0, 1.0));
	objectGUI.add(slider_location.setup("Location", s->position, glm::vec3(-5, -5, -5), glm::vec3(5, 10, 5)));
	//objectGUI.add(slider_rotation.setup("Angle Rotation", s->rotation, glm::vec3(-90, -90, -90), glm::vec3(90, 90, 90)));
	group_rotation.setBorderColor(ofColor(25, 25, 25));
	objectGUI.add(group_rotation.setup("Rotation"));
	group_rotation.add(gui_angleX.setup("Angle X", s->rotation.x, -90, 90));
	group_rotation.add(gui_angleY.setup("Angle Y", s->rotation.y, -90, 90));
	group_rotation.add(gui_angleZ.setup("Angle Z", s->rotation.z, -90, 90));
	slider_scale.setBorderColor(ofColor(25, 25, 25));

	// Don't need scale for lights
	if (dynamic_cast<Light*>(s) == nullptr) {
		objectGUI.add(slider_scale.setup("Scale", s->scale, glm::vec3(1, 1, 1), glm::vec3(10, 10, 10)));
	}

	slider_location.setBorderColor(ofColor(25, 25, 25));
	objectGUI.add(color.setup("Color", s->diffuseColor, ofColor(0, 0), ofColor(255, 255)));
	color.setBorderColor(ofColor(25, 25, 25));
	objectGUI.getGroup("Color").maximize();
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofEnableDepthTest();

	theCam = ((bool)toggle_render_cam) ? &previewCam : &mainCam;
	theCam->begin();

	if ((bool)toggle_image && renderFinished) {
		ofPushMatrix();
			ofMultMatrix(renderCam.Transform);
			image.draw(glm::vec3(renderCam.view.bottomLeft()), renderCam.view.width(), renderCam.view.height());
		ofPopMatrix();
	}
	// Draw Grid
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
	ofSetColor(ofColor::white);

	ofEnableLighting();
	material.begin();
	ofFill();
	ofSetColor(ofColor::white);
	
	for (int i = 0; i < scene.size(); i++) {
		scene[i]->draw();
	}

	material.end();
	ofDisableLighting();


	for (int i = 0; i < lights.size(); i++) {
		lights[i]->draw();
	}
	renderCam.draw();
	theCam->end();
	ofDisableDepthTest();

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

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
	selected.clear();
	objectGUI.clear();

	vector<SceneObject *> hits;

	glm::vec3 p = theCam->screenToWorld(glm::vec3(x, y, 0));
	glm::vec3 d = p - theCam->getPosition();
	glm::vec3 dn = glm::normalize(d);

	for (int i = 0; i < scene.size(); i++) {
		glm::vec3 point, normal;

		if (scene[i]->intersect(Ray(p, dn), point, normal)) {
			hits.push_back(scene[i]);
		}
	}

	for (int i = 0; i < lights.size(); i++) {
		glm::vec3 point, normal;
		if (lights[i]->intersect(Ray(p, dn), point, normal)) {
			hits.push_back(lights[i]);
		}
	}

	glm::vec3 tempPoint, tempNormal;
	if (renderCam.intersect(Ray(p, dn), tempPoint, tempNormal)) {
		hits.push_back(&renderCam);
	}

	SceneObject *selectedObj = NULL;
	if (hits.size() > 0) {
		selectedObj = hits[0];
		float nearestDist = FLT_MAX;
		for (int n = 0; n < hits.size(); n++) {
			float dist = glm::length(hits[n]->position - theCam->getPosition());
			if (dist < nearestDist) {
				nearestDist = dist;
				selectedObj = hits[n];
			}
		}
	}
	for (int i = 0; i < scene.size(); i++) {
		scene[i]->isSelected = false;
	}
	for (int i = 0; i < lights.size(); i++) {
		lights[i]->isSelected = false;
	}

	renderCam.isSelected = false;

	if (selectedObj) {
		selectedObj->isSelected = true;
		selected.push_back(selectedObj);
		hideGUI = false;
		updateGUI(selectedObj);
	}
	else {	
		selected.clear();
		objectGUI.clear();
		hideGUI = true;
	}

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
	//bDrag = false;

}

void ofApp::handleRayTrace() {
	time_t start, end;
	for (int i = 0; i < scene.size(); i++) {
		if (dynamic_cast<LSystem*>(scene[i]) != nullptr) {
			LSystem *lsys = (LSystem*)scene[i];
			cout << "LSystem: " << lsys->sentence << endl;
		}
	}

	time(&start);
	image = rayTracer.render(samples);
	time(&end);

	renderFinished = true;
	cout << "\nFinished Ray Tracing : "  << double(end - start) << " sec\n" << endl;
}

void ofApp::handleRayMarch() {
	time_t start, end;
	for (int i = 0; i < scene.size(); i++) {
		if (dynamic_cast<LSystem*>(scene[i]) != nullptr) {
			LSystem *lsys = (LSystem*)scene[i];
			cout << "LSystem: " << lsys->sentence << endl;
		}
	}

	time(&start);
	image = rayMarcher.render(samples);
	time(&end);

	renderFinished = true;
	cout << "\nFinished Ray Marching : " << double(end - start) << " sec\n" << endl;
}

void ofApp::handleSaveImage() {
	if (renderFinished) {
		ofFileDialogResult result = ofSystemSaveDialog("render.jpg", "Save");
		if (result.bSuccess) {
			image.save(result.getPath());
		}
	}
}

void ofApp::handleDelete() {
	for (int i = 0; i < scene.size(); i++) {
		if (selected.size() > 0 && scene[i]->objName == selected[0]->objName) {
			hideGUI = true;
			rayTracer.removeObject(scene[i]->objName);
			rayMarcher.removeObject(scene[i]->objName);
			scene.erase(std::remove(scene.begin(), scene.end(), scene[i]), scene.end());
			selected.clear();
		}
	}
	for (int i = 0; i < lights.size(); i++) {
		if (selected.size() > 0 && lights[i]->objName == selected[0]->objName) {
			hideGUI = true;
			rayTracer.removeLight(lights[i]->objName);
			rayMarcher.removeLight(lights[i]->objName);
			lights.erase(std::remove(lights.begin(), lights.end(), lights[i]), lights.end());
			selected.clear();
		}
	}
}


void ofApp::addObject(SceneObject *s) {
	scene.push_back(s);
	rayTracer.addObject(*s);
	rayMarcher.addObject(*s);
}

void ofApp::addLight(Light *light) {
	lights.push_back(light);
	rayTracer.addLight(*light);
	rayMarcher.addLight(*light);
}

void ofApp::addSphere() {
	string nameString = "Sphere_" + to_string(2);
	addObject(new Sphere(glm::vec3(0, 0, 0), 1, "Sphere_" + to_string(++sphereCount), ofColor::seaGreen));
}
void ofApp::addCube() {
	addObject(new Cube(glm::vec3(0, 0, 0), 2, "Cube_" + to_string(++cubeCount), ofColor::seaGreen));
}
void ofApp::addPlane() {
	addObject(new Plane(glm::vec3(0, -3, 0), glm::vec3(0, 1, 0), "Plane_" + to_string(++planeCount), ofColor::lightGray));
}
void ofApp::addCylinder() {
	addObject(new Cylinder(glm::vec3(0, 0, 0), 2, 0.5, "Cylinder_" + to_string(++cylinderCount), ofColor::seaGreen));
}
void ofApp::addCone() {
	addObject(new Cone(glm::vec3(0, 0, 0), 2, 0.5, "Cone_" + to_string(++coneCount), ofColor::seaGreen));
}
void ofApp::addTorus() {
	addObject(new Torus(glm::vec3(0, 0, 0), 1, 0.5, "Torus_" + to_string(++torusCount), ofColor::seaGreen));
}
void ofApp::addMesh() {
	ofFileDialogResult result = ofSystemLoadDialog();
	if (result.bSuccess && result.fileName.substr(result.fileName.find_last_of(".") + 1) == "obj") {
		FileLoader(result.filePath.c_str());
	}
	else {
		cout << "Incorrect file type. Please enter only .obj files\n" << endl;
	}
}
void ofApp::addLSystem() {
	LSystem *ls = new LSystem(glm::vec3(0,0,0), 1, "F", "LSystem_" + to_string(++lsystemCount));
	ls->generate();
	addObject(ls);
}
void ofApp::addWaterPool() {
	addObject(new WaterPool(glm::vec3(0, 0, 0), 1, "WaterPool_" + to_string(++waterpoolCount), ofColor::seaGreen));
}
void ofApp::addPointLight() {
	addLight(new Light(glm::vec3(-4, 3, 5), 50, "Point_Light_" + to_string(++pointlightCount)));
}

void ofApp::addSpotLight() {
	addLight(new SpotLight(glm::vec3(0, 3, 0), glm::vec3(0, -1, 0), 10.0f, 10.0f, "Spot_Light_" + to_string(++spotlightCount)));
}

void ofApp::addAreaLight() {
	addLight(new AreaLight(glm::vec3(0, 3, 0), glm::vec3(0, -1, 0), 1.0f, 1.0f, "Area_Light_" + to_string(++arealightCount)));
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	switch (key) {
	case OF_KEY_F1: theCam = &mainCam;
		break;
	case OF_KEY_F2: theCam = &sideCam;
		break;
	case OF_KEY_F3: theCam = &previewCam;
		break;
	case 'b': 
		hideGrid = !hideGrid;
		break;
	default:
		break;
	}
}

/**
* File loader class from the first project. It reads a .obj file
* and stores the information regarding the vertices, vertex normals, and faces.
*/
bool ofApp::FileLoader(const char * path) {
	vector<int> tempIndices;
	vector<int> tempVertNormIndices;
	vector<glm::vec3> tempVertices;
	vector<glm::vec3> tempVertNormals;
	FILE * file;
	errno_t err = fopen_s(&file, path, "r");
	if (err != 0) {
		printf("Could not open file!\n");
		return false;
	}
	while (1) {
		char line[128];
		int res = fscanf(file, "%s", line);
		if (res == EOF)
			break;

		if (strcmp(line, "v") == 0) {
			glm::vec3 vertex;
			fscanf_s(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			tempVertices.push_back(vertex);
		}
		if (strcmp(line, "vn") == 0) {
			glm::vec3 vertNormal;
			fscanf_s(file, "%f %f %f\n", &vertNormal.x, &vertNormal.y, &vertNormal.z);
			tempVertNormals.push_back(vertNormal);
		}
		else if (strcmp(line, "f") == 0) {
			unsigned int vertexIndex[3];
			unsigned int vertNormIndex[3];

			//Only taking the vertex indices so we ignore the other numbers
			fscanf(file, "%d/%*d/%d %d/%*d/%d %d/%*d/%d\n", &vertexIndex[0], &vertNormIndex[0],
				&vertexIndex[1], &vertNormIndex[1], &vertexIndex[2], &vertNormIndex[2]);
			tempIndices.push_back(vertexIndex[0]);
			tempIndices.push_back(vertexIndex[1]);
			tempIndices.push_back(vertexIndex[2]);
			tempVertNormIndices.push_back(vertNormIndex[0]);
			tempVertNormIndices.push_back(vertNormIndex[1]);
			tempVertNormIndices.push_back(vertNormIndex[2]);
		}
	}

	vector<glm::vec3> meshVertices;
	vector<glm::vec3> meshVerticesNormals;
	vector<Triangle> meshTris;

	for (unsigned int i = 0; i < tempIndices.size(); i++) {
		unsigned int vertexIndex = tempIndices[i];
		glm::vec3 vertex = tempVertices[vertexIndex - 1];
		meshVertices.push_back(vertex);
	}
	for (unsigned int i = 0; i < tempVertNormIndices.size(); i++) {
		unsigned int vertNormIndex = tempVertNormIndices[i];
		glm::vec3 vn = tempVertNormals[vertNormIndex - 1];
		meshVerticesNormals.push_back(vn);
	}

	int count = 0;
	vector<int> temp;
	for (unsigned int i = 0; i < meshVertices.size(); i++) {
		temp.push_back(i);
		count++;
		if (count == 3) {
			Triangle triangle;
			triangle.i = temp[0];
			triangle.in = temp[0];
			triangle.j = temp[1];
			triangle.jn = temp[1];
			triangle.k = temp[2];
			triangle.kn = temp[2];
			meshTris.push_back(triangle);
			temp.clear();
			count = 0;
		}
	}
	Mesh *meshObj = new Mesh(glm::vec3(0, 0, 0), meshTris, meshVertices, meshVerticesNormals, "Mesh_" + to_string(++meshCount), ofColor::seaGreen);

	scene.push_back(meshObj);
	rayTracer.addObject(*meshObj);
	rayMarcher.addObject(*meshObj);

	// Display diagnostic information
	cout << "Number of Vertices: " << meshVertices.size() << endl;
	cout << "Number of Faces: " << meshTris.size() << endl;
	cout << "Number of Vertex Normals: " << meshVerticesNormals.size() << endl;
}

// Allows the user to drag in a .obj file into the scene
void ofApp::dragEvent(ofDragInfo dragInfo) {
	if (dragInfo.files.size() > 1) {

		dragInfo.files.clear();
		mesh.vertices.clear();
		mesh.tris.clear();
	}

	for (int i = 0; i < dragInfo.files.size(); i++) {

		string dir = dragInfo.files[i];
		char * file = new char[dir.length() + 1];
		strcpy(file, dir.c_str());

		FileLoader(file);
		cout << "files = " << dragInfo.files[i] << endl;
	}
}
