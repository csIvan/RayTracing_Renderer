#pragma once

#include "ofMain.h"
#include "../Scene/SceneObject.h"
#include "../Scene/SceneObjects/Lights/Light.h"

class Renderer {
public:
	vector<SceneObject*> objects;
	vector<Light*> lights;
	ofImage image;

	virtual ofImage render() = 0;
	virtual bool castRay(Ray &r, ofColor &color, int depth = 0) {
		return false;
	}
	virtual ~Renderer() {};


	void addObject(SceneObject &object) { objects.push_back(&object); }
	void addLight(Light &light) { lights.push_back(&light); }

	void removeObject(string name) {
		for (int i = 0; i < objects.size(); i++) {
			if (objects[i]->objName == name)
				objects.erase(std::remove(objects.begin(), objects.end(), objects[i]), objects.end());
		}
	}

	void removeLight(string name) {
		for (int i = 0; i < lights.size(); i++) {
			if (lights[i]->objName == name) 
				lights.erase(std::remove(lights.begin(), lights.end(), lights[i]), lights.end());
		}
	}
};