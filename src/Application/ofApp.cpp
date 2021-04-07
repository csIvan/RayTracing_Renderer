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

	ui.button_rayTrace.addListener(this, &ofApp::handleRayTrace);
	ui.button_rayMarch.addListener(this, &ofApp::handleRayMarch);
	ui.button_saveImage.addListener(this, &ofApp::handleSaveImage);
	ui.button_delete.addListener(this, &ofApp::handleDelete);
	ui.button_sphere.addListener(this, &ofApp::addSphere);
	ui.button_cube.addListener(this, &ofApp::addCube);
	ui.button_plane.addListener(this, &ofApp::addPlane);
	ui.button_cylinder.addListener(this, &ofApp::addCylinder);
	ui.button_cone.addListener(this, &ofApp::addCone);
	ui.button_torus.addListener(this, &ofApp::addTorus);
	ui.button_mesh.addListener(this, &ofApp::addMesh);
	ui.button_lsystem.addListener(this, &ofApp::addLSystem);
	ui.button_point_light.addListener(this, &ofApp::addPointLight);
	ui.button_spot_light.addListener(this, &ofApp::addSpotLight);
	ui.button_area_light.addListener(this, &ofApp::addAreaLight);

	nearestDistance = FLT_MAX;
	//f1 = LSystem(glm::vec3(0, -2.5, 0), 1, ofColor::seaGreen);
	//wp1 = WaterPool(glm::vec3(1.5, -3, -1), 1, ofColor::mediumPurple);
	//torus1 = Torus(glm::vec3(-1.3, -1.2, 0), 1, 0.5, ofColor::seaGreen);
	//torus2 = Torus(glm::vec3(2.2, -0.4, -2), 2, 0.2, 40.0f, glm::vec3(1, -1, 0), ofColor::orangeRed);
	ui.setup();
}

//--------------------------------------------------------------
void ofApp::update() {
	previewCam.setPosition(renderCam.position);
	previewCam.setOrientation(renderCam.getRotateMatrix());
	ui.update();

	lightScene.setGlobalPosition(theCam->getPosition());
	if (selected.size() > 0) {
		ui.label_material = ofToString(ui.selectedMaterial);
		ui.updateSelected(selected[0]);
		ui.objectGUI.maximize();
		mainCam.setControlArea(ofRectangle(ui.sceneGUI.getWidth(), 0, ofGetWidth() - ui.sceneGUI.getWidth() - ui.objectGUI.getWidth(), ofGetHeight()));

	}
	mainCam.setControlArea(ofRectangle(ui.sceneGUI.getWidth(), 0, ofGetWidth() - ui.sceneGUI.getWidth(), ofGetHeight()));
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofEnableDepthTest();

	theCam = ((bool)ui.toggle_render_cam) ? &previewCam : &mainCam;
	theCam->begin();

	if ((bool)ui.toggle_image && renderFinished) {
		ofPushMatrix();
			ofMultMatrix(renderCam.Transform);
			image.draw(glm::vec3(renderCam.view.bottomLeft()), renderCam.view.width(), renderCam.view.height());
		ofPopMatrix();
	}
	// Draw Grid
	if ((bool)ui.toggle_grid) {
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

	ui.draw();
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
	selected.clear();
	ui.objectGUI.clear();

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
		ui.selectedMaterial = selectedObj->objMaterial.toString();
		selected.push_back(selectedObj);
		ui.hideGUI = false;
		ui.updateGUI(selectedObj);
	}
	else {	
		selected.clear();
		ui.objectGUI.clear();
		ui.hideGUI = true;
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
	image = rayTracer.render(ui.samples);
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
	image = rayMarcher.render(ui.samples);
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
			ui.hideGUI = true;
			rayTracer.removeObject(scene[i]->objName);
			rayMarcher.removeObject(scene[i]->objName);
			scene.erase(std::remove(scene.begin(), scene.end(), scene[i]), scene.end());
			selected.clear();
		}
	}
	for (int i = 0; i < lights.size(); i++) {
		if (selected.size() > 0 && lights[i]->objName == selected[0]->objName) {
			ui.hideGUI = true;
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
		//hideGrid = !hideGrid;
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
