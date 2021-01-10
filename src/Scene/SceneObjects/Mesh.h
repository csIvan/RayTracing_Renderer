#pragma once

#include "ofMain.h"
#include "../SceneObject.h"

class Triangle {
public:
	int i, j, k; //indices for triangles
	int in, jn, kn; //indices for normals;
};

class Mesh : public SceneObject {
public:
	ofMaterial material;
	vector<glm::vec3> vertices;
	vector<Triangle> tris;
	vector<glm::vec3> vertNormals;

	bool intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal);
	//return glm::intersectRayTriangle(ray.p, ray.d, vertices[triangle.i], vertices[triangle.j], vertices[triangle.k], bary);

	void draw();

};