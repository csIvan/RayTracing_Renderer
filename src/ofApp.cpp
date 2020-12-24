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
	sp1 = Spotlight(glm::vec3(.5, -1, -.55), .1, 4);
	sp1.position = glm::vec3(-3, 4, 1);

	//point light 
	light1 = Light(glm::vec3(0, 5, 8), 70);
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


	nearestDistance = FLT_MAX;
	//sphere1 = Sphere(glm::vec3(1.5, -1, -1), 1, ofColor::mediumPurple);
	sphere1 = Sphere(glm::vec3(0, 1, 0), .25, ofColor::mediumPurple);
	cube1 = Cube(glm::vec3(0, 0, 0), 3, ofColor::seaGreen);
	f1 = LSystem(glm::vec3(0, -2.5, 0), 1, ofColor::seaGreen);
	wp1 = WaterPool(glm::vec3(1.5, -3, -1), 1, ofColor::mediumPurple);
	plane1 = Plane(glm::vec3(0, -3.25, 0), glm::vec3(0, 1, 0), ofColor::lightBlue);
	torus1 = Torus(glm::vec3(-1.3, -1.2, 0), 1, 0.5, ofColor::seaGreen);
	torus2 = Torus(glm::vec3(2.2, -0.4, -2), 2, 0.2, 65.0f, glm::vec3(1, 0, 1), ofColor::orangeRed);
	//scene.push_back(&sphere1);
	//scene.push_back(&cube1);
	scene.push_back(&f1);
	//scene.push_back(&wp1);
	//scene.push_back(&plane1);
	//scene.push_back(&torus1);
	//scene.push_back(&torus2);
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


ofColor ofApp::lambert(const glm::vec3 &point, const glm::vec3 &normal, const ofColor diffuse) {
	float ambientCo = 0.08;
	ofColor lambertColor = diffuse * ambientCo;
	for (int i = 0; i < lights.size(); i++) {
		glm::vec3 l = glm::normalize(lights[i]->position - point);
		glm::vec3 lightToPixel = glm::normalize(point - lights[i]->position);
		//float SpotFactor = glm::dot(lightToPixel, lights[i]->direction);
		float distance = glm::distance(lights[i]->position, point);
		Ray shadRay = Ray(glm::vec3(point.x, point.y, point.z + .001f), l);

		//call to calculate the falloff factor for the spotlight
		//float falloff = lights[i]->falloff(SpotFactor);

		ofColor lambertCalculation = diffuse * (lights[i]->intensity / glm::pow(distance, 2)) * glm::max(0.0f, glm::dot(normal, l));

		if (!inShadow(shadRay)) {
			lambertColor += lambertCalculation;
		}

	}
	return lambertColor;

}

ofColor ofApp::phong(const glm::vec3 &point, const glm::vec3 &normal, const ofColor diffuse, const ofColor specular, float power) {
	float ambientCo = 0.05;
	ofColor phongColor = diffuse * ambientCo;
	for (int i = 0; i < lights.size(); i++) {
		glm::vec3 l = glm::normalize(lights[i]->position - point);
		glm::vec3 lightToPixel = glm::normalize(point - lights[i]->position);
		//float SpotFactor = glm::dot(lightToPixel, lights[i]->direction);
		float distance = glm::distance(lights[i]->position, point);
		glm::vec3 v = -glm::normalize(point - renderCam.position);
		glm::vec3 h = glm::normalize(v + l);
		Ray shadRay = Ray(glm::vec3(point.x, point.y, point.z + 0.001f), l);;
		//if (normal == glm::vec3(0, 1, 0))
			//shadRay = Ray(glm::vec3(point.x, point.y + .01f, point.z), l);


		//call to calculate the falloff factor for the spotlight
		//float falloff = lights[i]->falloff(SpotFactor);

		ofColor phongCalculation = diffuse * (lights[i]->intensity / glm::pow(distance, 2)) * glm::max(0.0f, glm::dot(normal, l)) +
			specular * (lights[i]->intensity / glm::pow(distance, 2)) * (glm::pow(glm::max(0.0f, glm::dot(normal, h)), power));

		//if (!inShadow(shadRay)) {
		phongColor += phongCalculation;

		//}
	}
	return phongColor;
}

// Calculate the light depending on the constraint angle and falloff angle
float Spotlight::falloff(float spotAngle) {
	spotAngle = 1 - spotAngle;
	if (spotAngle < lightAngle) {
		return 1;
	}
	if (spotAngle > falloffAngle) {
		return 0;
	}

	float delta = (spotAngle - falloffAngle) / (lightAngle - falloffAngle);
	return delta;
}

bool ofApp::inShadow(const Ray &r) {
	bool blocked = false;
	for (int index = 0; index < scene.size(); index++) {
		glm::vec3 point;
		glm::vec3 normal;

		//calculate shadows using rayMarch()
		if (rayMarch(r, point)) {
			blocked = true;
		}
	}
	return blocked;
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
	case 'r': triRayTrace();
		break;
	case 'm': RayMarching();
		cout << "done" << endl;
		break;
	default:
		break;
	}
}

/**
* To call the raytracing function, press the 'r' key
* Raytrace function. It is similar to the ray tracing function from the first project
* but it also allows for triangle-ray intersection. Also, it checks if the object is
* a sphere or a plane in order to apply the correct texture. It also calls for lambert
* shading because I thought that my scene looked better in lambert, but phong can also be used.
*/
void ofApp::triRayTrace() {
	for (float i = 0; i < imageHeight; i++) {
		for (float j = 0; j < imageWidth; j++) {
			Ray ray = renderCam.getRay((j) / imageWidth, (i) / imageHeight);

			bool hit = false;
			float dist;
			ofColor color;

			for (int index = 0; index < scene.size(); index++) {

				glm::vec3 point;
				glm::vec3 normal;
				if (scene[index]->intersect(ray, point, normal)) {
					dist = glm::distance(renderCam.position, point);
					if (dist < nearestDistance) {
						nearestDistance = dist;
						hit = true;
						normal = glm::normalize(normal);
						//if the object is a sphere
						if (typeid(*scene[index]) == typeid(Sphere)) {
							Sphere *globe = (Sphere*)scene[index];

							glm::vec3 n = glm::normalize(point - globe->position);
							float u = atan2(n.x, n.z) / (2 * PI) + 0.5;
							float v = n.y * 0.5 + 0.5;


							float uLookUp = u * sphereTexture.getWidth() - .5;
							float pixelj = fmod(uLookUp, sphereTexture.getWidth());
							float vLookUp = v * sphereTexture.getHeight() - .5;
							float pixeli = fmod(vLookUp, sphereTexture.getHeight());

							ofColor globeColor = sphereTexture.getColor(pixelj, sphereTexture.getHeight() - pixeli - 1);
							color = lambert(point, normal, globeColor);
						}
						else if (typeid(*scene[index]) == typeid(Plane)) {
							//10x10 tiles
							int uvTileFactor = 10;

							Plane *p1 = (Plane*)scene[index];

							float u = (p1->width + point.x) / (p1->width / uvTileFactor);
							float v = (p1->height + point.z) / (p1->height / uvTileFactor);

							//find the color at the point of u and v
							float uLookUp = u * texture.getWidth() - .5;
							float pixelj = fmod(uLookUp, texture.getWidth());
							float vLookUp = v * texture.getHeight() - .5;
							float pixeli = fmod(vLookUp, texture.getHeight());

							//apply the color of the texture using the shading algorithm
							ofColor planeColor = texture.getColor(pixelj, texture.getHeight() - pixeli - 1);

							//color = phong(point, normal, planeColor, scene[index]->specularColor, Power);
							color = lambert(point, normal, planeColor);
						}
						else {
							color = phong(point, normal, scene[index]->diffuseColor, scene[index]->specularColor, Power);
							//color = lambert(point, normal, scene[index]->diffuseColor);
						}
					}
				}
			}
			nearestDistance = FLT_MAX;

			if (hit) {
				image.setColor(j, imageHeight - i - 1, color);
			}
			else
				image.setColor(j, imageHeight - i - 1, ofColor::black);
		}
	}
	image.save("images/RayIntersectRender.jpg");
}



/******************************************** Ray Marching Functions ********************************************************/
bool ofApp::rayMarch(Ray r, glm::vec3 &p) {
	bool hit = false;
	p = r.p;
	for (int i = 0; i < MAX_RAY_STEPS; i++) {
		float dist = sceneSDF(p);
		if (dist < DIST_THRESHOLD) {
			hit = true;
			break;
		}
		else if (dist > MAX_THRESHOLD) {
			break;
		}
		else {
			p = p + r.d*dist;
		}
	}
	return hit;
}

/*
* RayMarching function. Calls rayMarch instead of the object's ray intersect function.
* Also uses the normalRM so that phong shading can be applied to the scene
*/
void ofApp::RayMarching() {
	for (float i = 0; i < imageHeight; i++) {
		for (float j = 0; j < imageWidth; j++) {
			Ray ray = renderCam.getRay((j) / imageWidth, (i) / imageHeight);

			ofColor color;
			glm::vec3 p;
			bool hit = rayMarch(ray, p);
			glm::vec3 norm = getNormalRM(p);

			color = phong(p, norm, scene[indexHit]->diffuseColor, ofColor::lightGray, 50);
			//color = lambert(p, norm, scene[indexHit]->diffuseColor);

			if (hit) {
				//cout << "*** Hit ******************************************************* " << p << endl << endl;
				image.setColor(j, imageHeight - i - 1, color);
			}
			else
				image.setColor(j, imageHeight - i - 1, ofColor::black);
		}
		cout << i << "-";
	}
	image.save("images/LSystem140.jpg");
}

//SceneSDF. Checks every primitive's sdf and determines the closest one to the point
//also marks the index of the object hit
float ofApp::sceneSDF(glm::vec3 p) {
	nearestDistance = FLT_MAX;
	float d = 0.0;
	for (int i = 0; i < scene.size(); i++) {
		//WaterPool *wp1 = (WaterPool*)scene[i];
		//d = wp1->sdf(p, 1, 8);
		//d = opRep(p, glm::vec3(20.0f, 20.0f, 20.0f), *scene[i]);
		d = scene[i]->sdf(p);
		if (d < nearestDistance) {
			nearestDistance = d;
			indexHit = i;
		}
	}
	return nearestDistance;
}

//Midterm - opRep Function
float ofApp::opRep(const glm::vec3 p, const glm::vec3 c, SceneObject &s) {
	//glm::vec3 q = glm::mod(p,c) - 10.0f * c;
	glm::vec3 q = glm::vec3(glm::mod(p.x, c.x) - 10.0f, glm::mod(p.y + 10.0f, c.y) - 10.0f, glm::mod(p.z + 10.0f, c.z) - 10.0f);

	//round the hard edges before returning
	return opRound(q, s, 0.3f);
}

//opRound Function - It rounds the edges based on a float variable
float ofApp::opRound(const glm::vec3 p, SceneObject &s, float rad) {
	return 2.0f;
	//return s.sdf(p) - rad;
}

//Ray Marching getNormal Function
glm::vec3 ofApp::getNormalRM(const glm::vec3 &p) {
	float eps = 0.001;
	float dp = sceneSDF(p);
	glm::vec3 n(dp - sceneSDF(glm::vec3(p.x - eps, p.y, p.z)),
		dp - sceneSDF(glm::vec3(p.x, p.y - eps, p.z)),
		dp - sceneSDF(glm::vec3(p.x, p.y, p.z - eps)));
	return glm::normalize(n);
}


//Regular hard-coded getNormal function
glm::vec3 ofApp::getNormal(const glm::vec3 &p, int i) {
	glm::vec3 pNormal;
	if ((typeid(*scene[i]) == typeid(Sphere)))
		pNormal = glm::normalize(p - scene[i]->position);
	else if ((typeid(*scene[i]) == typeid(Plane)))
		pNormal = glm::vec3(0, 1, 0);

	return pNormal;
}

glm::vec3 ViewPlane::toWorld(float u, float v) {
	float w = width();
	float h = height();
	return (glm::vec3((u * w) + min.x, (v * h) + min.y, position.z));
}

Ray RenderCam::getRay(float u, float v) {
	glm::vec3 pointOnPlane = view.toWorld(u, v);
	return(Ray(position, glm::normalize(pointOnPlane - position)));
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
