#include "ofApp.h"

/*************************************************************************
* ------------------- RAY TRACING / RAY MARCHING -------------------------
* Author: Ivan Hernandez
* Description: This project allows the user to render a scene using either
* the Ray Tracing or Ray Marching algorithm. The User Interface provides
* several options to create and manipulate scene objects. The project has 
* smooth shading for mesh(.obj only) objects as well as texturing for 
* supported files(.png, .jpg, and .tga). A command console is provided to
* show the rendering progress and information regarding specific objects,
* such as Lsystem strings or mesh vertices. Also, shortcuts are indicated
* next to their correspontding user interface buttons.
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

	for (SceneObject *o : scene.objects) {
		glm::vec3 point, normal;
		ofColor surfaceColor;
		if (o->intersect(Ray(p, dn), point, normal, surfaceColor)) {
			hits.push_back(o);
		}
	}

	for (Light *l : scene.lights) {
		glm::vec3 point, normal;
		ofColor lsurfaceColor;
		if (l->intersect(Ray(p, dn), point, normal, lsurfaceColor)) {
			hits.push_back(l);
		}
	}

	glm::vec3 tempPoint, tempNormal;
	ofColor rsurfaceColor;
	if (scene.renderCam.intersect(Ray(p, dn), tempPoint, tempNormal, rsurfaceColor)) {
		hits.push_back(&(scene.renderCam));
	}

	SceneObject *selectedObj = NULL;
	if (hits.size() > 0) {
		selectedObj = hits[0];
		float nearestDist = FLT_MAX;
		for (int n = 1; n < hits.size(); n++) {
			float dist = glm::length(hits[n]->position - theCam->getPosition());
			if (dist < nearestDist) {
				nearestDist = dist;
				selectedObj = hits[n];
			}
		}
	}
	for (SceneObject *o : scene.objects) {
		o->isSelected = false;
	}
	for (Light *l : scene.lights) {
		l->isSelected = false;
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


// Shortcut Keys
void ofApp::keyPressed(int key) {
	switch (key) {
	case OF_KEY_F1: theCam = &mainCam;
		break;
	case OF_KEY_F2: theCam = &sideCam;
		break;
	case OF_KEY_F3: theCam = &previewCam;
		break;
	case 'a': scene.addSphere();
		break;
	case '0': scene.addCube();
		break;
	case '1': scene.addPlane();
		break;
	case '2': scene.addCylinder();
		break;
	case '3': scene.addCone();
		break;
	case '4': scene.addTorus();
		break;
	case '5': scene.addMesh();
		break;
	case '6': scene.addLSystem();
		break;
	case '7': scene.addPointLight();
		break;
	case '8': scene.addSpotLight();
		break;
	case '9': scene.addAreaLight();
		break;
	case 's': scene.handleSaveImage();
		break;	
	case 'n': scene.handleRename();
		break;
	case 'd': scene.handleClearScene();
		break;
	case 'b': ui.toggle_bvh = !ui.toggle_bvh;
		break;
	case 'g': ui.toggle_grid = !ui.toggle_grid;
		break;
	case 'r': ui.toggle_image = !ui.toggle_image;
		break;
	case '.': ui.toggle_render_cam = !ui.toggle_render_cam;
		break;
	case 'x': scene.handleDelete();
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
