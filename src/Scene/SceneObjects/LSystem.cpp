#include "LSystem.h"
#define HEIGHT_MULTIPLIER 0.7
#define RADIUS_MULTIPLIER 0.8

LSystem::LSystem(glm::vec3 p, int n, string ax, string name, ofColor diffuse) {
	position = p;
	iterations = n;
	axiom = ax;
	objName = name;
	objMaterial.diffuseColor = diffuse;

	rule1.a = 'F';
	rule2.a = 'A';
	rule3.a = 'B';
	rule1.b = "F-^F-^F";
	//rule1.b = "F[+F][-F]";
	//rule1.b = "F[&FA]////[&FA]////[&FA]";
	///rule1.a = 'X';
	//rule1.a = 'A';
	//rule1.b = "F-[[A]+A]+F[+FA][-A]";
	///rule1.b = "FF[+XZ++X-F[+ZX]][-X++F-X]";
	///rule1.a = 'A';
	///rule1.b = "[B]////[B]////[B]";
	//rule2.a = 'F';
	///rule2.b = "FX[FX[+XF]]";
	//rule2.b = "FF+[+F-F-F]-[-F+F+F]";
	///rule3.a = 'Z';
	///rule3.b = "[+F-X-F][++ZX]";
	box = new Box();

}

void LSystem::setBounds() {
	box->transformBox(Transform);
}

void LSystem::generate() {
	rules.push_back(rule1);
	rules.push_back(rule2);
	rules.push_back(rule3);
	sentence = axiom;
	int n = iterations;
	while (n > 0) {
		string nextSentence = "";
		for (int i = 0; i < sentence.length(); i++) {

			bool found = false;
			for (Rule r : rules) {
				if (sentence[i] == r.a) {
					found = true;
					nextSentence += r.b;
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

	// Create Bounding Box
	glm::vec3 point, normal;
	ofColor diffuse;
	build();
	glm::vec3 min = glm::vec3(100, 100, -100);
	glm::vec3 max = glm::vec3(-100, -100, 100);
	for (Box *b : boxes) {
		min.x = (b->min().x < min.x) ? b->min().x : min.x;
		min.y = (b->min().y < min.y) ? b->min().y : min.y;
		min.z = (b->min().z > min.z) ? b->min().z : min.z;

		max.x = (b->max().x > max.x) ? b->max().x : max.x;
		max.y = (b->max().y > max.y) ? b->max().y : max.y;
		max.z = (b->max().z < max.z) ? b->max().z : max.z;
	}
	rules.clear();


	applyMatrix();
	box->setParameters(min, max);
	setBounds();
}

void LSystem::build() {
	glm::vec3 point, normal;
	Ray ray = Ray(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0));
	boxes.clear();
	objs.clear();
	glm::vec3 rdd, roo;

	// Apply Transformation
	glm::vec4 p = glm::inverse(Transform) * glm::vec4(ray.p.x, ray.p.y, ray.p.z, 1.0);
	glm::vec4 p1 = glm::inverse(Transform) * glm::vec4(ray.p + ray.d, 1.0);
	roo = glm::vec4(p.x, p.y, p.z, 1.0);
	rdd = glm::normalize(p1 - p);
	Ray rTemp = Ray(roo, rdd);

	// Joint variables
	float x, y, z;
	x = y = z = 0.0f;

	//Initial Angles
	float xangle = 0.0f;	//yaw xz
	float yangle = 0.0f;	//roll
	float zangle = 0.0f;	//pitch yz, yx

	vector<float> heights, widths;
	vector<glm::mat4> mats, jmats, mrots;

	glm::mat4 objM = glm::mat4(1.0);
	bool isHit = false;
	float dist;
	float nearestDist = FLT_MAX;

	glm::mat4 jointM = glm::mat4(1.0);
	glm::mat4 mrot = glm::mat4(1.0);

	for (int i = 0; i < sentence.length(); i++) {
		if (sentence[i] == 'F') {
			//move forward
			glm::mat4 m = glm::translate(glm::vec3(0, -tubeHeight, 0));
			glm::mat4 M = glm::rotate(glm::radians(zangle), glm::vec3(0, 0, 1));
			glm::mat4 M2 = glm::rotate(glm::radians(yangle), glm::vec3(0, 1, 0));
			glm::mat4 M3 = glm::rotate(glm::radians(xangle), glm::vec3(1, 0, 0));
			objM = m * M3 * M2 * M  * objM;
			mrot = M3 * M2 * M * mrot;

			Cylinder *tube = new Cylinder(glm::vec3(0, 0, 0), tubeHeight * 2, tubeRadius, "", ofColor::seaGreen);
			tube->Transform =  glm::inverse(objM) ;
			tube->Rotate = getRotateMatrix() * glm::inverse(mrot);
			tube->box->transformBox(glm::inverse(objM));
			boxes.push_back(tube->box);
			objs.push_back(tube);
			objM = m * objM;



			if (!(sentence[i + 1] == NULL ||
				sentence[i + 1] != NULL && (sentence[i + 1] == ']' || sentence[i + 1] == 'A' || sentence[i + 1] == 'B'))) {
				//joints
				Sphere *joint = new Sphere(glm::vec3(0, 0, 0), tubeRadius, "", ofColor::seaGreen);
				joint->Transform = glm::inverse(objM);
				joint->Rotate = getRotateMatrix() * glm::inverse(mrot);
				joint->box->transformBox(glm::inverse(objM));
				boxes.push_back(joint->box);
				objs.push_back(joint);
			}

			//Reset angles
			zangle = 0.0f;
			xangle = 0.0f;
			yangle = 0.0f;

		}
		else if (sentence[i] == '+') {
			zangle += angle;
		}
		else if (sentence[i] == '-') {
			zangle -= angle;
		}
		else if (sentence[i] == '/') {
			// Rotate along the z axis clockwise
			yangle -= angle;
		}
		else if (sentence[i] == '\\') {
			// Rotate along the z axis counterclockwise
			yangle += angle;
		}
		else if (sentence[i] == '^') {
			// Pitch down
			xangle -= angle;
		}
		else if (sentence[i] == '&') {
			// Pitch up
			xangle += angle;
		}
		else if (sentence[i] == '>') {
			// Multiply the height by the height length scale factor
			tubeHeight *= HEIGHT_MULTIPLIER;
			tubeRadius *= RADIUS_MULTIPLIER;

		}
		else if (sentence[i] == '<') {
			// Divide the height by the height length scale factor
			tubeHeight /= HEIGHT_MULTIPLIER;
			tubeRadius /= RADIUS_MULTIPLIER;
		}
		else if (sentence[i] == '[') {
			xVec.push_back(x);
			yVec.push_back(y);
			zVec.push_back(z);
			turnAngVec.push_back(xangle);
			rollAngVec.push_back(yangle);
			pitchAngVec.push_back(zangle);
			heights.push_back(tubeHeight);
			widths.push_back(tubeRadius);
			mats.push_back(objM);
			jmats.push_back(jointM);
			mrots.push_back(mrot);
		}
		else if (sentence[i] == ']') {
			x = xVec[xVec.size() - 1];
			y = yVec[yVec.size() - 1];
			z = zVec[zVec.size() - 1];
			xangle = turnAngVec[turnAngVec.size() - 1];
			yangle = rollAngVec[rollAngVec.size() - 1];
			zangle = pitchAngVec[pitchAngVec.size() - 1];
			tubeHeight = heights[heights.size() - 1];
			tubeRadius = widths[widths.size() - 1];
			objM = mats[mats.size() - 1];
			jointM = jmats[jmats.size() - 1];
			mrot = mrots[mrots.size() - 1];

			xVec.pop_back();
			yVec.pop_back();
			zVec.pop_back();
			turnAngVec.pop_back();
			pitchAngVec.pop_back();
			rollAngVec.pop_back();
			heights.pop_back();
			widths.pop_back();
			mats.pop_back();
			jmats.pop_back();
			mrots.pop_back();
		}

	}
}

bool LSystem::intersect(const Ray &ray, glm::vec3 &p, glm::vec3 &n, ofColor &surfaceColor) {
	glm::vec3 rdd, roo;

	// Apply Transformation
	glm::vec4 pp = glm::inverse(Transform) * glm::vec4(ray.p.x, ray.p.y, ray.p.z, 1.0);
	glm::vec4 p1 = glm::inverse(Transform) * glm::vec4(ray.p + ray.d, 1.0);
	roo = glm::vec4(pp.x, pp.y, pp.z, 1.0);
	rdd = glm::normalize(p1 - pp);
	Ray rTemp = Ray(roo, rdd);

	bool hit = false;
	float dist;
	float nearestDist = FLT_MAX;
	surfaceColor = objMaterial.diffuseColor;
	for (SceneObject *o : objs) {
		glm::vec3 point, normal;
		ofColor surfaceColor;
		if (o->intersect(rTemp, point, normal, surfaceColor)) {
			dist = glm::distance(rTemp.p, point);
			if (dist <= nearestDist) {
				nearestDist = dist;
				hit = true;
				normal = glm::normalize(normal);
				p = point;
				n = normal;
			}
		}
	}
	p = Transform * glm::vec4(p.x, p.y, p.z, 1.0);
	//n = glm::normalize(getRotateMatrix() * glm::vec4(n.x, n.y, n.z, 1.0));
	return hit;
}

void LSystem::draw() {
	applyMatrix();
	ofDisableLighting();
	for (int i = 0; i < points.size(); i++) {
		ofSetColor(ofColor::red);
		//ofDrawSphere(points[i], 0.025);
		ofSetColor(ofColor::yellow);
		//ofDrawLine(points[i], normals[i]);
	}


	ofEnableLighting();
	if (isSelected) {
		ofDisableLighting();
		ofSetColor(ofColor::yellow);
		ofNoFill();
		ofPushMatrix();
			ofMultMatrix(Transform);
			ofDrawAxis(iterations * 1.5);
			drawLSystem();
		ofPopMatrix();
		ofFill();
		ofEnableLighting();
	}

	ofSetColor(ofColor::yellow);
	sceneMaterial.begin();
	sceneMaterial.setDiffuseColor(objMaterial.diffuseColor);
	ofPushMatrix();
		ofMultMatrix(Transform);
		drawLSystem();
	ofPopMatrix();
	sceneMaterial.end();

}

float LSystem::sdf(glm::vec3 p1) {
	glm::vec4 pp = glm::inverse(Transform) * glm::vec4(p1.x, p1.y, p1.z, 1.0);
	glm::vec3 p = glm::vec3(pp.x, pp.y, pp.z);

	float nearestDist = FLT_MAX;

	for (SceneObject *o : objs) {
		float dist = o->sdf(p);
		if (dist < nearestDist) {
			nearestDist = dist;
		}
	}
	
	return nearestDist;

}

void LSystem::drawLSystem() {
	for (SceneObject *o : objs) {
		o->draw();
	}
}