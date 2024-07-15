#include "Scene.h"

//--------------------------------------------------------------
void Scene::setup() {
	AddPointLight();
	AddCube();

	string renderCamName = "Render_Cam";
	renderCam.setName(renderCamName);

	rayTracer = new RayTracer();
	rayMarcher = new RayMarcher();
	buildBVH();
}

//--------------------------------------------------------------
void Scene::update() {

}

//--------------------------------------------------------------
void Scene::draw() {
	ofEnableLighting();
	for (int i = 0; i < objects.size(); i++) {
		objects[i]->draw();
	}

	if (showBVH) {
		bvh.draw();
	}

	ofDisableLighting();

	ofNoFill();
	if (renderFinished && showImage) {
		ofPushMatrix();
		ofMultMatrix(renderCam.getTransform());
		renderedImage.draw(glm::vec3(renderCam.getViewPlane().bottomLeft()), renderCam.getViewPlane().width(), renderCam.getViewPlane().height());
		ofPopMatrix();
	}

	for (int i = 0; i < lights.size(); i++) {
		lights[i]->draw();
	}

	renderCam.draw();
	ofFill();

	ofEnableLighting();
}

//--------------------------------------------------------------
void Scene::HandleRayTrace() {
	for (int i = 0; i < objects.size(); i++) {
		if (dynamic_cast<LSystem *>(objects[i]) != nullptr) {
			LSystem *lsys = (LSystem *)objects[i];
			cout << "LSystem: " << lsys->getSentence() << endl;
		}
	}

	time_t start, end;
	time(&start);

	//renderedImage = rayTracer->render(renderSamples, renderCam, *this);	// single thread
	renderedImage = rayTracer->renderMT(16, renderSamples, renderCam, *this);

	time(&end);
	renderFinished = true;
	std::printf("\rRendering... 100%%");
	cout << "\nFinished Ray Tracing : " << static_cast<float>(end - start) << " sec\n" << endl;
}

//--------------------------------------------------------------
void Scene::HandleRayMarch() {
	for (int i = 0; i < objects.size(); i++) {
		if (dynamic_cast<LSystem *>(objects[i]) != nullptr) {
			LSystem *lsys = (LSystem *)objects[i];
			cout << "LSystem: " << lsys->getSentence() << endl;
		}
	}

	time_t start, end;
	time(&start);

	//renderedImage = rayMarcher->render(renderSamples, renderCam, *this);	// single thread
	renderedImage = rayMarcher->renderMT(16, renderSamples, renderCam, *this);

	time(&end);
	renderFinished = true;
	std::printf("\rRendering... 100%%");
	cout << "\nFinished Ray Marching: " << static_cast<float>(end - start) << " sec\n" << endl;

}

//--------------------------------------------------------------
void Scene::HandleSaveImage() {
	if (renderFinished) {
		ofFileDialogResult result = ofSystemSaveDialog("render.jpg", "Save");
		if (result.bSuccess) {
			renderedImage.save(result.getPath(), OF_IMAGE_QUALITY_BEST);
		}
	}
}

//--------------------------------------------------------------
void Scene::HandleRename() {
	if (getObjectSelected() != nullptr && getObjectSelected()->getName() != "Render_Cam") {
		string result = ofSystemTextBoxDialog("Enter a New Name", "");

		bool alreadyExists = false;
		for (SceneObject *object : objects) {
			if (object->getName() == result) {
				alreadyExists = true;
			}
		}

		for(Light *light : lights){
			if (light->getName() == result) {
				alreadyExists = true;
			}
		}

		if ((result.length() > 0 && result.length() < 20) && !alreadyExists) {
			cout << "Name change confirmed: " << result << endl;
			getObjectSelected()->setName(result);
		}
		else {
			cout << "Name change denied (must be unique and 1 to 20 characters long)" << endl;
			setObjectSelected(nullptr);
		}
	}
}

//--------------------------------------------------------------
void Scene::HandleDelete() {
	for (int i = 0; i < objects.size(); i++) {
		if (objects[i] == getObjectSelected()) {
			objects[i]->setSelected(false);
			objects.erase(std::remove(objects.begin(), objects.end(), objects[i]), objects.end());

		}
	}

	for (int i = 0; i < lights.size(); i++) {
		if (lights[i] == getObjectSelected()) {
			lights[i]->setSelected(false);
			lights.erase(std::remove(lights.begin(), lights.end(), lights[i]), lights.end());
		}
	}
	setObjectSelected(nullptr);
	buildBVH();
}

//--------------------------------------------------------------
void Scene::HandleClearScene() {
	while (objects.size() > 0) {
		for (SceneObject *object : objects) {
			objects.erase(std::remove(objects.begin(), objects.end(), object), objects.end());
		}
	}
	
	while (lights.size() > 0) {
		for (Light *light : lights) {
			lights.erase(std::remove(lights.begin(), lights.end(), light), lights.end());
		}
	}
	setObjectSelected(nullptr);
	buildBVH();
}

//--------------------------------------------------------------
void Scene::HandleLoadTexture() {
	if (getObjectSelected() != nullptr) {
		ofFileDialogResult result = ofSystemLoadDialog("Choose image(.jpg, .png) texture file");
		if (result.bSuccess) {
			getObjectSelected()->setTextureImage(result.getPath());
			cout << "Textured" << endl;
		}
	}
}

//--------------------------------------------------------------
void Scene::HandleRemoveTexture() {
	for (int i = 0; i < getObjects().size(); i++) {
		if (getObjectSelected() != nullptr) {
			getObjectSelected()->removeTextureImage();
			break;
		}
	}
}


//--------------------------------------------------------------
void Scene::buildBVH() {
	bvh.create(objects);
}

//--------------------------------------------------------------
void Scene::AddObject(SceneObject *object) {
	objects.push_back(object);
	buildBVH();
}

//--------------------------------------------------------------
void Scene::AddLight(Light *light) {
	lights.push_back(light);
}

//--------------------------------------------------------------
void Scene::AddSphere() {
	AddObject(new Sphere(ZERO_VECTOR, 1.0f, "Sphere_" + to_string(++objectCount["Sphere"]), ofColor::seaGreen));
}

//--------------------------------------------------------------
void Scene::AddCube() {
	AddObject(new Cube(ZERO_VECTOR, 1.0f, "Cube_" + to_string(++objectCount["Cube"]), ofColor::seaGreen));
}

//--------------------------------------------------------------
void Scene::AddPlane() {
	AddObject(new Plane(glm::vec3(0, -3, 0), glm::vec3(0, 1, 0), "Plane_" + to_string(++objectCount["Plane"]), 10, 10, ofColor::seaGreen));
}

//--------------------------------------------------------------
void Scene::AddCylinder() {
	AddObject(new Cylinder(ZERO_VECTOR, 2.0f, 0.5f, "Cylinder_" + to_string(++objectCount["Cylinder"]), ofColor::seaGreen));
}

//--------------------------------------------------------------
void Scene::AddCone() {
	AddObject(new Cone(ZERO_VECTOR, 2.0f, 0.5f, "Cone_" + to_string(++objectCount["Cone"]), ofColor::seaGreen));
}

void Scene::AddTorus() {
	AddObject(new Torus(ZERO_VECTOR, 0.5f, 1.0f, "Torus_" + to_string(++objectCount["Torus"]), ofColor::seaGreen));
}

//--------------------------------------------------------------
void Scene::AddLSystem() {
	LSystem *ls = new LSystem(ZERO_VECTOR, 1, "F", "LSystem_" + to_string(++objectCount["LSystem"]), ofColor::seaGreen);
	ls->generate();
	AddObject(ls);
}

//--------------------------------------------------------------
void Scene::AddMesh() {
	ofFileDialogResult result = ofSystemLoadDialog();
	if (result.bSuccess && result.fileName.substr(result.fileName.find_last_of(".") + 1) == "obj") {
		OBJFileLoader(result.filePath.c_str());
	}
	else {
		cout << "Incorrect file type. Please enter only .obj files\n" << endl;
	}
}

//--------------------------------------------------------------
void Scene::AddPointLight() {
	AddLight(new Light(glm::vec3(-4, 3, 5), 6.0f, "Point_Light_" + to_string(++objectCount["PointLight"])));
}

//--------------------------------------------------------------
void Scene::AddSpotLight() {
	AddLight(new SpotLight(glm::vec3(0, 3, 0), glm::vec3(0, -1, 0), 6.0f, 10.0f, 10.0f, "SpotLight_" + to_string(++objectCount["SpotLight"])));
}

//--------------------------------------------------------------
void Scene::AddAreaLight() {
	AddLight(new AreaLight(glm::vec3(0, 3, 0), glm::vec3(0, -1, 0), 6.0f, 1.0f, 1.0f, "AreaLight_" + to_string(++objectCount["AreaLight"])));
}


//--------------------------------------------------------------
void Scene::OBJFileLoader(const char *path) {
	vector<int> vertexIndices;
	vector<int> normalIndices;
	vector<int> textureIndices;
	vector<glm::vec3> vertices;
	vector<glm::vec3> normals;
	vector<glm::vec2> textures;

	MeshObject *currentMesh = nullptr;
	vector<MeshObject *> meshObjects;

	ifstream objFile(path);
	string line, matLibName;
	int vertexCount = 0, normalCount = 0, textureCount = 0;
	int vertexStart = 1, normalStart = 1, textureStart = 1;

	string directory = static_cast<string>(path).substr(0, static_cast<string>(path).find_last_of("\\")) + "\\";

	while (getline(objFile, line, '\n')) {
		stringstream   linestream(line);
		string token;
		linestream >> token;

		if (token == "mtllib") {
			linestream >> matLibName;
		}
		else if (token == "o") {
			if (currentMesh != nullptr) {
				currentMesh->processData(vertexStart, normalStart, textureStart, vertexIndices, normalIndices, textureIndices);
				vertexStart += vertexCount;
				normalStart += normalCount;
				textureStart += textureCount;
				vertexCount = 0, normalCount = 0, textureCount = 0;
				vertexIndices.clear();
				normalIndices.clear();
				textureIndices.clear();
			}
			currentMesh = new MeshObject();
			meshObjects.push_back(currentMesh);
		}
		else if (token == "usemtl") {
			string name;
			linestream >> name;
			currentMesh->setMtlName(name);
		}
		else if (token == "v") {
			glm::vec3 vertex;
			linestream >> vertex.x >> vertex.y >> vertex.z;
			currentMesh->vertices.push_back(vertex);
			vertexCount++;
		}
		else if (token == "vn") {
			glm::vec3 normal;
			linestream >> normal.x >> normal.y >> normal.z;
			currentMesh->vertNormals.push_back(normal);
			normalCount++;
		}
		else if (token == "vt") {
			glm::vec2 texture;
			linestream >> texture.x >> texture.y;
			currentMesh->vertTextures.push_back(texture);
			textureCount++;
		}
		else if (token == "f") {
			for (int i = 0; i < 3; i++) {
				string vertexData;
				linestream >> vertexData;
				stringstream vertexStream(vertexData);
				if ((vertexData.find("//") != std::string::npos)) {
					break;
				}

				string index;
				int count = 0;
				while (getline(vertexStream, index, '/')) {
					if (count == 0)
						vertexIndices.push_back(stoi(index));
					else if (count == 1) {
						textureIndices.push_back(stoi(index));
					}
					else if (count == 2) {
						normalIndices.push_back(stoi(index));
					}
					count++;
				}
			}
		}
	}

	if (currentMesh) {
		currentMesh->processData(vertexStart, normalStart, textureStart, vertexIndices, normalIndices, textureIndices);
		vertexIndices.clear();
		normalIndices.clear();
		textureIndices.clear();
	}
	objFile.close();


	ifstream matFile(directory + matLibName);
	vector<MeshTexture *> materialTextures;
	MeshTexture *currentTexture = nullptr;
	string mline;
	if (matFile) {
		while (getline(matFile, mline, '\n')) {
			stringstream   linestream(mline);
			string token;
			linestream >> token;

			if (token == "newmtl") {
				string name;
				linestream >> name;
				currentTexture = new MeshTexture();
				currentTexture->name = name;
				currentTexture->hasTexture = false;
				materialTextures.push_back(currentTexture);
			}
			else if (token == "Kd") {
				glm::vec3 color;
				linestream >> color.x >> color.y >> color.z;
				currentTexture->diffuse = color;
			}
			else if (token == "map_Kd") {
				string name;
				linestream >> name;

				if ((name.find("\\") == std::string::npos) && (name.find(".") != std::string::npos)) {
					string mapDir = string(path);
					string mapPath = mapDir.substr(0, mapDir.find_last_of("\\")) + "\\" + name;

					currentTexture->path = mapPath;
					currentTexture->hasTexture = true;
				}

			}
		}
	}
	matFile.close();

	Mesh *meshObj = new Mesh(ZERO_VECTOR, meshObjects, materialTextures, "Mesh_" + to_string(++objectCount["Mesh"]), ofColor::seaGreen);
	AddObject(meshObj);
}