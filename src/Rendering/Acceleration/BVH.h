#pragma once

#include "ofMain.h"
#include "../../Scene/SceneObject.h"

// Simple container for BVH tree node
struct BVHNode {
	AABB box;
	vector<SceneObject *> objects;
	BVHNode *left;
	BVHNode *right;

	BVHNode() : left(nullptr), right(nullptr) {};
	~BVHNode() {};
};


// Bounding Volume Hierarchy Acceleration Structure class
class BVH {
private:
	BVHNode *root;
	vector<BVHNode *> nodes;
	bool isEmpty;

public:
	BVH() : root(nullptr), isEmpty(true) {};
	~BVH() {};

	// Generate the BVH tree structure based on given objects
	void create(vector<SceneObject *> &objs);


	// Recursively create each BVH node in the tree
	void buildNode(vector<SceneObject *> objs, BVHNode *node);


	// Check if ray intersects the BVH. If it does, return sceneObjects inside the intersected BVH nodes
	bool intersect(const Ray &ray, vector<SceneObject *> &hitObjects);


	// Recursively check if given ray intersects given BVH node
	bool intersect(const Ray &ray, BVHNode *node, vector<SceneObject *> &hitObjects);


	// Draw BVH structure
	void draw();


	// Recursively draw bounding boxes of each BVH nodes
	void drawBVH(BVHNode *node);


	// Create a new box representing the combination of two given bounding boxes
	AABB combineBoxes(const AABB &abox, const AABB &bbox);


	// Order the vector of sceneObjects based on their bounding box center and given axis
	int sortObjects(vector<SceneObject *> &objs, int axis);

};