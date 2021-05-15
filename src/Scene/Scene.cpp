#include "Scene.h"

void Scene::setup() {
	renderCam.objName = "Render_Cam";
	image.allocate(imageWidth, imageHeight, ofImageType::OF_IMAGE_COLOR);

	rayTracer = RayTracer(imageWidth, imageHeight, image, renderCam);
	rayMarcher = RayMarcher(imageWidth, imageHeight, image, renderCam);
	nearestDistance = FLT_MAX;
	addSphere();
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
		else if (strcmp(line, "vn") == 0) {
			glm::vec3 vertNormal;
			fscanf_s(file, "%f %f %f\n", &vertNormal.x, &vertNormal.y, &vertNormal.z);
			tempVertNormals.push_back(vertNormal);
		}
		else if (strcmp(line, "vt") == 0) {
			glm::vec2 vertTex;
			fscanf_s(file, "%f %f %f\n", &vertTex.x, &vertTex.y);
			tempVertTextures.push_back(vertTex);
		}
		else if (strcmp(line, "f") == 0) {
			unsigned int vertexIndex[3];
			unsigned int vertNormIndex[3];
			unsigned int vertTexIndex[3];

			//Only taking the vertex indices so we ignore the other numbers
			fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &vertTexIndex[0], &vertNormIndex[0],
				&vertexIndex[1], &vertTexIndex[1], &vertNormIndex[1], &vertexIndex[2], &vertTexIndex[2], &vertNormIndex[2]);
			tempIndices.push_back(vertexIndex[0]);
			tempIndices.push_back(vertexIndex[1]);
			tempIndices.push_back(vertexIndex[2]);
			tempVertTexIndices.push_back(vertTexIndex[0]);
			tempVertTexIndices.push_back(vertTexIndex[1]);
			tempVertTexIndices.push_back(vertTexIndex[2]);
			tempVertNormIndices.push_back(vertNormIndex[0]);
			tempVertNormIndices.push_back(vertNormIndex[1]);
			tempVertNormIndices.push_back(vertNormIndex[2]);
		}
	}

	vector<glm::vec3> meshVertices;
	vector<glm::vec3> meshVerticesNormals;
	vector<glm::vec2> meshVerticesTex;
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
	for (unsigned int i = 0; i < tempVertTexIndices.size(); i++) {
		unsigned int vertTexIndex = tempVertTexIndices[i];
		glm::vec2 vn = tempVertTextures[vertTexIndex - 1];
		meshVerticesTex.push_back(vn);
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
			triangle.it = temp[0];
			triangle.j = temp[1];
			triangle.jn = temp[1];
			triangle.jt = temp[1];
			triangle.k = temp[2];
			triangle.kn = temp[2];
			triangle.kt = temp[2];
			meshTris.push_back(triangle);
			temp.clear();
			count = 0;
		}
	}
	Mesh *meshObj = new Mesh(glm::vec3(0, 0, 0), meshTris, meshVertices, meshVerticesNormals, meshVerticesTex, "Mesh_" + to_string(++meshCount), ofColor::seaGreen);

	objects.push_back(meshObj);
	rayTracer.addObject(*meshObj);
	rayMarcher.addObject(*meshObj);

	// Display diagnostic information
	cout << "Number of Vertices: " << meshVertices.size() << endl;
	cout << "Number of Faces: " << meshTris.size() << endl;
	cout << "Number of Vertex Normals: " << meshVerticesNormals.size() << endl;
}