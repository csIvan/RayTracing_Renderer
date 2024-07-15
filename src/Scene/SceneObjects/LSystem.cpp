#include "LSystem.h"

//--------------------------------------------------------------
LSystem::LSystem(const glm::vec3 &position, int iterations, const string &axiom, const string &name, const ofColor &diffuse) {
	this->position = position;
	this->iterations = iterations;
	this->axiom = axiom;
	objName = name;
	objMaterial.setDiffuse(diffuse);

	rule1.symbol = 'F';
	rule2.symbol = 'A';
	rule3.symbol = 'B';
	rule1.sequence = "F[+F][-F]";
	//rule1.sequence = "F-^F-^F";
	//rule2.sequence = "F-[[A]+A]+F[+FA][-A]";
	//rule1.sequence = "FF+[+F-F-F]-[-F+F+F]";
	//rule1.sequence = "F";
	//rule2.sequence = "[&FA]////[&FA]////[&FA]";
	setBounds();
}

//--------------------------------------------------------------
void LSystem::setBounds() {
	applyMatrix();
	glm::vec3 min = glm::vec3(100, 100, -100);
	glm::vec3 max = glm::vec3(-100, -100, 100);
	for (SceneObject *object : geometryObjects) {
		min.x = glm::min(object->getBoundingBox().getMin().x, min.x);
		min.y = glm::min(object->getBoundingBox().getMin().y, min.y);
		min.z = glm::max(object->getBoundingBox().getMin().z, min.z);

		max.x = glm::max(object->getBoundingBox().getMax().x, max.x);
		max.y = glm::max(object->getBoundingBox().getMax().y, max.y);
		max.z = glm::min(object->getBoundingBox().getMax().z, max.z);
	}
	box.setParameters(min, max);
	box.transformBox(Transform);
}


//--------------------------------------------------------------
void LSystem::draw() {
	applyMatrix();

	// check if object is selected
	if (isSelected) {
		ofDisableLighting();
		glLineWidth(0.1f);

		// Draw selection outline
		ofSetColor(SELECTED_COLOR);
		ofNoFill();
		ofPushMatrix();
		ofMultMatrix(Transform);
		ofDrawAxis(iterations * 1.5f);
		drawLSystem();
		ofPopMatrix();
		ofFill();

		glLineWidth(1.0f);
		ofEnableLighting();
	}

	// Draw 3D solid LSystem
	ofSetColor(ofColor::white);
	sceneMaterial.begin();
	sceneMaterial.setDiffuseColor(objMaterial.getDiffuse());
	ofPushMatrix();
	ofMultMatrix(Transform);
	drawLSystem();
	ofPopMatrix();
	sceneMaterial.end();
}

//--------------------------------------------------------------
void LSystem::drawLSystem() {
	for (SceneObject *object : geometryObjects) {
		object->setColor(objMaterial.getDiffuse());
		object->draw();
	}
}

//--------------------------------------------------------------
bool LSystem::intersect(const Ray &ray, HitInfo &hitInfo) {
	// Apply Transformation
	glm::vec3 rayOriginLocal = glm::inverse(Transform) * glm::vec4(ray.p, 1.0f);
	glm::vec3 rayDirLocal = glm::normalize(glm::inverse(Transform) * glm::vec4(ray.d, 0.0f));
	Ray localRay = Ray(rayOriginLocal, rayDirLocal);


	float nearestDist = FLT_MAX;
	for (SceneObject *object : geometryObjects) {
		HitInfo objectHit;
		if (object->intersect(localRay, objectHit)) {
			float dist = glm::distance(rayOriginLocal, objectHit.point);
			if (dist <= nearestDist) {
				nearestDist = dist;
				hitInfo.hit = true;
				hitInfo.normal = glm::normalize(objectHit.normal);
				hitInfo.point = objectHit.point;
				hitInfo.surfaceColor = objectHit.surfaceColor;
			}
		}
	}

	hitInfo.point = Transform * glm::vec4(hitInfo.point, 1.0f);
	return hitInfo.hit;
}


//--------------------------------------------------------------
float LSystem::sdf(const glm::vec3 &point) {
	glm::vec3 pointLocal = glm::inverse(Transform) * glm::vec4(point, 1.0f);
	
	float nearestDist = FLT_MAX;
	for (SceneObject *object : geometryObjects) {
		float dist = object->sdf(pointLocal);
		if (dist < nearestDist) {
			nearestDist = dist;
		}
	}

	return nearestDist;
}


//--------------------------------------------------------------
void LSystem::generate() {
	rules.clear();
	rules.push_back(rule1);
	rules.push_back(rule2);
	rules.push_back(rule3);
	sentence = axiom;
	int n = iterations;
	while (n > 0) {
		string nextSentence = "";
		for (int i = 0; i < sentence.size(); i++) {
			bool found = false;
			for (Rule r : rules) {
				if (sentence[i] == r.symbol) {
					found = true;
					nextSentence += r.sequence;
					break;
				}
			}

			if (!found) {
				nextSentence += sentence[i];
			}
		}
		sentence = nextSentence;
		n--;
	}

	build();
	setBounds();
}


//--------------------------------------------------------------
void LSystem::build() {
	geometryObjects.clear();
	if (sentence.empty()) {
		sentence = "F";
	}

	float angleX = 0.0f;	// yaw xz
	float angleY = 0.0f;	// roll
	float angleZ = 0.0f;	// pitch yz, yx

	vector<float> turnAngleVec, pitchAngleVec, rollAngleVec;
	vector<float> heights, widths;
	vector<glm::vec3> positions;
	vector<glm::mat4> rotMats;
	glm::mat4 rotMat = glm::mat4(1.0f);
	glm::vec3 currentPosition = ZERO_VECTOR;

	for (int i = 0; i < sentence.size(); i++) {
		if (sentence[i] == 'F') {

			glm::mat4 T = glm::translate(glm::vec3(0, tubeHeight, 0));
			glm::mat4 M = glm::rotate(glm::radians(angleZ), glm::vec3(0, 0, 1));
			glm::mat4 M2 = glm::rotate(glm::radians(angleY), glm::vec3(0, 1, 0));
			glm::mat4 M3 = glm::rotate(glm::radians(angleX), glm::vec3(1, 0, 0));
			rotMat =  M3 * M2 * M * rotMat;


			// Create cylinder (tube)
			SceneObject *tube = new Cylinder(ZERO_VECTOR, tubeHeight * 2, tubeRadius, "", ofColor::seaGreen);
			float x, y, z;
			glm::extractEulerAngleYXZ(glm::inverse(rotMat), y, x, z);
			tube->setRotation(glm::vec3(ofRadToDeg(x), ofRadToDeg(y), ofRadToDeg(z)));
    
			glm::vec3 forward = glm::vec3(0, tubeHeight, 0);
			glm::vec3 rotatedForward = glm::vec3(tube->getRotateMatrix() * glm::vec4(forward, 0.0f));
			currentPosition += rotatedForward;

			tube->setPosition(currentPosition);
			tube->setBounds();
			geometryObjects.push_back(tube);


			// Create joint at the end of the tube
			SceneObject *joint = new Sphere(ZERO_VECTOR, tubeRadius, "", ofColor::seaGreen);
			currentPosition += rotatedForward;
			joint->setPosition(currentPosition);
			joint->setBounds();
			geometryObjects.push_back(joint);

			// Reset angles
			angleX = 0.0f;
			angleY = 0.0f;
			angleZ = 0.0f;
		}
		else if (sentence[i] == '+') {
			angleZ += angle;
		}
		else if (sentence[i] == '-') {
			angleZ -= angle;
		}
		else if (sentence[i] == '\\') {		// z axis rotate clockwise
			angleY += angle;
		}
		else if (sentence[i] == '/') {		// z axis rotate counterclockwise
			angleY -= angle;
		}
		else if (sentence[i] == '&') {		// pitch down
			angleX += angle;
		}
		else if (sentence[i] == '^') {		// pitch up
			angleX -= angle;
		}
		else if (sentence[i] == '>') {		
			tubeHeight *= HEIGHT_MULTIPLIER;
			tubeRadius *= RADIUS_MULTIPLIER;
		}
		else if (sentence[i] == '<') {
			tubeHeight /= HEIGHT_MULTIPLIER;
			tubeRadius /= RADIUS_MULTIPLIER;
		}
		else if (sentence[i] == '[') {
			turnAngleVec.push_back(angleX);
			rollAngleVec.push_back(angleY);
			pitchAngleVec.push_back(angleZ);
			heights.push_back(tubeHeight);
			widths.push_back(tubeRadius);
			positions.push_back(currentPosition);
			rotMats.push_back(rotMat);
		}
		else if (sentence[i] == ']') {
			angleX = turnAngleVec[turnAngleVec.size() - 1];
			angleY = rollAngleVec[rollAngleVec.size() - 1];
			angleZ = pitchAngleVec[pitchAngleVec.size() - 1];
			tubeHeight = heights[heights.size() - 1];
			tubeRadius = widths[widths.size() - 1];
			currentPosition = positions[positions.size() - 1];
			rotMat = rotMats[rotMats.size() - 1];

			turnAngleVec.pop_back();
			rollAngleVec.pop_back();
			pitchAngleVec.pop_back();
			heights.pop_back();
			widths.pop_back();
			positions.pop_back();
			rotMats.pop_back();
		}
	}
}