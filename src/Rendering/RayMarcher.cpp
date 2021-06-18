#include "RayMarcher.h"

RayMarcher::RayMarcher(int imageWidth, int imageHeight, ofImage image, RenderCam &cam) {
	this->imageWidth = imageWidth;
	this->imageHeight = imageHeight;
	this->image = image;
	this->renderCam = &cam;
}

/*
* RayMarching function. Calls rayMarch instead of the object's ray intersect function.
* Also uses the normalRM so that phong shading can be applied to the scene
*/
ofImage RayMarcher::render(int samples) {
	shader = Shader(this, lights, objects);
	for (float row = 0; row < imageHeight; row++) {
		for (float column = 0; column < imageWidth; column++) {

			glm::vec3 total = glm::vec3(0.0f, 0.0f, 0.0f);

			for (int i = 0; i < sqrt(samples); i++) {
				for (int j = 0; j < sqrt(samples); j++) {
					Ray ray = renderCam->getRay((column + (j + 0.5) / sqrt(samples)) / imageWidth,
						(row + (i + 0.5) / sqrt(samples)) / imageHeight);
					ofColor color;
					glm::vec3 ptest;
					glm::vec3 ntest;
					if (castRay(ray, color, ptest, ntest))
						total += glm::vec3(color.r, color.g, color.b);
					else
						total += glm::vec3(ofColor::black.r, ofColor::black.g, ofColor::black.b);
				}
			}
			total /= samples;
			image.setColor(column, imageHeight - row - 1, ofColor(total.x, total.y, total.z));
		}

		int percent = (int)(row / imageHeight * 100) + 1;
		printf("\rRendering... %d%%", percent);
	}
	return image;
}

void RayMarcher::mtRender(glm::vec2 start, glm::vec2 dim, int samples, float &percent) {
	int startX = (int)(start.x);
	int startY = (int)(start.y);
	int width = (int)dim.x + startX;
	int height = (int)dim.y + startY;

	for (float row = startY; row < height; row++) {
		for (float column = startX; column < width; column++) {

			glm::vec3 total = glm::vec3(0.0f, 0.0f, 0.0f);

			for (int i = 0; i < sqrt(samples); i++) {
				for (int j = 0; j < sqrt(samples); j++) {
					float jitter = (sqrt(samples) == 1) ? 0.5 : ((float)rand() / (RAND_MAX));

					Ray ray = renderCam->getRay((column + (j + jitter) / sqrt(samples)) / imageWidth,
						1 - (row + (i + jitter) / sqrt(samples)) / imageHeight);
					ofColor color;
					glm::vec3 ptest;
					glm::vec3 ntest;
					if (castRay(ray, color, ptest, ntest))
						total += glm::vec3(color.r, color.g, color.b);
					else
						total += glm::vec3(ofColor::black.r, ofColor::black.g, ofColor::black.b);
				}
			}
			total /= samples;
			image.setColor(column, row, ofColor(total.x, total.y, total.z));
		}
		percent += 0.03125f; //  100 / 16 threads / 200 height = 0.03125
		printf("\rRendering... %d%%", (int)percent);
	}
}

bool RayMarcher::castRay(Ray &r, ofColor &color, glm::vec3 &po, glm::vec3 &n, int depth) {

	if (depth > 1)
		return false;

	glm::vec3 p;
	int index;
	bool hit = rayMarch(r, p, index);

	glm::vec3 normal = getNormalRM(p);
	
	if(hit)
		color = shader.getColor(r, p, normal, objects[index]->objMaterial.diffuseColor, objects[index], depth);

	return hit;
}

bool RayMarcher::rayMarch(Ray r, glm::vec3 &p, int &index) {
	bool hit = false;
	p = r.p;
	for (int i = 0; i < MAX_RAY_STEPS; i++) {
		int temp;
		float dist = sceneSDF(p, temp);
		if (dist < DIST_THRESHOLD) {
			hit = true;
			index = temp;
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
float RayMarcher::sceneSDF(glm::vec3 p, int &index) {
	float nearestDist = FLT_MAX;
	float d = 0.0;
	for (int i = 0; i < objects.size(); i++) {
		//WaterPool *wp1 = (WaterPool*)objects[i];
		//d = wp1->sdf(p, 1, 8);
		//d = opRep(p, glm::vec3(20.0f, 20.0f, 20.0f), *objects[i]);
		d = objects[i]->sdf(p);
		if (d <= nearestDist) {
			nearestDist = d;
			index = i;
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
	float eps = 0.000008;
	glm::vec2 k = glm::vec2(1, -1);

	int temp;

	float dp = sceneSDF(p, temp);
	//glm::vec3 n(dp - sceneSDF(glm::vec3(p.x - eps, p.y, p.z)),
	//	dp - sceneSDF(glm::vec3(p.x, p.y - eps, p.z)),
	//	dp - sceneSDF(glm::vec3(p.x, p.y, p.z - eps)));

	glm::vec3 n(glm::vec3(k.x, k.y, k.y) * sceneSDF(p + glm::vec3(k.x, k.y, k.y) * eps, temp) +
		glm::vec3(k.y, k.y, k.x) * sceneSDF(p + glm::vec3(k.y, k.y, k.x) * eps, temp) +
		glm::vec3(k.y, k.x, k.y) * sceneSDF(p + glm::vec3(k.y, k.x, k.y) * eps, temp) +
		glm::vec3(k.x, k.x, k.x) * sceneSDF(p + glm::vec3(k.x, k.x, k.x) * eps, temp));

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
