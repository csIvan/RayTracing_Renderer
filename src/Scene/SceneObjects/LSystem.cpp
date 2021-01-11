#include "LSystem.h"

LSystem::LSystem(glm::vec3 p, float t, string name, ofColor diffuse) {
	position = p;
	temp = t;
	objName = name;
	diffuseColor = diffuse;
	//rule1.a = 'X';
	rule1.a = 'A';
	rule1.b = "F-[[A]+A]+F[+FA][-A]";
	//rule1.b = "FF[+XZ++X-F[+ZX]][-X++F-X]";
	//rule1.a = 'A';
	//rule1.b = "[B]////[B]////[B]";
	rule2.a = 'F';
	//rule2.b = "FX[FX[+XF]]";
	rule2.b = "FF+[+F-F-F]-[-F+F+F]";
	//rule3.a = 'Z';
	//rule3.b = "[+F-X-F][++ZX]";
	rules.push_back(rule1);
	rules.push_back(rule2);
	generate(n);
	cout << "L-System: " << sentence << endl;
}

bool LSystem::intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal) {
	return (glm::intersectRaySphere(ray.p, ray.d, position, temp, point, normal));
}

void LSystem::draw() {
	ofDrawSphere(position, temp);
}

void LSystem::generate(int n) {
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
}

//L-System sdf
float LSystem::sdf(glm::vec3 p) {
	vector<float> dists;
	//Initial Angles
	float xangle = 0.0f;	//yaw xz
	float yangle = 0.0f;	//roll
	float zangle = 90.0f;	//pitch yz, yx

	//Applied angles
	float tAngle = 22.5f;
	float pAngle = 28.0f;
	float rAngle = 28.0f;

	x = position.x;
	y = position.y;
	z = position.z;
	float thickness = .02f;
	float height = 0.05f;
	float thickness2 = .05f;

	vector<float> ztempAngs;
	vector<float> xtempAngs;
	vector<float> ytempAngs;

	float tempzangle = 0.0f;
	float tempxangle = 0.0f;
	float tempyangle = 0.0f;
	glm::mat4 objM = glm::mat4(1.0);
	glm::mat4 M3 = glm::translate(objM, glm::vec3(x, y, z));
	objM = glm::translate(objM, glm::vec3((-1.0)*x, (-1.0)*y, (-1.0)*z));

	glm::mat4 jointM = glm::mat4(1.0);

	for (int i = 0; i < sentence.length(); i++) {
		if (sentence[i] == 'F' || sentence[i] == 'A') {
			//move forward
			glm::mat4 m = glm::translate(glm::vec3(height, 0, 0));
			glm::mat4 M = glm::rotate(glm::radians(zangle), glm::vec3(0, 0, 1));
			glm::mat4 M2 = glm::rotate(glm::radians(xangle), glm::vec3(0, 1, 0));
			glm::mat4 M3 = glm::rotate(glm::radians(yangle), glm::vec3(1, 0, 0));
			objM = m * M2 * M3 * M  * objM;

			glm::vec4 pp = objM * glm::vec4(p.x, p.y, p.z, 1.0);
			objM = m * objM;
			glm::vec2 d = abs(glm::vec2(length(glm::vec2(pp.z, pp.y)), pp.x)) - glm::vec2(thickness, height);
			dists.push_back(glm::min(glm::max(d.x, d.y), 0.0f) + length(glm::max(d, 0.0f)));



			//joints
			glm::mat4 jm = glm::translate(glm::vec3(x, y, z));
			glm::mat4 jM = glm::rotate(glm::radians(tempzangle), glm::vec3(0, 0, 1));
			glm::mat4 jM2 = glm::rotate(glm::radians(tempxangle), glm::vec3(1, 0, 0));
			glm::mat4 jM3 = glm::rotate(glm::radians(yangle), glm::vec3(0, 1, 0));
			jointM = jm * jM * jointM  * jM3 * jM2;

			glm::vec4 pj = jointM * glm::vec4(0, height * 2.0f, 0, 1.0);
			x = pj.x;
			y = pj.y;
			z = pj.z;;

			jointM = glm::translate(jointM, glm::vec3(0, height * 2.0f, 0));
			glm::mat4 resetm = glm::translate(glm::vec3((-1.0f) * x, (-1.0f) * y, (-1.0f) * z));
			jointM = resetm * jointM;

			float sphereDist = glm::distance(p, glm::vec3(pj.x, pj.y, pj.z)) - thickness;
			dists.push_back(sphereDist);

			//Reset angles
			tempzangle = 0.0f;
			tempxangle = 0.0f;
			tempyangle = 0.0f;
			zangle = 0.0f;
			xangle = 0.0f;
			yangle = 0.0f;
		}
		else if (sentence[i] == '+') {
			zangle += tAngle;
			tempzangle -= tAngle;
		}
		else if (sentence[i] == '-') {
			zangle -= tAngle;
			tempzangle += tAngle;
		}
		else if (sentence[i] == '/') {
			// Rotate along the z axis clockwise
			yangle += rAngle;
			tempyangle -= rAngle;
		}
		else if (sentence[i] == '\\') {
			// Rotate along the z axis counterclockwise
			yangle -= rAngle;
			tempyangle += rAngle;
		}
		else if (sentence[i] == '^') {
			// Pitch up, split it?
			xangle -= pAngle;
			tempxangle += pAngle;
		}
		else if (sentence[i] == '&') {
			// Pitch down, split it?
			xangle += pAngle;
			tempxangle -= pAngle;
			//zangle = 45.0f;
		}
		else if (sentence[i] == '[') {
			//branched = true;
			xVec.push_back(x);
			yVec.push_back(y);
			zVec.push_back(z);
			turnAngVec.push_back(xangle);
			rollAngVec.push_back(yangle);
			pitchAngVec.push_back(zangle);
			ztempAngs.push_back(tempzangle);
			xtempAngs.push_back(tempxangle);
			ytempAngs.push_back(tempyangle);
			mats.push_back(objM);
			jmats.push_back(jointM);
			//objM = glm::inverse(M2);
			//thickness -= 0.005;
			//height -= 0.001;
		}
		else if (sentence[i] == ']') {
			//branched = false;
			x = xVec[xVec.size() - 1];
			y = yVec[yVec.size() - 1];
			z = zVec[zVec.size() - 1];
			xangle = turnAngVec[turnAngVec.size() - 1];
			yangle = rollAngVec[rollAngVec.size() - 1];
			zangle = pitchAngVec[pitchAngVec.size() - 1];
			tempzangle = ztempAngs[ztempAngs.size() - 1];
			tempxangle = xtempAngs[xtempAngs.size() - 1];
			tempyangle = ytempAngs[ytempAngs.size() - 1];
			objM = mats[mats.size() - 1];
			jointM = jmats[jmats.size() - 1];

			xVec.pop_back();
			yVec.pop_back();
			zVec.pop_back();
			turnAngVec.pop_back();
			pitchAngVec.pop_back();
			rollAngVec.pop_back();
			ztempAngs.pop_back();
			xtempAngs.pop_back();
			ytempAngs.pop_back();
			mats.pop_back();
			jmats.pop_back();
			//thickness += 0.005;
			//height += 0.001;
		}

	}
	return *min_element(dists.begin(), dists.end());
}