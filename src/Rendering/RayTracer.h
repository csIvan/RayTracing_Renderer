#pragma once

#include "ofMain.h"
#include "Renderer.h"
#include "Shader.h"
#include "Ray.h"
#include "RenderCam.h"
#include "../Scene/SceneObjects/Sphere.h"
#include "../Scene/SceneObjects/Cube.h"
#include "../Scene/SceneObjects/Torus.h"
#include "../Scene/SceneObjects/Plane.h"

class RayTracer : public Renderer {
public:
	ofImage texture, sphereTexture;
	Shader shader;
	int imageWidth, imageHeight;
	float Power;

	RayTracer(int imageWidth, int imageHeight, ofImage &image, RenderCam &cam);
	RayTracer() {};
 	~RayTracer() {};

	ofImage render(int samples);
	bool castRay(Ray &ray, ofColor &color, int depth = 0);

};
