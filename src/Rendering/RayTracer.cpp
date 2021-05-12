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

	for (int index = 0; index < objects.size(); index++) {
		glm::vec3 point, normal;
		glm::vec2 uv;

		if (objects[index]->intersect(ray, point, normal, uv)) {
			dist = glm::distance(ray.p, point);
			if (dist <= nearestDist) {
				nearestDist = dist;
				hit = true;
				normal = glm::normalize(normal);
				p = point;
				n = normal;
				// If the object is a sphere
				//if (typeid(*objects[index]) == typeid(Sphere)) {
				//	Sphere *globe = (Sphere*)objects[index];

				//	glm::vec3 n = glm::normalize(point - globe->position);
				//	float u = atan2(n.x, n.z) / (2 * PI) + 0.5;
				//	float v = n.y * 0.5 + 0.5;


				//	float uLookUp = u * sphereTexture.getWidth() - .5;
				//	float pixelj = fmod(uLookUp, sphereTexture.getWidth());
				//	float vLookUp = v * sphereTexture.getHeight() - .5;
				//	float pixeli = fmod(vLookUp, sphereTexture.getHeight());

				//	ofColor globeColor = sphereTexture.getColor(pixelj, sphereTexture.getHeight() - pixeli - 1);
				//	color = shader.lambert(ray, point, normal, globeColor, 0, depth);
				//}
				//else if (typeid(*objects[index]) == typeid(Plane)) {
				//	// 10x10 tiles
				//	int uvTileFactor = 10;

				//	Plane *p1 = (Plane*)objects[index];

				//	float u = (p1->width + point.x) / (p1->width / uvTileFactor);
				//	float v = (p1->height + point.z) / (p1->height / uvTileFactor);

				//	// Find the color at the point of u and v
				//	float uLookUp = u * texture.getWidth() - .5;
				//	float pixelj = fmod(uLookUp, texture.getWidth());
				//	float vLookUp = v * texture.getHeight() - .5;
				//	float pixeli = fmod(vLookUp, texture.getHeight());

				//	// Apply the color of the texture using the shading algorithm
				//	ofColor planeColor = texture.getColor(pixelj, texture.getHeight() - pixeli - 1);

				//	//color = phong(point, normal, renderCam.position, planeColor, objects[index]->specularColor, Power);
				//	color = shader.lambert(ray, point, normal, planeColor, 0, depth);
				//}
				//else {
					//color = shader.phong(point, normal, renderCam.position, objects[index]->diffuseColor, objects[index]->specularColor, Power);
					color = shader.getColor(ray, point, normal, uv, objects[index], depth);
					//color = shader.lambert(ray, point, normal, objects[index]->objMaterial.diffuseColor, objects[index]->objMaterial.reflection, depth);
				//}
			}
		}
	}

	return hit;
}
