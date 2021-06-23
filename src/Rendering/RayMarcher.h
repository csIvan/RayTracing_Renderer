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
	void mtRender(glm::vec2 start, glm::vec2 dim, int samples, float &percent);
	bool castRay(Ray &r, ofColor &color, glm::vec3 &p, glm::vec3 &n, int depth = 0);

	bool rayMarch(Ray r, glm::vec3 &p, int &index);
	float sceneSDF(glm::vec3 p, int &index);
	glm::vec3 getNormalRM(const glm::vec3 &p);
	float opRep(const glm::vec3 p, const glm::vec3 c, SceneObject &s);
	float opRound(const glm::vec3 p, SceneObject &s, float rad);

	void setShader() { shader = Shader(this, lights, objects); };
	ofImage getImage() { return image; }
};