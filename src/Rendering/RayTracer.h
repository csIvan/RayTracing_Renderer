#pragma once

#include "ofMain.h"
#include "Shader.h"
#include "../Scene/Ray.h"
#include "../Scene/RenderCam.h"
#include "../Scene/SceneObject.h"
#include "../Scene/SceneObjects/Light.h"
#include "../Scene/SceneObjects/Sphere.h"
#include "../Scene/SceneObjects/Cube.h"
#include "../Scene/SceneObjects/Torus.h"
#include "../Scene/SceneObjects/Plane.h"

class RayTracer {
public:
	vector<SceneObject*> objects;
	vector<Light*> lights;
	RenderCam renderCam;
	ofImage image, texture, sphereTexture;
	Shader shader;
	int imageWidth, imageHeight;
	float nearestDist = FLT_MAX;
	float Power;

	RayTracer(int imageWidth, int imageHeight, ofImage &image);
	RayTracer() {};
	ofImage render();

	void addObject(SceneObject &object);
	void addLight(Light &light);
	void remove(string name);

};
