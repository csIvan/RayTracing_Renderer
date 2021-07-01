#include "RayTracer.h"

RayTracer::RayTracer(int imageWidth, int imageHeight, ofImage &image, RenderCam &cam) {
	this->imageWidth = imageWidth;
	this->imageHeight = imageHeight;
	this->image = image;
	this->renderCam = &cam;
}

ofImage RayTracer::render(int samples) {
	shader = Shader(this, lights, objects);
	for (float row = 0; row < imageHeight; row++) {
		for (float column = 0; column < imageWidth; column++) {

			glm::vec3 total = glm::vec3(0.0f, 0.0f, 0.0f);

			for (int i = 0; i < sqrt(samples); i++) {
				for (int j = 0; j < sqrt(samples); j++) {
					float jitter = (sqrt(samples) == 1) ? 0.5 : ((float)rand() / (RAND_MAX));

					Ray ray = renderCam->getRay((column + (j + jitter) / sqrt(samples)) / imageWidth,
						1 - (row + (i + jitter) / sqrt(samples)) / imageHeight);
					ofColor color;
					glm::vec3 ptest, ntest;
					if (castRay(ray, color, ptest, ntest))
						total += glm::vec3(color.r, color.g, color.b);
					else
						total += glm::vec3(ofColor::black.r, ofColor::black.g, ofColor::black.b);
				}
			}
			total /= samples;
			image.setColor(column, row, ofColor(total.x, total.y, total.z));
		}

		int percent = (int)(row / imageHeight * 100) + 1;
		printf("\rRendering... %d%%", percent);
	}

	return image;
}

void RayTracer::mtRender(glm::vec2 start, glm::vec2 dim, int samples, float &percent){
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
					glm::vec3 ptest, ntest;
					if (castRay(ray, color, ptest, ntest))
						total += glm::vec3(color.r, color.g, color.b);
					else
						total += glm::vec3(10, 10, 10);
				}
			}
			total /= samples;
			image.setColor(column, row, ofColor(total.x, total.y, total.z));
		}
		percent += 0.03125f; //  100 / 16 threads / 200 height = 0.03125
		printf("\rRendering... %d%%", (int)percent);
	}
}

bool RayTracer::castRay(Ray &ray, ofColor &color, glm::vec3 &p, glm::vec3 &n, int depth) {
	bool hit = false;
	float dist;
	float nearestDist = FLT_MAX;

	if (depth > 2)
		return false;

	vector<SceneObject *> nodeObjs;
	bool inside = (bvh->isEmpty) ? false : bvh->root->box->inside(ray.p);

	if (bvh->intersect(ray, bvh->root, nodeObjs, inside)) {
		glm::vec3 point, normal;
		ofColor surfaceColor;

		for (SceneObject *o : nodeObjs) {
			if (o->intersect(ray, point, normal, surfaceColor)) {
				dist = glm::distance(ray.p, point);
				if (dist <= nearestDist) {
					nearestDist = dist;
					hit = true;
					normal = glm::normalize(normal);
					p = point;
					n = normal;

					color = shader.getColor(ray, point, normal, surfaceColor, o, depth);
				}
			}
		}
	}

	return hit;
}

bool RayTracer::castNewRay(Ray &ray, ofColor &color, glm::vec3 &p, glm::vec3 &n, int depth) {
	bool hit = false;
	float dist;
	float nearestDist = FLT_MAX;

	if (depth > 2)
		return false;

	for (SceneObject *o : objects) {
		glm::vec3 point, normal;
		ofColor surfaceColor;
		if (o->intersect(ray, point, normal, surfaceColor)) {
			dist = glm::distance(ray.p, point);
			if (dist <= nearestDist) {
				nearestDist = dist;
				hit = true;
				normal = glm::normalize(normal);
				p = point;
				n = normal;

				color = shader.getColor(ray, point, normal, surfaceColor, o, depth);
			}
		}
	}
	

	return hit;
}
