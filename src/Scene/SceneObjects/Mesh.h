#pragma once

#include "ofMain.h"
#include "../SceneObject.h"

class Triangle {
public:
	Triangle() {};
	~Triangle() {};
	int i, j, k; //indices for triangles
	int in, jn, kn; //indices for normals;
	int it, jt, kt; //indices for texture coordinates;
};

class Mesh : public SceneObject {
public:
	ofMesh mesh;
	vector<Triangle> tris;
	vector<glm::vec3> vertices;
	vector<glm::vec3> vertNormals;
	vector<glm::vec2> vertTextures;
	vector<glm::vec3> points;
	vector<glm::vec3> normals;
	Triangle *selectedTri;

	Mesh(glm::vec3 p, vector<Triangle> t, vector<glm::vec3> v, vector<glm::vec3> vn, vector<glm::vec2> vt, string name, ofColor diffuse = ofColor::lightGray);
	Mesh() {};
	~Mesh() {};
	bool intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal, glm::vec2 &uv);

	void draw();
	float sdf(const glm::vec3 p);
	glm::vec2 getUV(glm::vec3 p);
};