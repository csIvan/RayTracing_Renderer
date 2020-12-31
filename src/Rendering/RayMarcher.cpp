#include "RayMarcher.h"

RayMarcher::RayMarcher(int imageWidth, int imageHeight, ofImage image) {
	this->imageWidth = imageWidth;
	this->imageHeight = imageHeight;
	this->image = image;
}

/*
* RayMarching function. Calls rayMarch instead of the object's ray intersect function.
* Also uses the normalRM so that phong shading can be applied to the scene
*/
void RayMarcher::render() {
	shader = Shader(lights, objects);
	for (float row = 0; row < imageHeight; row++) {
		for (float column = 0; column < imageWidth; column++) {
			Ray ray = renderCam.getRay(column / imageWidth, row / imageHeight);

			ofColor color;
			glm::vec3 p;
			bool hit = rayMarch(ray, p);
			glm::vec3 normal = getNormalRM(p);

			//color = shader.phong(p, normal, renderCam.position, objects[indexHit]->diffuseColor, ofColor::lightGray, 50);
			color = shader.lambert(p, normal, objects[indexHit]->diffuseColor);

			if (hit) {
				//cout << "*** Hit ****************************************** " << p << endl << endl;
				image.setColor(column, imageHeight - row - 1, color);
			}
			else
				image.setColor(column, imageHeight - row - 1, ofColor::black);
		}
		//cout << i << "-";
	}
	image.save("images/RayMarch3.jpg");
}

bool RayMarcher::rayMarch(Ray r, glm::vec3 &p) {
	bool hit = false;
	p = r.p;
	for (int i = 0; i < MAX_RAY_STEPS; i++) {
		float dist = sceneSDF(p);
		if (dist < DIST_THRESHOLD) {
			hit = true;
			break;
		}
		else if (dist > MAX_THRESHOLD) {
			break;
		}
		else {
			p = p + r.d * dist;
		}
	}
	return hit;
}

//SceneSDF. Checks every primitive's sdf and determines the closest one to the point
//also marks the index of the object hit
float RayMarcher::sceneSDF(glm::vec3 p) {
	nearestDist = FLT_MAX;
	float d = 0.0;
	for (int i = 0; i < objects.size(); i++) {
		//WaterPool *wp1 = (WaterPool*)objects[i];
		//d = wp1->sdf(p, 1, 8);
		//d = opRep(p, glm::vec3(20.0f, 20.0f, 20.0f), *objects[i]);
		d = objects[i]->sdf(p);
		if (d < nearestDist) {
			nearestDist = d;
			indexHit = i;
		}
	}
	return nearestDist;
}

//Regular hard-coded getNormal function
glm::vec3 RayMarcher::getNormal(const glm::vec3 &p, int i) {
	glm::vec3 pNormal;
	//if ((typeid(*objects[i]) == typeid(Sphere)))
	//	pNormal = glm::normalize(p - objects[i]->position);
	//else if ((typeid(*objects[i]) == typeid(Plane)))
	//	pNormal = glm::vec3(0, 1, 0);

	return pNormal;
}

// Ray Marching getNormal Function
glm::vec3 RayMarcher::getNormalRM(const glm::vec3 &p) {
	float eps = 0.001;
	float dp = sceneSDF(p);
	glm::vec3 n(dp - sceneSDF(glm::vec3(p.x - eps, p.y, p.z)),
		dp - sceneSDF(glm::vec3(p.x, p.y - eps, p.z)),
		dp - sceneSDF(glm::vec3(p.x, p.y, p.z - eps)));
	return glm::normalize(n);
}

// opRef Function - it repeats infinitely
float RayMarcher::opRep(const glm::vec3 p, const glm::vec3 c, SceneObject &s) {
	//glm::vec3 q = glm::mod(p, c) - 10.0f * c;
	glm::vec3 q = glm::vec3(glm::mod(p.x, c.x) - 10.0f, glm::mod(p.y + 10.0f, c.y) - 10.0f, glm::mod(p.z + 10.0f, c.z) - 10.0f);

	// Round the hard edges before returning
	return opRound(q, s, 0.3f);
}

// opRound Function - it rounds the edges based on a float variable
float RayMarcher::opRound(const glm::vec3 p, SceneObject &s, float rad) {
	//return 2.0f;
	return s.sdf(p) - rad;
}


void RayMarcher::addObject(SceneObject &object) {
	objects.push_back(&object);
}

void RayMarcher::addLight(Light &light) {
	lights.push_back(&light);
}