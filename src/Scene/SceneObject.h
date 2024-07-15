#pragma once

#include "ofMain.h"
#include <glm/gtx/intersect.hpp>
#include <glm/gtx/euler_angles.hpp>
#include "../Application/Definitions.h"
//#include <memory>
#include "AABB.h"
#include "../Rendering/Shading/Material.h"
#include "../Rendering/Shading/Texture.h"
#include "../Rendering/Ray.h"


// Base SceneObject Class
class SceneObject {
protected:
	// Default object properties
	string objName = "SceneObject_";
	glm::mat4 Transform = glm::mat4(1.0);
	glm::mat4 Rotate = glm::mat4(1.0);
	glm::vec3 position = ZERO_VECTOR;
	glm::vec3 rotation = ZERO_VECTOR;
	glm::vec3 scale = glm::vec3(1, 1, 1);
	bool isSelected = false;

	// Collision and visual properties
	AABB box;
	ofMaterial sceneMaterial;
	ofTexture sceneTexture;
	Material objMaterial;
	Texture objTexture;


public:
	virtual ~SceneObject() {};

	// Sets the bounding box for the object
	virtual void setBounds() { box = AABB(glm::vec3(-1, -1, 1), glm::vec3(1, 1, -1)); };


	// Pure virtual function to draw the object
	virtual void draw() = 0;


	// Checks for ray-object intersection
	virtual bool intersect(const Ray &ray, HitInfo &hitInfo) { return false; };


	// Calculates the signed distance function
	virtual float sdf(const glm::vec3 &point) { return 0.0f; }


	// Calculates the UV coordinates for texture mapping
	virtual glm::vec2 getUV(const glm::vec3 &point) const { return glm::vec2(0, 0); }


	// Matrix Transformations
	glm::mat4 getTranslateMatrix() const {
		return (glm::translate(glm::mat4(1.0), glm::vec3(position.x, position.y, position.z)));
	}
	glm::mat4 getScaleMatrix() const {
		return (glm::scale(glm::mat4(1.0), glm::vec3(scale.x, scale.y, scale.z)));
	}
	glm::mat4 getRotateMatrix() const {
		return (glm::eulerAngleYXZ(glm::radians(rotation.y), glm::radians(rotation.x), glm::radians(rotation.z)));
	}
	glm::mat4 getTransform() const {
		return Transform;
	}
	void applyMatrix() {
		Transform = (getTranslateMatrix() * getRotateMatrix() * getScaleMatrix());
	}


	// Setters and Getters
	void setName(string &newName) { objName = newName; }
	void setPosition(const glm::vec3 &position) { this->position = position; }
	void setRotation(const glm::vec3 &rotation) { this->rotation = rotation; }
	void setScale(const glm::vec3 &scale) { this->scale = scale; }
	void setSelected(bool selected) { isSelected = selected; }
	void setColor(ofColor color) { objMaterial.setDiffuse(color); }
	void setMaterialType(const string &materialType) { objMaterial.SetMat(materialType); }
	void setTileFactor(int tileFactor) { objTexture.setUVTile(tileFactor); }
	void setTextureImage(const string &path) { objTexture.addTexture(path); }
	void removeTextureImage() { objTexture.removeTexture(); }
	string getName() const { return objName; }
	glm::vec3 getPosition() const { return position; }
	glm::vec3 getRotation() const { return rotation; }
	glm::vec3 getScale() const { return scale; }
	Material getMaterial() const { return objMaterial; }
	Texture getTexture() const { return objTexture; }
	AABB getBoundingBox() const { return box; }

};