#pragma once

#include "ofMain.h"
#include "../SceneObject.h"

struct MeshTextureMap {
	string path;
	string name;
	glm::vec3 kd;
	bool hasTexture;
};

class Triangle {
public:
	Triangle() {};
	~Triangle() {};
	int i, j, k; //indices for triangles
	int in, jn, kn; //indices for normals;
	int it, jt, kt; //indices for texture coordinates;
};

class MeshObject {
public:
	MeshObject() {};
	~MeshObject() {};

	void processData(int vstart, int nstart, int tstart, vector<int> verti, vector<int> normi, vector<int> texi);
	void setMtlName(string n) { mtlName = n; };

	MeshTextureMap *meshTex;
	vector<Triangle> tris;
	vector<glm::vec3> vertices;
	vector<glm::vec3> vertNormals;
	vector<glm::vec2> vertTextures;
	string mtlName;
};

class Mesh : public SceneObject {
public:
	vector<ofMesh *> meshes;
	vector<MeshObject *> mObjects;
	vector<MeshTextureMap> textureMaps;
	vector<Triangle> tris;
	vector<glm::vec3> vertices;
	vector<glm::vec3> vertNormals;
	vector<glm::vec2> vertTextures;
	vector<glm::vec3> points;
	vector<glm::vec3> normals;
	Triangle *selectedTri;
	MeshObject *objSel;
	glm::vec2 barySelected;

	Mesh(glm::vec3 p, vector<MeshObject *> objs, vector<MeshTextureMap *> maps, string name, ofColor diffuse = ofColor::lightGray);
	Mesh() {};
	~Mesh() {};

	void setBounds();
	int getMeshPointsInBox(const vector<int> &points, Box &box, vector<int> &pointsRtn);

	bool intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal, glm::vec2 &uv);
	void draw();
	float sdf(const glm::vec3 p);
	glm::vec2 getUV(glm::vec3 p);
	glm::vec2 getMeshUV(glm::vec3 p, MeshObject *o, glm::vec3 texCoors, glm::vec2 bary);
};