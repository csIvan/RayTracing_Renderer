#pragma once

#include "../Application/Definitions.h"

#include "ofMain.h"
#include "Shader.h"
#include "Renderer.h"
#include "Ray.h"
#include "RenderCam.h"

class RayMarcher : public Renderer{
public:
	Shader shader;
	int imageWidth, imageHeight;
	int indexHit;

	RayMarcher(int imageWidth, int imageHeight, ofImage image, RenderCam &cam);
	RayMarcher() {};
	~RayMarcher() {};

	ofImage render(int samples);
	bool castRay(Ray &r, ofColor &color, glm::vec3 &p, glm::vec3 &n, int depth = 0);

	bool rayMarch(Ray r, glm::vec3 &p);
	float sceneSDF(glm::vec3 p);
	glm::vec3 getNormal(const glm::vec3 &p, int i); 
	glm::vec3 getNormalRM(const glm::vec3 &p);
	float opRep(const glm::vec3 p, const glm::vec3 c, SceneObject &s);
	float opRound(const glm::vec3 p, SceneObject &s, float rad);
};