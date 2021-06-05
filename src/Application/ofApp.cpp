#include "ofApp.h"

/*************************************************************************
* ------------------- RAY TRACING / RAY MARCHING -------------------------
* Date Created: Fall 2020
* Author: Ivan Hernandez
* Description: This project adds the functionality to perform ray
* marching on a 3D scene. It also has the sceneSDF() function that
* calls each primitive's sdf and returns the closest distance to the
* point. This program also has a new primitive, a torus. The torus
* can be be rendered using the ray marching algorithm and has matrices
* that can translate or rotate the object.
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

	lightScene.enable();
	lightScene.setDiffuseColor(ofColor(255.f, 255.f, 255.f));
	lightScene.setSpecularColor(ofColor(255.f, 255.f, 255.f));

	ui.setup(&scene);
	scene.setup();

	box = new Box(glm::vec3(2, -2, 1), glm::vec3(4, 2, -1));
}

//--------------------------------------------------------------
void ofApp::update() {
	previewCam.setPosition(scene.renderCam.position);
	previewCam.setOrientation(scene.renderCam.getRotateMatrix());
	ui.update();

	lightScene.setGlobalPosition(theCam->getPosition());
	if (scene.selected.size() > 0) {
		mainCam.setControlArea(ofRectangle(ui.sceneGUI.getWidth(), 0, ofGetWidth() - ui.sceneGUI.getWidth() - ui.objectGUI.getWidth(), ofGetHeight()));
	}
	else {
		mainCam.setControlArea(ofRectangle(ui.sceneGUI.getWidth(), 0, ofGetWidth() - ui.sceneGUI.getWidth(), ofGetHeight()));
	}
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofEnableDepthTest();

	theCam = ((bool)ui.toggle_render_cam) ? &previewCam : &mainCam;
	theCam->begin();

	if ((bool)ui.toggle_image && scene.renderFinished) {
		ofPushMatrix();
			ofMultMatrix(scene.renderCam.Transform);
			scene.image.draw(glm::vec3(scene.renderCam.view.bottomLeft()), scene.renderCam.view.width(), scene.renderCam.view.height());
		ofPopMatrix();
	}

	ui.drawGrid();
	//box->draw();

	ofEnableLighting();
	material.begin();
	ofFill();
	ofSetColor(ofColor::white);
	
	scene.draw();

	theCam->end();
	ofDisableDepthTest();

	ui.draw();
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
	scene.selected.clear();
	ui.objectGUI.clear();

	vector<SceneObject *> hits;

	glm::vec3 p = theCam->screenToWorld(glm::vec3(x, y, 0));
	glm::vec3 d = p - theCam->getPosition();
	glm::vec3 dn = glm::normalize(d);

	for (int i = 0; i < scene.objects.size(); i++) {
		glm::vec3 point, normal;
		glm::vec2 uv;
		if (scene.objects[i]->intersect(Ray(p, dn), point, normal, uv)) {
			hits.push_back(scene.objects[i]);
		}
	}

	for (int i = 0; i < scene.lights.size(); i++) {
		glm::vec3 point, normal;
		glm::vec2 luv;
		if (scene.lights[i]->intersect(Ray(p, dn), point, normal, luv)) {
			hits.push_back(scene.lights[i]);
		}
	}

	glm::vec3 tempPoint, tempNormal;
	glm::vec2 ruv;
	if (scene.renderCam.intersect(Ray(p, dn), tempPoint, tempNormal, ruv)) {
		hits.push_back(&(scene.renderCam));
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
	for (int i = 0; i < scene.objects.size(); i++) {
		scene.objects[i]->isSelected = false;
	}
	for (int i = 0; i < scene.lights.size(); i++) {
		scene.lights[i]->isSelected = false;
	}

	scene.renderCam.isSelected = false;

	if (selectedObj) {
		selectedObj->isSelected = true;
		ui.selectedMaterial = selectedObj->objMaterial.toString();
		scene.selected.push_back(selectedObj);
		ui.hideGUI = false;
		ui.updateGUI(selectedObj);
	}
	else {	
		scene.selected.clear();
		ui.objectGUI.clear();
		ui.hideGUI = true;
	}
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
	default:
		break;
	}
}

// Allows the user to drag in a .obj file into the scene
void ofApp::dragEvent(ofDragInfo dragInfo) {
	if (dragInfo.files.size() > 1) {
		dragInfo.files.clear();
	}

	for (int i = 0; i < dragInfo.files.size(); i++) {
		string dir = dragInfo.files[i];
		char * file = new char[dir.length() + 1];
		strcpy(file, dir.c_str());

		scene.FileLoader(file);
		cout << "files = " << dragInfo.files[i] << endl;
	}
}
