#include "BVH.h"

void BVH::create(vector<SceneObject *> &objs) {
	// Clean up previous nodes
	for (BVHNode *node : nodes) {
		delete node;
	}
	nodes.clear();

	if (objs.empty()) {
		isEmpty = true;
		return;
	}

	// Create new tree
	root = new BVHNode();
	nodes.push_back(root);
	isEmpty = false;
	buildNode(objs, root);
}

void BVH::buildNode(vector<SceneObject *> objs, BVHNode *node) {
	// Base case
	if (objs.size() == 1) {
		node->box = objs[0]->getBoundingBox();
		node->objects = objs;
		node->left = nullptr;
		node->right = nullptr;
		return;
	}

	AABB combinedBox = objs[0]->getBoundingBox();
	for (SceneObject *obj : objs) {
		combinedBox = combineBoxes(combinedBox, obj->getBoundingBox());
	}
	node->box = combinedBox;

	int axis = 0;
	glm::vec3 extents = node->box.getMax() - node->box.getMin();
	if (extents.y > extents.x && extents.y > extents.z) {
		axis = 1;
	}
	else if (extents.z > extents.x && extents.z > extents.y) {
		axis = 2;
	}

	int mid = sortObjects(objs, axis);
	vector<SceneObject *> leftObjs(objs.begin(), objs.begin() + mid);
	vector<SceneObject *> rightObjs(objs.begin() + mid, objs.end());

	node->left = new BVHNode();
	node->right = new BVHNode();
	nodes.push_back(node->left);
	nodes.push_back(node->right);

	buildNode(leftObjs, node->left);
	buildNode(rightObjs, node->right);

}

bool BVH::intersect(const Ray &ray, vector<SceneObject *> &hitObjects) {
	if (isEmpty) {
		return false;
	}

	return intersect(ray, root, hitObjects);
}

bool BVH::intersect(const Ray &ray, BVHNode *node, vector<SceneObject *> &hitObjects) {
	if (!node->box.intersect(ray)) {
		return false;
	}

	// If is leaf node
	if (node->left == nullptr && node->right == nullptr) {
		for (SceneObject *obj : node->objects) {
			hitObjects.push_back(obj);
		}
		return true;
	}

	bool hitLeft = node->left ? intersect(ray, node->left, hitObjects) : false;
	bool hitRight = node->right ? intersect(ray, node->right, hitObjects) : false;

	return hitLeft || hitRight;
}


void BVH::draw() {
	if (!isEmpty && root != nullptr) {
		drawBVH(root);
	}
}

void BVH::drawBVH(BVHNode *node) {
	if (node == nullptr) {
		return;
	}

	node->box.draw();
	drawBVH(node->left);
	drawBVH(node->right);
}

AABB BVH::combineBoxes(const AABB &abox, const AABB &bbox) {
	glm::vec3 min, max;
	min.x = glm::min(abox.getMin().x, bbox.getMin().x);
	min.y = glm::min(abox.getMin().y, bbox.getMin().y);
	min.z = glm::max(abox.getMin().z, bbox.getMin().z); // max() because min.z is in +z direction

	max.x = glm::max(abox.getMax().x, bbox.getMax().x);
	max.y = glm::max(abox.getMax().y, bbox.getMax().y);
	max.z = glm::min(abox.getMax().z, bbox.getMax().z);	// min() because max.z is in -z direction

	return AABB(min, max);
}

int BVH::sortObjects(std::vector<SceneObject *> &objs, int axis) {
	// Lambda compare
	auto compare = [axis](SceneObject *a, SceneObject *b) {
		return a->getBoundingBox().getCenter()[axis] < b->getBoundingBox().getCenter()[axis];
	};

	sort(objs.begin(), objs.end(), compare);
	return objs.size() / 2;
}