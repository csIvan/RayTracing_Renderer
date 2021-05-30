#pragma once

#include "ofMain.h"
#include "../Scene/SceneObject.h"
#include "../Scene/Box.h"


class BVHNode {
public:
	BVHNode() {};
	~BVHNode() {};

	Box box;
	vector<SceneObject *> objects;
	BVHNode *left;
	BVHNode *right;
};

class BVH : public SceneObject {
public:

	BVH();
	~BVH() {};

	void splitBox(Box &abox, Box &bbox);
	void buildLeaf(BVHNode *prev_node);
	//Box combineBoxes(Box abox, Box bbox);
	glm::vec3 findCentroid(Box box);
	//void sortObjects(vector<SceneObject *> objs, int axis);

	void create(vector<SceneObject *> objs);
	void buildNode(vector<SceneObject *> objs, BVHNode *node);
	bool intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal, glm::vec2 &uv);
	void draw();
	void drawBVH(BVHNode *node);

	vector<SceneObject *> bvhObjs;
	vector<glm::vec3> points;
	BVHNode *root;
};