#include "Scene.h"

void Scene::setup() {
	renderCam.objName = "Render_Cam";
	image.allocate(imageWidth, imageHeight, ofImageType::OF_IMAGE_COLOR);

	rayTracer = RayTracer(imageWidth, imageHeight, image, renderCam);
	rayMarcher = RayMarcher(imageWidth, imageHeight, image, renderCam);
	nearestDistance = FLT_MAX;
	addPlane();
	addPointLight();
}

void Scene::update() {}

void Scene::draw() {
	for (int i = 0; i < objects.size(); i++) {
		objects[i]->draw();
	}

	ofDisableLighting();

	for (int i = 0; i < lights.size(); i++) {
		lights[i]->draw();
	}
	renderCam.draw();
}

void Scene::handleRayTrace() {
	time_t start, end;
	for (int i = 0; i < objects.size(); i++) {
		if (dynamic_cast<LSystem*>(objects[i]) != nullptr) {
			LSystem *lsys = (LSystem*)objects[i];
			cout << "LSystem: " << lsys->sentence << endl;
		}
	}

	time(&start);
	image = rayTracer.render(samples);
	time(&end);

	renderFinished = true;
	cout << "\nFinished Ray Tracing : " << double(end - start) << " sec\n" << endl;
}

void Scene::handleRayMarch() {
	time_t start, end;
	for (int i = 0; i < objects.size(); i++) {
		if (dynamic_cast<LSystem*>(objects[i]) != nullptr) {
			LSystem *lsys = (LSystem*)objects[i];
			cout << "LSystem: " << lsys->sentence << endl;
		}
	}

	time(&start);
	image = rayMarcher.render(samples);
	time(&end);

	renderFinished = true;
	cout << "\nFinished Ray Marching : " << double(end - start) << " sec\n" << endl;
}

void Scene::handleSaveImage() {
	if (renderFinished) {
		ofFileDialogResult result = ofSystemSaveDialog("render.jpg", "Save");
		if (result.bSuccess) {
			image.save(result.getPath(), OF_IMAGE_QUALITY_BEST);
		}
	}
}

void Scene::handleDelete() {
	for (int i = 0; i < objects.size(); i++) {
		if (selected.size() > 0 && objects[i]->objName == selected[0]->objName) {
			rayTracer.removeObject(objects[i]->objName);
			rayMarcher.removeObject(objects[i]->objName);
			objects.erase(std::remove(objects.begin(), objects.end(), objects[i]), objects.end());
			selected.clear();
		}
	}
	for (int i = 0; i < lights.size(); i++) {
		if (selected.size() > 0 && lights[i]->objName == selected[0]->objName) {
			rayTracer.removeLight(lights[i]->objName);
			rayMarcher.removeLight(lights[i]->objName);
			lights.erase(std::remove(lights.begin(), lights.end(), lights[i]), lights.end());
			selected.clear();
		}
	}
}

void Scene::handleRemoveTexture() {
	for (int i = 0; i < objects.size(); i++) {
		if (selected.size() > 0 && objects[i]->objName == selected[0]->objName) {
			objects[i]->objTexture.removeTexture();
		}
	}
}

void Scene::handleRemoveBumpMap() {
	for (int i = 0; i < objects.size(); i++) {
		if (selected.size() > 0 && objects[i]->objName == selected[0]->objName) {
			objects[i]->objTexture.removeBumpMap();
		}
	}
}


void Scene::addObject(SceneObject *s) {
	objects.push_back(s);
	rayTracer.addObject(*s);
	rayMarcher.addObject(*s);
}

void Scene::addLight(Light *light) {
	lights.push_back(light);
	rayTracer.addLight(*light);
	rayMarcher.addLight(*light);
}

void Scene::addSphere() {
	string nameString = "Sphere_" + to_string(2);
	addObject(new Sphere(glm::vec3(0, 0, 0), 1, "Sphere_" + to_string(++sphereCount), ofColor::seaGreen));
}
void Scene::addCube() {
	addObject(new Cube(glm::vec3(0, 0, 0), 2, "Cube_" + to_string(++cubeCount), ofColor::seaGreen));
}
void Scene::addPlane() {
	addObject(new Plane(glm::vec3(0, -3, 0), glm::vec3(0, 1, 0), "Plane_" + to_string(++planeCount), ofColor::dimGrey));
}
void Scene::addCylinder() {
	addObject(new Cylinder(glm::vec3(0, 0, 0), 2, 0.5, "Cylinder_" + to_string(++cylinderCount), ofColor::seaGreen));
}
void Scene::addCone() {
	addObject(new Cone(glm::vec3(0, 0, 0), 2, 0.5, "Cone_" + to_string(++coneCount), ofColor::seaGreen));
}
void Scene::addTorus() {
	addObject(new Torus(glm::vec3(0, 0, 0), 1, 0.5, "Torus_" + to_string(++torusCount), ofColor::seaGreen));
}
void Scene::addMesh() {
	ofFileDialogResult result = ofSystemLoadDialog();
	if (result.bSuccess && result.fileName.substr(result.fileName.find_last_of(".") + 1) == "obj") {
		FileLoader(result.filePath.c_str());
	}
	else {
		cout << "Incorrect file type. Please enter only .obj files\n" << endl;
	}
}
void Scene::addLSystem() {
	LSystem *ls = new LSystem(glm::vec3(0, 0, 0), 1, "F", "LSystem_" + to_string(++lsystemCount));
	ls->generate();
	addObject(ls);
}
void Scene::addWaterPool() {
	addObject(new WaterPool(glm::vec3(0, 0, 0), 1, "WaterPool_" + to_string(++waterpoolCount), ofColor::seaGreen));
}
void Scene::addPointLight() {
	addLight(new Light(glm::vec3(-4, 3, 5), 6.5f, "Point_Light_" + to_string(++pointlightCount)));
}

void Scene::addSpotLight() {
	addLight(new SpotLight(glm::vec3(0, 3, 0), glm::vec3(0, -1, 0), 10.0f, 10.0f, "Spot_Light_" + to_string(++spotlightCount)));
}

void Scene::addAreaLight() {
	addLight(new AreaLight(glm::vec3(0, 3, 0), glm::vec3(0, -1, 0), 1.0f, 1.0f, "Area_Light_" + to_string(++arealightCount)));
}

/**
* File loader function reads a .obj file
* and instantiates a mesh object
*/
bool Scene::FileLoader(const char * path) {
	vector<int> tempIndices;
	vector<int> tempVertNormIndices;
	vector<int> tempVertTexIndices;
	vector<glm::vec3> tempVertices;
	vector<glm::vec3> tempVertNormals;
	vector<glm::vec2> tempVertTextures;
	FILE * file;
	char mtl[100];
	MeshObject *tempObj;
	vector<MeshObject *> mObjects;
	bool firstObject = true;

	ifstream objFile;
	objFile.open(string(path));
	string line;
	int vCount = 0, nCount = 0, tCount = 0;
	int vstart = 1, nstart = 1, tstart = 1;

	vector <string> tokens;
	while (getline(objFile, line, '\n')) {
		stringstream   linestream(line);
		string token;
		linestream >> token;
		//cout << dataT << endl;

		if (token == "mtllib") {
			string name;
			linestream >> name;
			cout << name << endl;
		}
		else if (token == "o") {
			string name;
			linestream >> name;
			if (firstObject) {
				tempObj = new MeshObject(name);
				mObjects.push_back(tempObj);
				firstObject = false;
				
			}
			else {
				cout << "Size: " << vCount << endl;
				cout << "Size2: " << tempIndices.size() << endl;
				tempObj->processData(vstart, nstart, tstart, tempIndices, tempVertNormIndices, tempVertTexIndices);
				vstart += vCount;
				nstart += nCount;
				tstart += tCount;
				vCount = 0, nCount = 0, tCount = 0;
				tempIndices.clear();
				tempVertNormIndices.clear();
				tempVertTexIndices.clear();
				tempObj = new MeshObject(name);
				mObjects.push_back(tempObj);
				cout << "new" << endl;
			}
		}
		else if (token == "v") {
			float v1, v2, v3;
			linestream >> v1 >> v2 >> v3;
			tempObj->vertices.push_back(glm::vec3(v1, v2, v3));
			vCount++;
		}
		else if (token == "vn") {
			float v1, v2, v3;
			linestream >> v1 >> v2 >> v3;
			tempObj->vertNormals.push_back(glm::vec3(v1, v2, v3));
			nCount++;
		}
		else if (token == "vt") {
			float u, v;
			linestream >> u >> v;
			tempObj->vertTextures.push_back(glm::vec2(u, v));
			tCount++;
		}
		else if (token == "f") {
			vector<string> vs;
			for (int i = 0; i < 3; i++) {
				string v;
				linestream >> v;
				stringstream fp(v);

				int count = 0;
				string tok;
				while (getline(fp, tok, '/')) {
					if (count == 0)
						tempIndices.push_back(stoi(tok));
					else if (count == 1) {
						tempVertTexIndices.push_back(stoi(tok));
					}
					else if (count == 2) {
						tempVertNormIndices.push_back(stoi(tok));
					}
					count++;
					//cout << tok << "/";
				}
				//cout << " ";
			}
			//cout << endl;
			//for (int i = 0; i < tempIndices.size(); i++) {
			//	cout << i << " : " << tempIndices[i] << "/" << tempVertTexIndices[i] << "/" << tempVertNormIndices[i] <<  " ";
			//}
			//cout << endl;
		}
	}

	if (!firstObject) {
		tempObj->processData(vstart, nstart, tstart, tempIndices, tempVertNormIndices, tempVertTexIndices);
		tempIndices.clear();
		tempVertNormIndices.clear();
		tempVertTexIndices.clear();
	}
	

	//errno_t err = fopen_s(&file, path, "r");
	//if (err != 0) {
	//	printf("Could not open file!\n");
	//	return false;
	//}
	//while (1) {
	//	char line[128];
	//	int res = fscanf(file, "%s", line);
	//	if (res == EOF) {
	//		if (!firstObject) {
	//			tempObj->processData(tempIndices, tempVertNormIndices, tempVertTexIndices);
	//			cout << "Hey" << endl;
	//			tempIndices.clear();
	//			tempVertNormIndices.clear();
	//			tempVertTexIndices.clear();
	//		}
	//		break;
	//	}

	//	if (strcmp(line, "mtllib") == 0) {
	//		fscanf_s(file, "%s\n", mtl);
	//	}


	//	if (strcmp(line, "o") == 0) {
	//		char n[100];
	//		fscanf_s(file, "%s\n", n);

	//		if (firstObject) {			
	//			tempObj = new MeshObject(n);
	//			mObjects.push_back(tempObj);
	//			firstObject = false;
	//		}
	//		else {
	//			tempObj->processData(tempIndices, tempVertNormIndices, tempVertTexIndices);
	//			tempIndices.clear();
	//			tempVertNormIndices.clear();
	//			tempVertTexIndices.clear();
	//			tempObj = new MeshObject(n);
	//			mObjects.push_back(tempObj);
	//		}
	//	}
	//	

	//	if (strcmp(line, "v") == 0) {
	//		glm::vec3 vertex;
	//		fscanf_s(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
	//		tempObj->vertices.push_back(vertex);
	//	}
	//	else if (strcmp(line, "vn") == 0) {
	//		glm::vec3 vertNormal;
	//		fscanf_s(file, "%f %f %f\n", &vertNormal.x, &vertNormal.y, &vertNormal.z);
	//		tempObj->vertNormals.push_back(vertNormal);
	//	}
	//	else if (strcmp(line, "vt") == 0) {
	//		glm::vec2 vertTex;
	//		fscanf_s(file, "%f %f %f\n", &vertTex.x, &vertTex.y);
	//		tempObj->vertTextures.push_back(vertTex);
	//	}
	//	else if (strcmp(line, "f") == 0) {
	//		unsigned int vertexIndex[3];
	//		unsigned int vertNormIndex[3];
	//		unsigned int vertTexIndex[3];

	//		fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &vertTexIndex[0], &vertNormIndex[0],
	//			&vertexIndex[1], &vertTexIndex[1], &vertNormIndex[1], &vertexIndex[2], &vertTexIndex[2], &vertNormIndex[2]);
	//		tempIndices.push_back(vertexIndex[0]);
	//		tempIndices.push_back(vertexIndex[1]);
	//		tempIndices.push_back(vertexIndex[2]);
	//		tempVertTexIndices.push_back(vertTexIndex[0]);
	//		tempVertTexIndices.push_back(vertTexIndex[1]);
	//		tempVertTexIndices.push_back(vertTexIndex[2]);
	//		tempVertNormIndices.push_back(vertNormIndex[0]);
	//		tempVertNormIndices.push_back(vertNormIndex[1]);
	//		tempVertNormIndices.push_back(vertNormIndex[2]);
	//	}
	//}
	//cout << "yo" << endl;
	//fclose(file);

	//

	//// Read .MTL file
	//string dir = string(path);
	//string matLib = dir.substr(0, dir.find_last_of("\\")) + "\\" + mtl;
	//char *matLibDir = new char[matLib.length() + 1];
	//strcpy(matLibDir, matLib.c_str());

	//FILE *mtlfile;
	//errno_t mtlErr = fopen_s(&mtlfile, matLibDir, "r");
	//if (mtlErr != 0) {
	//	printf("Could not open file!\n");
	//	return false;
	//}

	//vector<string> mtlNames;
	//vector<string> mtlPaths;
	//vector<glm::vec3> mtlKds;

	//while (1) {
	//	char line[128];
	//	int res = fscanf(mtlfile, "%s", line);
	//	if (res == EOF)
	//		break;

	//	if (strcmp(line, "newmtl") == 0) {
	//		char mtlName[100];
	//		fscanf_s(mtlfile, "%s\n", mtlName);
	//		mtlNames.push_back(mtlName);
	//	}
	//	else if (strcmp(line, "Kd") == 0) {
	//		glm::vec3 kd;
	//		fscanf_s(mtlfile, "%f %f %f\n", &kd.x, &kd.y, &kd.z);
	//		mtlKds.push_back(kd);
	//	}
	//	else if (strcmp(line, "map_Kd") == 0) {
	//		char mtlPath[100];
	//		fscanf_s(mtlfile, "%s\n", mtlPath);
	//		mtlPaths.push_back(dir.substr(0, dir.find_last_of("\\")) + "\\" + mtlPath);
	//	}
	//}

	//for (int i = 0; i < mtlNames.size(); i++) {
	//	cout << "Name: " << mtlNames[i] << endl << "Path: " << mtlPaths[i] << endl << "Kd: " << mtlKds[i] << endl << endl;
	//}

	Mesh *meshObj = new Mesh(glm::vec3(0, 0, 0), mObjects, "Mesh_" + to_string(++meshCount), ofColor::seaGreen);

	objects.push_back(meshObj);
	rayTracer.addObject(*meshObj);
	rayMarcher.addObject(*meshObj);
	
	return true;

	// Display diagnostic information
	//cout << "Number of Vertices: " << meshVertices.size() << endl;
	//cout << "Number of Faces: " << meshTris.size() << endl;
	//cout << "Number of Vertex Normals: " << meshVerticesNormals.size() << endl;
}