#pragma once

#include "ofMain.h"
#include "../Scene/SceneObject.h"
#include "../Scene/Box.h"
#include "../Scene/SceneObjects/Mesh.h"


class BVHNode {
public:
	BVHNode() {};
	~BVHNode() {};

	Box *box;
	vector<SceneObject *> objects;
	BVHNode *left;
	BVHNode *right;
};

class BVH : public SceneObject {
public:

	BVH();
	~BVH() {};

	Box* combineBoxes(Box *abox, Box *bbox);
	glm::vec3 findCentroid(Box *box);
	int sortObjects(vector<SceneObject *> &objs, int axis);

	void create(vector<SceneObject *> objs);
	void buildNode(vector<SceneObject *> objs, BVHNode *node);
	bool intersect(const Ray &ray, BVHNode *node, vector<SceneObject *> &objs, bool inside);
	void draw();
	void drawBVH(BVHNode *node);

	vector<SceneObject *> bvhObjs;
	vector<BVHNode *> nodes;
	vector<glm::vec3> points;
	BVHNode *root;
	int levels;
};