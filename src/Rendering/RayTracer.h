#pragma once

#include "ofMain.h"
#include "Shader.h"
#include "../Scene/Ray.h"

class RayTracer {
public:
	Shader shader;

	RayTracer();
	void render();

};
