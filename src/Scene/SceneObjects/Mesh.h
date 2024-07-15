#pragma once

#include "../SceneObject.h"


// Simple geometric triangle struct
struct Triangle {
	int i, j, k;	// indices for triangles
	int in, jn, kn; // indices for normals
	int it, jt, kt; // indices for texture coordinates

	Triangle() {};
	~Triangle() {};
};

// Simple struct for containing texture data for a given mesh object
struct MeshTexture {
	ofImage texture;
	string path;
	string name;
	glm::vec3 diffuse;
	bool hasTexture;

	MeshTexture() {};
	~MeshTexture() {};
};

// 3D representation of a scene mesh object
class MeshObject {
public:
	MeshTexture meshTex;
	vector<Triangle> tris;
	vector<glm::vec3> vertices;
	vector<glm::vec3> vertNormals;
	vector<glm::vec2> vertTextures;
	string mtlName;

	MeshObject() {};
	~MeshObject() {};

	void processData(int vertOffset, int normOffset, int texOffset, const vector<int> &vertIndices, const vector<int> &normIndices, const vector<int> &texIndices);
	void setMtlName(const string &name) { mtlName = name; }
};



// 3D Mesh class that maintains all mesh objects in the scene
class Mesh : public SceneObject {
private:
	vector<ofMesh *> ofMeshes;
	vector<MeshObject *> meshObjects;

public:
	Mesh(const glm::vec3 &position, const vector<MeshObject *> &objs, const vector<MeshTexture *> &textures, const string &name, const ofColor &diffuse = DEFAULT_COLOR);
	~Mesh() {};


	// Override functions from SceneObject
	void setBounds();
	void draw();
	bool intersect(const Ray &ray, HitInfo &hitInfo);
	float sdf(const glm::vec3 &point);
	glm::vec2 getMeshUV(const glm::vec3 &point, const glm::vec2 &t1, const glm::vec2 &t2, const glm::vec2 &t3, const glm::vec2 &bary) const;


	// Collects all mesh objects with their corresponding textures
	void createMesh(const vector<MeshTexture *> &textures);


};