#include "ofApp.h"

/*************************************************************************
* ------------------- 3D MODELING AND RENDERING -------------------------
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
void ofApp::setup(){
	// Camera Setup
	mainCam = &interactiveCam;
	interactiveCam.setTarget(ZERO_VECTOR);
	interactiveCam.setDistance(12);
	interactiveCam.setNearClip(NEAR_CLIP);
	sideCam.setPosition(glm::vec3(5, 0, 0));
	sideCam.lookAt(ZERO_VECTOR);
	sideCam.setNearClip(NEAR_CLIP);
	previewCam.setNearClip(NEAR_CLIP);

	// Lighting Setup
	ofSetBackgroundColor(ofColor(30, 30, 30));
	light.enable();
	light.setDiffuseColor(ofColor(255.0f, 255.0f, 255.0f));
	light.setSpecularColor(ofColor(255.0f, 255.0f, 255.0f));

	// create reasonable 3x2 aspect for the RenderCam view plane
	RenderCam renderCam(glm::vec2(-1.5, -1), glm::vec2(1.5, 1), glm::vec3(0, 0, -3.5), glm::vec3(0, 0, 12));
	scene = new Scene(renderCam);
	scene->setup();

	ui = new UI(scene);
	ui->setup();

}

//--------------------------------------------------------------
void ofApp::update(){
	previewCam.setPosition(scene->getRenderCam()->getPosition());
	previewCam.setOrientation(scene->getRenderCam()->getRotateMatrix());
	ui->update();
	light.setGlobalPosition(mainCam->getPosition());
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofEnableDepthTest();
	mainCam = (ui->getToggleRenderCam()) ? &previewCam : &interactiveCam;
	mainCam->begin();
	ui->drawGrid();

	ofEnableLighting();
	material.begin();
	ofFill();
	ofSetColor(ofColor::white);

	// Draw 3D Scene objects
	scene->draw();

	mainCam->end();
	ofDisableLighting();
	ofDisableDepthTest();

	// Draw UI elements
	ui->draw();
}

//--------------------------------------------------------------
// Shortcut Keys
void ofApp::keyPressed(int key) {
	switch (key) {
	case OF_KEY_F1: mainCam = &interactiveCam;
		break;
	case OF_KEY_F2: mainCam = &sideCam;
		break;
	case OF_KEY_F3: mainCam = &previewCam;
		break;
	case 'a': scene->AddSphere();
		break;
	case '0': scene->AddCube();
		break;
	case '1': scene->AddPlane();
		break;
	case '2': scene->AddCylinder();
		break;
	case '3': scene->AddCone();
		break;
	case '4': scene->AddTorus();
		break;
	case '5': scene->AddLSystem();
		break;
	case '6': scene->AddMesh();
		break;
	case '7': scene->AddPointLight();
		break;
	case '8': scene->AddSpotLight();
		break;
	case '9': scene->AddAreaLight();
		break;
	case 's': scene->HandleSaveImage();
		break;	
	case 'n': scene->HandleRename();
		break;
	case 'd': scene->HandleClearScene();
		break;
	case 'x': scene->HandleDelete();
		break;
	case 'b': ui->toggleBVH();
		break;
	case 'g': ui->toggleGrid();
		break;
	case 'r': ui->toggleImage();
		break;
	case '.': ui->toggleRenderView();
		break;
	default:
		break;
	}
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	ui->mousePressed(x, y, button);

	bool hit = false;
	SceneObject *currentSelected = scene->getObjectSelected();

	glm::vec3 rayOrigin = mainCam->screenToWorld(glm::vec3(x, y, 0));
	glm::vec3 rayDir = glm::normalize(rayOrigin - mainCam->getPosition());
	Ray mouseRay = Ray(rayOrigin, rayDir);



	// Get nearest selected object
	float nearestDist = FLT_MAX;
	SceneObject *newSelected = nullptr;
	for (SceneObject *object : scene->getObjects()) {
		HitInfo hitInfo;
		if (object->intersect(mouseRay, hitInfo)) {
			float dist = glm::distance(mouseRay.p, hitInfo.point);
			if (dist < nearestDist) {
				nearestDist = dist;
				newSelected = object;
			}
		}
	}

	for (Light *light : scene->getLights()) {
		HitInfo hitInfo;
		if (light->intersect(mouseRay, hitInfo)) {
			float dist = glm::distance(mouseRay.p, hitInfo.point);
			if (dist < nearestDist) {
				nearestDist = dist;
				newSelected = light;
			}
		}
	}

	HitInfo camHitInfo;
	if (scene->getRenderCam()->intersect(mouseRay, camHitInfo)) {
		float dist = glm::distance(mouseRay.p, camHitInfo.point);
		if (dist < nearestDist) {
			nearestDist = dist;
			newSelected = scene->getRenderCam();
		}
	}


	if (newSelected) {
		if (currentSelected != newSelected) {
			if (currentSelected != nullptr) {
				currentSelected->setSelected(false);
			}
			scene->setObjectSelected(newSelected);
			scene->getObjectSelected()->setSelected(true);
			ui->updateGUI(scene->getObjectSelected());
		}
	}
	else {
		if (scene->getObjectSelected() != nullptr) {
			scene->getObjectSelected()->setSelected(false);
			scene->setObjectSelected(nullptr);
			ui->updateGUI(nullptr);
		}
	}


}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 
	if (dragInfo.files.size() > 1) {
		dragInfo.files.clear();
	}

	for (int i = 0; i < dragInfo.files.size(); i++) {
		string dir = dragInfo.files[i];
		char *file = new char[dir.length() + 1];
		strcpy(file, dir.c_str());

		scene->OBJFileLoader(file);
		cout << "Files = " << dragInfo.files[i] << endl;
	}
}
