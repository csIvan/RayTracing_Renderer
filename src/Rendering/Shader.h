#pragma once

#include "ofMain.h"
#include "Renderer.h"
#include "Ray.h"
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
	glm::vec3 opoint2;
	glm::vec3 onormal;

	Shader(Renderer *renderer, vector<Light *> lights, vector<SceneObject *> objects);
	Shader() {};
	~Shader() {};

	ofColor getColor(Ray &ray, const glm::vec3 &p, const glm::vec3 &norm, SceneObject* obj, int depth);
	
	ofColor lambert(Ray &ray, const glm::vec3 &p, const glm::vec3 &norm, SceneObject* obj, int depth);
	ofColor lambert(Ray &ray, const glm::vec3 &p, const glm::vec3 &norm, SceneObject* obj, float reflect, int depth);
	ofColor phong(Ray &ray, const glm::vec3 &p, const glm::vec3 &norm, const glm::vec3 camPos, SceneObject* obj, int depth);
	bool inShadow(const Ray &r, glm::vec3 hitPoint, float lightDistance, bool ref);

	Ray reflect(glm::vec3 point, glm::vec3 viewRay, glm::vec3 normal, bool outside);
	Ray refract(glm::vec3 point, glm::vec3 viewRay, glm::vec3 normal, float ior, bool outside);

	void fresnel(glm::vec3 point, glm::vec3 viewRay, glm::vec3 normal, float ior, float &kr);
};