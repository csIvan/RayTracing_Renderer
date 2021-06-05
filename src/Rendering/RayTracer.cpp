#include "RayTracer.h"

RayTracer::RayTracer(int imageWidth, int imageHeight, ofImage &image, RenderCam &cam) {
	this->imageWidth = imageWidth;
	this->imageHeight = imageHeight;
	this->image = image;
	this->renderCam = &cam;

	texture.load("images/texture2.jpg");
	sphereTexture.load("images/WorldMap.jpg");
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

		int percent = (int)(row / imageHeight * 100) + 1;
		printf("\rRendering... %d%%", percent);
	}

	return image;
}

bool RayTracer::castRay(Ray &ray, ofColor &color, glm::vec3 &p, glm::vec3 &n, int depth) {
	bool hit = false;
	float dist;
	float nearestDist = FLT_MAX;

	if (depth > 1)
		return false;



	vector<SceneObject *> nodeObjs;
	bool inside = bvh->root->box->inside(ray.p);

	if (bvh->intersect(ray, bvh->root, nodeObjs, inside)) {
		glm::vec3 point, normal;
		glm::vec2 uv;

		//cout << "name: " << o->objName << endl;
		for (SceneObject *o : nodeObjs) {
			if (o->intersect(ray, point, normal, uv)) {
				//cout << "params: " << o->box.min() << " , " <<  o->box.max() << endl;
				dist = glm::distance(ray.p, point);
				if (dist <= nearestDist) {
					nearestDist = dist;
					hit = true;
					normal = glm::normalize(normal);
					p = point;
					n = normal;

					color = shader.getColor(ray, point, normal, uv, o, depth);
				}
			}
		}
	}

	//for (int index = 0; index < objects.size(); index++) {
	//	glm::vec3 point, normal;
	//	glm::vec2 uv;

	//	if (objects[index]->intersect(ray, point, normal, uv)) {
	//		dist = glm::distance(ray.p, point);
	//		if (dist <= nearestDist) {
	//			nearestDist = dist;
	//			hit = true;
	//			normal = glm::normalize(normal);
	//			p = point;
	//			n = normal;
	//			
	//			color = shader.getColor(ray, point, normal, uv, objects[index], depth);
	//		}
	//	}
	//}

	return hit;
}
