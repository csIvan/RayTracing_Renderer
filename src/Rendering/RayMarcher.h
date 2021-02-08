#pragma once

#include "ofMain.h"
#include "Shader.h"
#include "../Definitions.h"
#include "../Scene/Ray.h"
#include "../Scene/RenderCam.h"
#include "../Scene/SceneObject.h"
#include "../Scene/SceneObjects/Lights/Light.h"
#include "../Scene/SceneObjects/Cone.h"

class RayMarcher {
public:
	vector<SceneObject*> objects;
	vector<Light*> lights;
	RenderCam renderCam;
	ofImage image;
	Shader shader;
	int imageWidth, imageHeight;
	float nearestDist = FLT_MAX;
	int indexHit;

	RayMarcher(int imageWidth, int imageHeight, ofImage image);
	RayMarcher() {};
	ofImage render();
	bool rayMarch(Ray r, glm::vec3 &p);

	float sceneSDF(glm::vec3 p);
	glm::vec3 getNormal(const glm::vec3 &p, int i); 
	glm::vec3 getNormalRM(const glm::vec3 &p);
	float opRep(const glm::vec3 p, const glm::vec3 c, SceneObject &s);
	float opRound(const glm::vec3 p, SceneObject &s, float rad);

	void addObject(SceneObject &object);
	void addLight(Light &light);
	void removeObject(string name);
	void removeLight(string name);
};