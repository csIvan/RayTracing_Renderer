#pragma once

#include "ofMain.h"
#include "Renderer.h"

// RayTracing Renderer class
class RayTracer : public Renderer {
public:
	RayTracer() {};
	~RayTracer() {};

	// Override function from Renderer
	bool castRay(const Ray &ray, ofColor &color, const Scene &scene, int depth = 0);
	
};