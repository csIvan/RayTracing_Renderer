#pragma once

#include "ofMain.h"
#include "../Rendering/Material.h"
#include "../Rendering/Texture.h"
#include "../Rendering/Ray.h"

// Base class for any renderable object in the scene
class SceneObject {
public:
	Texture objTexture;
	Material objMaterial;
	ofMaterial sceneMaterial;
	ofTexture sceneTexture;

	string objName = "SceneObject_";
	bool isSelected = false;

	glm::mat4 Transform = glm::mat4(1.0);
	glm::vec3 position = glm::vec3(0, 0, 0);
	glm::vec3 rotation = glm::vec3(0, 0, 0);
	glm::vec3 scale = glm::vec3(1, 1, 1);


	virtual ~SceneObject() {};
	virtual void draw() = 0;    // pure virtual funcs - must be overloaded
	virtual bool intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal, glm::vec2 &uv) { return false; }
	virtual float sdf(glm::vec3 p) { return 0.0f; }
	virtual glm::vec2 getUV(glm::vec3 p) { return glm::vec2(0, 0); };

	// Transformations
	glm::mat4 getTranslateMatrix() {
		return (glm::translate(glm::mat4(1.0), glm::vec3(position.x, position.y, position.z)));
	}
	glm::mat4 getRotateMatrix() {
		return (glm::eulerAngleYXZ(glm::radians(rotation.y), glm::radians(rotation.x), glm::radians(rotation.z)));
	}
	glm::mat4 getScaleMatrix() {
		return (glm::scale(glm::mat4(1.0), glm::vec3(scale.x, scale.y, scale.z)));
	}
	void applyMatrix() {
		Transform = (getTranslateMatrix() * getRotateMatrix() * getScaleMatrix());
	}
};