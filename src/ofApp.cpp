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
	ofSetBackgroundColor(ofColor::black);
	theCam = &mainCam;
	mainCam.setDistance(12);
	mainCam.setNearClip(.1);
	sideCam.setPosition(glm::vec3(5, 0, 0));
	sideCam.lookAt(glm::vec3(0, 0, 0));
	sideCam.setNearClip(.1);
	previewCam.setPosition(glm::vec3(0, 0, 10));
	previewCam.lookAt(glm::vec3(0, 0, -1));
	previewCam.setNearClip(.1);

	//spotlight
	sp1 = SpotLight(glm::vec3(.5, -1, -.55), .1, 4);
	sp1.position = glm::vec3(-3, 4, 1);

	//point light 
	light1 = Light(glm::vec3(-4, 3, 5), 50);
	light2 = Light(glm::vec3(0, 5, -25), 50);
	light3 = Light(glm::vec3(0, 5, -50), 50);
	light4 = Light(glm::vec3(0, 5, -75), 50);

	//lights.push_back(&sp1);
	lights.push_back(&light1);
	//lights.push_back(&light2);
	//lights.push_back(&light3);
	//lights.push_back(&light4);

	image.allocate(imageWidth, imageHeight, ofImageType::OF_IMAGE_COLOR);

	//texture.load("images/texture2.jpg");
	//sphereTexture.load("images/WorldMap.jpg");

	rayTracer = RayTracer(imageWidth, imageHeight, image);
	rayMarcher = RayMarcher(imageWidth, imageHeight, image);

	nearestDistance = FLT_MAX;
	//sphere1 = Sphere(glm::vec3(1.5, -1, -1), 1, ofColor::mediumPurple);
	sphere1 = Sphere(glm::vec3(-.1, 1, -.28), 1.5, ofColor::mediumPurple);
	cube1 = Cube(glm::vec3(0, 0, 0), 2, ofColor::seaGreen);
	f1 = LSystem(glm::vec3(0, -2.5, 0), 1, ofColor::seaGreen);
	wp1 = WaterPool(glm::vec3(1.5, -3, -1), 1, ofColor::mediumPurple);
	plane1 = Plane(glm::vec3(0, -3.25, 0), glm::vec3(0, 1, 0), ofColor::lightBlue);
	torus1 = Torus(glm::vec3(-1.3, -1.2, 0), 1, 0.5, ofColor::seaGreen);
	torus2 = Torus(glm::vec3(2.2, -0.4, -2), 2, 0.2, 65.0f, glm::vec3(1, 0, 1), ofColor::orangeRed);
	//scene.push_back(&sphere1);
	scene.push_back(&cube1);
	//scene.push_back(&f1);
	//scene.push_back(&wp1);
	scene.push_back(&plane1);
	//scene.push_back(&torus1);
	//scene.push_back(&torus2);

	rayTracer.addObject(sphere1);
	rayTracer.addObject(plane1);
	rayTracer.addLight(light1);

	rayMarcher.addObject(cube1);
	rayMarcher.addObject(plane1);
	rayMarcher.addLight(light1);
}

//--------------------------------------------------------------
void ofApp::update() {

}

//--------------------------------------------------------------
void ofApp::draw() {
	theCam->begin();
	ofSetColor(ofColor::white);
	//ofDrawAxis(3);

	for (int i = 0; i < lights.size(); i++) {
		lights[i]->draw();
	}

	for (int i = 0; i < scene.size(); i++) {
		scene[i]->draw();
	}

	ofNoFill();
	renderCam.draw();
	renderCam.drawFrustum();
	theCam->end();
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
	case 'r': rayTracer.render();
		cout << "done" << endl;
		break;
	case 'm': rayMarcher.render();
		cout << "done" << endl;
		break;
	default:
		break;
	}
}

/**
* File loader class from the first project. It reads a .obj file
* and stores the information regarding the vertices, vertex normals, and faces.
*/
bool ofApp::FileLoader(char * path) {
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

	for (unsigned int i = 0; i < tempIndices.size(); i++) {
		unsigned int vertexIndex = tempIndices[i];
		glm::vec3 vertex = tempVertices[vertexIndex - 1];
		mesh.vertices.push_back(vertex);
	}
	for (unsigned int i = 0; i < tempVertNormIndices.size(); i++) {
		unsigned int vertNormIndex = tempVertNormIndices[i];
		glm::vec3 vn = tempVertNormals[vertNormIndex - 1];
		mesh.vertNormals.push_back(vn);
	}

	int count = 0;
	vector<int> temp;
	for (unsigned int i = 0; i < mesh.vertices.size(); i++) {
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
			mesh.tris.push_back(triangle);
			temp.clear();
			count = 0;
		}
	}
	scene.push_back(&mesh);

	// Display diagnostic information
	cout << "Number of Vertices: " << mesh.vertices.size() << endl;
	cout << "Number of Faces: " << mesh.tris.size() << endl;
	cout << "Number of Vertex Normals: " << mesh.vertNormals.size() << endl;
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
