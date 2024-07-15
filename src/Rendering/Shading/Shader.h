#pragma once

#include "ofMain.h"
#include "Material.h"
#include "../Ray.h"
#include "../../Scene/SceneObject.h"
#include "../../Scene/SceneObjects/Sphere.h"
#include "../../Scene/SceneObjects/Lights/Light.h"

class Scene; // Forward declaration

// Shader class used for calculating surface color and shading scene objects
class Shader {
public:
	Shader() {};
	~Shader() {};


	// Return the calculated shading color
	ofColor getColor(const Ray &ray, const HitInfo &hitInfo, const Material &mat, const Scene &scene, int depth);


	// Calculates Lambertian shading color of a given point on the surface of a scene object
	ofColor lambert(const HitInfo &hitInfo, const Material &mat, const Scene &scene);


	// Calculates the phong shading, which includes highlights and reflective properties
	ofColor phong(const Ray &ray, const HitInfo &hitInfo, const Material &mat, const Scene &scene, int depth);


	// Check if given point on surface of the object is in shadow
	bool inShadow(const Ray &ray, const glm::vec3 &hitPoint, const Scene &scene, float lightDistance);


	// Check if ray from point on surface of the object hits another scene object
	bool castSurfaceRay(const Ray &ray, HitInfo &hitInfo, const Scene &scene, int depth);


	// Helper functions for reflection and refraction
	ofColor getReflectColor(const Ray &ray, const HitInfo &hitInfo, const Scene &scene, bool outside, int depth);
	ofColor getRefractColor(const Ray &ray, const HitInfo &hitInfo, const Scene &scene, float refractCoeff, bool outside, int depth);
	Ray getReflectRay(const Ray &ray, const HitInfo &hitInfo, bool outside);
	Ray getRefractRay(const Ray &ray, const HitInfo &hitInfo, float ior, bool outside);
	float getFresnelRatio(const Ray &ray, const HitInfo &hitInfo, float ior);

};