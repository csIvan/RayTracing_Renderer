#include "BVH.h"

BVH::BVH() {
	position = glm::vec3(0, 0, 12);
}

void BVH::create(vector<SceneObject *> objs) {
	nodes.clear();
	bvhObjs = objs;
	root = new BVHNode();
	levels = 0;
	buildNode(objs, root);
}

void BVH::buildNode(vector<SceneObject *> objs, BVHNode *node) {
	vector<SceneObject *> a;
	vector<SceneObject *> b;
	int n = objs.size();
	levels++;

	node->objects = objs;
	nodes.push_back(node);

	if (n == 1) {
		node->left = nullptr;
		node->right = nullptr;
		node->box = node->objects[0]->box;
		return;
	}
	else if (n == 2) {
		node->left = new BVHNode();
		a.push_back(objs[0]);
		buildNode(a, node->left);

		node->right = new BVHNode();
		b.push_back(objs[1]);
		buildNode(b, node->right);

		node->box = combineBoxes(node->left->box, node->right->box);
	}
	else {
		int k = sortObjects(node->objects, 0);

		node->left = new BVHNode();
		for (int i = 0; i <= k; i++) {
			a.push_back(node->objects[i]);
		}
		buildNode(a, node->left);

		node->right = new BVHNode();
		for (int i = k + 1; i < node->objects.size(); i++) {
			b.push_back(node->objects[i]);
		}
		buildNode(b, node->right);

		node->box = combineBoxes(node->left->box, node->right->box);
	}

}

Box* BVH::combineBoxes(Box *abox, Box *bbox) {
	glm::vec3 min, max;
	min.x = (abox->parameters[0].x < bbox->parameters[0].x) ? abox->parameters[0].x : bbox->parameters[0].x;
	min.y = (abox->parameters[0].y < bbox->parameters[0].y) ? abox->parameters[0].y : bbox->parameters[0].y;
	min.z = (abox->parameters[0].z > bbox->parameters[0].z) ? abox->parameters[0].z : bbox->parameters[0].z;

	max.x = (abox->parameters[1].x > bbox->parameters[1].x) ? abox->parameters[1].x : bbox->parameters[1].x;
	max.y = (abox->parameters[1].y > bbox->parameters[1].y) ? abox->parameters[1].y : bbox->parameters[1].y;
	max.z = (abox->parameters[1].z < bbox->parameters[1].z) ? abox->parameters[1].z : bbox->parameters[1].z;

	return new Box(min, max);
}

glm::vec3 BVH::findCentroid(Box *box) {
	return (box->parameters[0] + box->parameters[1]) / 2.f;
}

int BVH::sortObjects(vector<SceneObject *> &objs, int axis) {
	for (int i = 0; i < objs.size(); i++) {
		glm::vec3 icen = objs[i]->box->center();
		float closestDist = FLT_MAX;
		int index = i;
		for (int j = i + 1; j < objs.size(); j++) {
			glm::vec3 jcen = objs[j]->box->center();
			float dist = glm::distance(icen, jcen);
			if (dist < closestDist) {
				closestDist = dist;
				index = j;
			}
		}
		if (index != i) {
			SceneObject* temp = objs[i+1];
			objs[i+1] = objs[index];
			objs[index] = temp;
		}
	}

	float longestDist = 0;
	int indx = 0;
	for (int i = 0; i < objs.size(); i++) {
		glm::vec3 icen = objs[i]->box->center();
		if (i + 1 < objs.size()) {
			glm::vec3 jcen = objs[i + 1]->box->center();
			float dist = glm::distance(icen, jcen);
			if (dist > longestDist) {
				longestDist = dist;
				indx = i;
			}
		}
	}

	return indx;
}

bool BVH::intersect(const Ray &ray, BVHNode *node, vector<SceneObject *> &objs, bool inside) {
	bool hit = false;

	if (inside) {
		if (node->left == nullptr && node->right == nullptr) {
			objs.push_back(node->objects[0]);
			hit = true;
		}

		for (BVHNode *n : nodes) {
			if (n->left == nullptr && n->right == nullptr && n->box->intersect(ray)) {
				objs.push_back(n->objects[0]);
				hit = true;
			}
		}
	}
	else {
		if (node->box->intersect(ray)) {
			if (node->left == nullptr && node->right == nullptr) {
				// object intersection
				//cout << "test" << endl;
				objs.push_back(node->objects[0]);
				hit = true;
			}

			if (node->right != nullptr) {
				if (intersect(ray, node->right, objs, inside)) {
					hit = true;
				}
			}

			if (node->left != nullptr) {
				if (intersect(ray, node->left, objs, inside)) {
					hit = true;
				}
			}
		}
	}


	return hit;
}

void BVH::draw() {
	applyMatrix();
	ofDisableLighting();
	for (int i = 0; i < points.size(); i++) {
		ofSetColor(ofColor::red);
		ofDrawSphere(points[i], 0.1);		
	}

	drawBVH(root);

	ofEnableLighting();

	if (isSelected) {
		ofSetColor(ofColor::yellow);
		ofNoFill();
		ofPushMatrix();
			ofMultMatrix(Transform);
		ofPopMatrix();
		ofFill();
	}

	ofPushMatrix();
		ofMultMatrix(Transform);
		ofSetColor(ofColor::orange);
	ofPopMatrix();

}

void BVH::drawBVH(BVHNode *node) {
	if (node == nullptr ) {
		return;
	}

	node->box->draw();
	drawBVH(node->left);
	drawBVH(node->right);

}