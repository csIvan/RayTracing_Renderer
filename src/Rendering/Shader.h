#pragma once

#include "ofMain.h"
#include "Renderer.h"
#include "../Scene/Ray.h"
#include "../Scene/SceneObject.h"
#include "../Scene/SceneObjects/Torus.h"
#include "../Scene/SceneObjects/Sphere.h"
#include "../Scene/SceneObjects/Plane.h"
#include "../Scene/SceneObjects/Cone.h"
#include "../Scene/SceneObjects/Cylinder.h"
#include "../Scene/SceneObjects/LSystem.h"
#include "../Scene/SceneObjects/Mesh.h"
#include "../Scene/SceneObjects/Lights/Light.h"
#include "../Scene/SceneObjects/Lights/SpotLight.h"
#include "../Scene/SceneObjects/Lights/AreaLight.h"

class Shader {
public:
	Renderer *renderer;
	vector<Light *> lights;
	vector<SceneObject *> objects;
	glm::vec3 opoint;
	glm::vec3 onormal;

	Shader(Renderer *renderer, vector<Light *> lights, vector<SceneObject *> objects);
	Shader() {};
	~Shader() {};

	ofColor getColor(Ray &ray, const glm::vec3 &p, glm::vec3 &norm, SceneObject* obj, int depth);
	
	ofColor lambert(Ray &ray, const glm::vec3 &p, const glm::vec3 &norm, const ofColor diffuse, float reflect, int depth);
	ofColor phong(const glm::vec3 &p, const glm::vec3 &norm, const glm::vec3 camPos, const ofColor diffuse, const ofColor specular, float power);
	bool inShadow(const Ray &r, glm::vec3 hitPoint, float lightDistance);

	Ray reflect(glm::vec3 point, glm::vec3 viewRay, glm::vec3 normal);
};