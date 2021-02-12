#pragma once

#include "ofMain.h"
#include "Renderer.h"
#include "Shader.h"
#include "../Scene/Ray.h"
#include "../Scene/RenderCam.h"
#include "../Scene/SceneObject.h"
#include "../Scene/SceneObjects/Lights/Light.h"
#include "../Scene/SceneObjects/Sphere.h"
#include "../Scene/SceneObjects/Cube.h"
#include "../Scene/SceneObjects/Torus.h"
#include "../Scene/SceneObjects/Plane.h"

class RayTracer : public Renderer {
public:
	RenderCam renderCam;
	ofImage texture, sphereTexture;
	Shader shader;
	int imageWidth, imageHeight;
	float Power;

	RayTracer(int imageWidth, int imageHeight, ofImage &image);
	RayTracer() {};
	ofImage render();
	bool castRay(Ray &ray, ofColor &color, int depth = 0);

};
