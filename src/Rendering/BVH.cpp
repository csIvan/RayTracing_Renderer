#include "BVH.h"

BVH::BVH() {
	position = glm::vec3(0, 0, 12);
}

void BVH::create(vector<SceneObject *> objs) {
	bvhObjs = objs;
	root = new BVHNode();
	buildNode(objs, root);
}

void BVH::buildNode(vector<SceneObject *> objs, BVHNode *node) {
	vector<SceneObject *> a;
	vector<SceneObject *> b;
	int n = objs.size();

	if (n == 0) {
		return;
	}
	
	node->objects = objs;

	if (n == 1) {
		node->left = nullptr;
		node->right = nullptr;
		node->box = node->objects[0]->box;
		findCentroid(node->box);
		cout << "1 object" << endl;
	}
	else if (n == 2) {
		node->left = new BVHNode();

		a.push_back(objs[0]);
		buildNode(a, node->left);

		b.push_back(objs[1]);
		node->right = new BVHNode();
		buildNode(b, node->right);

		//node->box = combineBoxes(node->left->box, node->right->box);

		cout << "2 objects" << endl;
	}
	else {
		//sortObjects(node->objects, 0);
		//node->left = new BVHNode();
		//
		//a.push_back(objs[0]);
		//buildNode(a, node->left);

		//b.push_back(objs[1]);
		//node->right = new BVHNode();
		//buildNode(b, node->right);

		//node->box = combineBoxes(node->left->box, node->right->box);

		cout << "more than 2" << endl;
	}

}

//Box BVH::combineBoxes(Box abox, Box bbox) {
//	return Box();
//}
//
glm::vec3 BVH::findCentroid(Box box) {
	points.push_back((box.parameters[0] + box.parameters[1]) / 2.f);
	cout << (box.parameters[0] + box.parameters[1]) / 2.f << endl;
	return (box.parameters[0] + box.parameters[1]) / 2.f;
}
//
//void BVH::sortObjects(vector<SceneObject *> objs, int axis) {
//	// Selection sort
//	for (int i = 0; i < objs.size(); i++) {
//		glm::vec3 icen = findCentroid(objs[i]->box);
//		int min = i;
//		int j = i + 1;
//		for (; j < objs.size(); j++) {
//			glm::vec3 jcen = findCentroid(objs[j]->box);
//			if (jcen[axis] < icen[axis]) {
//				min = j;
//			}
//		}
//		if (min != i) {
//			SceneObject* temp = objs[i];
//			objs[i] = objs[min];
//			objs[min] = temp;
//		}
//	}
//}

bool BVH::intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal, glm::vec2 &uv) {
	glm::vec3 rdd, roo, invdir, sign, t, tMinV, tMaxV, tMin, tMax;
	Ray rTemp = Ray(ray.p, ray.d);

	// Apply Transformation
	rdd = (glm::inverse(Transform) * glm::vec4(ray.d.x, ray.d.y, ray.d.z, 0.0));
	roo = (glm::inverse(Transform) * glm::vec4(ray.p.x, ray.p.y, ray.p.z, 1.0));

	return true;
}

void BVH::draw() {
	applyMatrix();
	ofDisableLighting();
	for (int i = 0; i < points.size(); i++) {
		ofSetColor(ofColor::red);
		ofDrawSphere(points[i], 0.1);		
		//ofSetColor(ofColor::blue);
		//ofDrawSphere(points2[i], 0.025);
		//ofSetColor(ofColor::yellow);
		//ofDrawLine(points[i], normals[i]);
		//cout << points2.size() << points2[i] << endl;
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

	drawBVH(node->left);
	node->objects[0]->box.draw();
	drawBVH(node->right);	

}