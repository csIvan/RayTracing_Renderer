#include "Scene.h"

void Scene::setup() {
	renderCam.objName = "Render_Cam";
	image.allocate(imageWidth, imageHeight, ofImageType::OF_IMAGE_COLOR);

	rayTracer = RayTracer(imageWidth, imageHeight, image, renderCam);
	rayMarcher = RayMarcher(imageWidth, imageHeight, image, renderCam);
	nearestDistance = FLT_MAX;
	addCube();
	addPointLight();
}

void Scene::update() {}

void Scene::draw() {
	for (int i = 0; i < objects.size(); i++) {
		objects[i]->applyMatrix();
		objects[i]->draw();
	}

	// Display acceleration
	if (showBVH) {
		bvh.draw();
	}

	ofDisableLighting();

	for (int i = 0; i < lights.size(); i++) {
		lights[i]->draw();
	}
	renderCam.draw();
}


void Scene::multithreadRender(Renderer *r) {
	// Create 16 threads
	float p = 0;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++) {
			threads[i * 4 + j].setup(r, glm::vec2(i, j), glm::vec2(imageWidth, imageHeight), samples, p);
		}
	for (int i = 0; i < 16; i++)
		threads[i].startThread();

	for (int i = 0; i < 16; i++)
		threads[i].waitForThread();
	printf("\rRendering... 100%%");
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
	rayTracer.setBVH(&bvh);
	//image = rayTracer.render(samples);	// single thread
	rayTracer.setShader();
	multithreadRender(&rayTracer);
	image = rayTracer.getImage();
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
	//image = rayMarcher.render(samples);	// single thread
	rayMarcher.setShader();
	multithreadRender(&rayMarcher);
	image = rayMarcher.getImage();
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
	bvh.create(objects);
}

void Scene::handleClearScene() {
	// Deleted all objects
	while (objects.size() > 0) {
		for (SceneObject *o : objects) {
			rayTracer.removeObject(o->objName);
			rayMarcher.removeObject(o->objName);
			objects.erase(std::remove(objects.begin(), objects.end(), o), objects.end());
		}
	}

	while (lights.size() > 0) {
		for (Light *l : lights) {
			rayTracer.removeLight(l->objName);
			rayMarcher.removeLight(l->objName);
			lights.erase(std::remove(lights.begin(), lights.end(), l), lights.end());
		}
	}
	selected.clear();
	bvh.create(objects);
}

void Scene::handleRemoveTexture() {
	for (int i = 0; i < objects.size(); i++) {
		if (selected.size() > 0 && objects[i]->objName == selected[0]->objName) {
			objects[i]->objTexture.removeTexture();
		}
	}
}

void Scene::addObject(SceneObject *s) {
	objects.push_back(s);
	rayTracer.addObject(*s);
	rayMarcher.addObject(*s);
	bvh.create(objects);
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
	LSystem *ls = new LSystem(glm::vec3(0, 0, 0), 1, "F", "LSystem_" + to_string(++lsystemCount), ofColor::seaGreen);
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

	MeshObject *tempObj;
	vector<MeshObject *> mObjects;
	bool firstObject = true;

	ifstream objFile;
	objFile.open(string(path));
	string line, matLibName;
	int vCount = 0, nCount = 0, tCount = 0;
	int vstart = 1, nstart = 1, tstart = 1;

	string objDir = string(path);
	string dir = objDir.substr(0, objDir.find_last_of("\\")) + "\\";

	vector <string> tokens;
	while (getline(objFile, line, '\n')) {
		stringstream   linestream(line);
		string token;
		linestream >> token;

		if (token == "mtllib") {
			string name;
			linestream >> name;
			matLibName = name;
		}
		else if (token == "o") {
			string name;
			linestream >> name;
			if (firstObject) {
				tempObj = new MeshObject();
				mObjects.push_back(tempObj);
				firstObject = false;				
			}
			else {
				tempObj->processData(vstart, nstart, tstart, tempIndices, tempVertNormIndices, tempVertTexIndices);
				vstart += vCount;
				nstart += nCount;
				tstart += tCount;
				vCount = 0, nCount = 0, tCount = 0;
				tempIndices.clear();
				tempVertNormIndices.clear();
				tempVertTexIndices.clear();
				tempObj = new MeshObject();
				mObjects.push_back(tempObj);
			}
		}
		else if (token == "usemtl") {
			string name;
			linestream >> name;
			tempObj->setMtlName(name);
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
				if ((v.find("//") != std::string::npos)) {
					break;
				}
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
				}
			}
		}
	}

	if (!firstObject) {
		tempObj->processData(vstart, nstart, tstart, tempIndices, tempVertNormIndices, tempVertTexIndices);
		tempIndices.clear();
		tempVertNormIndices.clear();
		tempVertTexIndices.clear();
	}
	objFile.close();


	string matLibPath = dir + matLibName;

	MeshTextureMap *tempMap;
	vector<MeshTextureMap *> mMaps;
	ifstream matFile;
	matFile.open(string(matLibPath));
	string mline;
	if (matFile) {
		while (getline(matFile, mline, '\n')) {
			stringstream   linestream(mline);
			string token;
			linestream >> token;

			if (token == "newmtl") {
				string name;
				linestream >> name;
				tempMap = new MeshTextureMap();
				tempMap->name = name;
				tempMap->hasTexture = false;
				mMaps.push_back(tempMap);
			}
			else if (token == "Kd") {
				float v1, v2, v3;;
				linestream >> v1 >> v2 >> v3;
				tempMap->kd = glm::vec3(v1, v2, v3);
			}
			else if (token == "map_Kd") {
				string name;
				linestream >> name;

				string mapName;
				bool correctFormat = false;

				if ((name.find("\\") == std::string::npos) && (name.find(".") != std::string::npos)) {
					mapName = name;
					correctFormat = true;
				}


				if (correctFormat) {
					string mapDir = string(path);
					string mapPath = mapDir.substr(0, mapDir.find_last_of("\\")) + "\\" + mapName;

					tempMap->path = mapPath;
					tempMap->hasTexture = true;
				}

			}
		}
	}
	matFile.close();

	Mesh *meshObj = new Mesh(glm::vec3(0, 0, 0), mObjects, mMaps, "Mesh_" + to_string(++meshCount), ofColor::seaGreen);

	objects.push_back(meshObj);
	rayTracer.addObject(*meshObj);
	rayMarcher.addObject(*meshObj);
	bvh.create(objects);

	return true;

}