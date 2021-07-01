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
	Shader shader;
	int imageWidth, imageHeight;

	RayTracer(int imageWidth, int imageHeight, ofImage &image, RenderCam &cam);
	RayTracer() {};
 	~RayTracer() {};

	ofImage render(int samples);
	void mtRender(glm::vec2 start, glm::vec2 dim, int samples, float &percent);
	bool castRay(Ray &ray, ofColor &color, glm::vec3 &p, glm::vec3 &n, int depth = 0);
	bool castNewRay(Ray &ray, ofColor &color, glm::vec3 &p, glm::vec3 &n, int depth = 0);
	void setBVH(BVH *h) { bvh = h; }
	void setShader() { shader = Shader(this, lights, objects); };
	ofImage getImage() { return image; }

};
