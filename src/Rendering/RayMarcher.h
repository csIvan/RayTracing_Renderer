#pragma once

#include "ofMain.h"
#include "Shader.h"
#include "../Scene/Ray.h"
#include "../Scene/SceneObject.h"

class RayMarcher {
public:
	Shader shader;

	RayMarcher();
	void render();
	bool rayMarch(Ray r, glm::vec3 &p);

	float sceneSDF(glm::vec3 p);
	glm::vec3 getNormal(const glm::vec3 &p, int i); 
	glm::vec3 getNormalRM(const glm::vec3 &p);
	float opRep(const glm::vec3 p, const glm::vec3 c, SceneObject &s);
	float opRound(const glm::vec3 p, SceneObject &s, float rad);
};