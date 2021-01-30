#include "RayTracer.h"

RayTracer::RayTracer(int imageWidth, int imageHeight, ofImage &image) {
	this->imageWidth = imageWidth;
	this->imageHeight = imageHeight;
	this->image = image;

	texture.load("images/texture2.jpg");
	sphereTexture.load("images/WorldMap.jpg");
}

/**
* To call the raytracing function, press the 'r' key
* Raytrace function. It is similar to the ray tracing function from the first project
* but it also allows for triangle-ray intersection. Also, it checks if the object is
* a sphere or a plane in order to apply the correct texture. It also calls for lambert
* shading because I thought that my scene looked better in lambert, but phong can also be used.
*/
ofImage RayTracer::render() {
	shader = Shader(lights, objects);
	for (float row = 0; row < imageHeight; row++) {
		for (float column = 0; column < imageWidth; column++) {
			Ray ray = renderCam.getRay(column / imageWidth, row / imageHeight);

			bool hit = false;
			float dist;
			ofColor color;

			for (int index = 0; index < objects.size(); index++) {
				glm::vec3 point, normal;
				
				if (dynamic_cast<Cylinder*>(objects[index]) != nullptr) {
					Cylinder *sphereSelected = (Cylinder*)objects[index];
					sphereSelected->imageX = column;
					sphereSelected->imageY = row;
				}

				if (objects[index]->intersect(ray, point, normal)) {
					dist = glm::distance(renderCam.position, point);
					if (dist < nearestDist) {
						nearestDist = dist;
						hit = true;
						normal = glm::normalize(normal);
						// If the object is a sphere
						if (typeid(*objects[index]) == typeid(Sphere)) {
							Sphere *globe = (Sphere*)objects[index];

							glm::vec3 n = glm::normalize(point - globe->position);
							float u = atan2(n.x, n.z) / (2 * PI) + 0.5;
							float v = n.y * 0.5 + 0.5;


							float uLookUp = u * sphereTexture.getWidth() - .5;
							float pixelj = fmod(uLookUp, sphereTexture.getWidth());
							float vLookUp = v * sphereTexture.getHeight() - .5;
							float pixeli = fmod(vLookUp, sphereTexture.getHeight());

							ofColor globeColor = sphereTexture.getColor(pixelj, sphereTexture.getHeight() - pixeli - 1);
							color = shader.lambert(point, normal, globeColor);
						}
						else if (typeid(*objects[index]) == typeid(Plane)) {
							// 10x10 tiles
							int uvTileFactor = 10;

							Plane *p1 = (Plane*)objects[index];

							float u = (p1->width + point.x) / (p1->width / uvTileFactor);
							float v = (p1->height + point.z) / (p1->height / uvTileFactor);

							// Find the color at the point of u and v
							float uLookUp = u * texture.getWidth() - .5;
							float pixelj = fmod(uLookUp, texture.getWidth());
							float vLookUp = v * texture.getHeight() - .5;
							float pixeli = fmod(vLookUp, texture.getHeight());

							// Apply the color of the texture using the shading algorithm
							ofColor planeColor = texture.getColor(pixelj, texture.getHeight() - pixeli - 1);

							//color = phong(point, normal, renderCam.position, planeColor, objects[index]->specularColor, Power);
							color = shader.lambert(point, normal, planeColor);
						}
						else {
							//color = shader.phong(point, normal, renderCam.position, objects[index]->diffuseColor, objects[index]->specularColor, Power);
							color = shader.lambert(point, normal, objects[index]->diffuseColor);
						}
					}
				}
			}
			nearestDist = FLT_MAX;

			if (hit) 
				image.setColor(column, imageHeight - row - 1, color);
			else
				image.setColor(column, imageHeight - row - 1, ofColor::black);
		}
		int percent = (int)(row / imageHeight * 100) + 1;
		printf("\rRendering... %d%%", percent);
	}
	return image;
}


void RayTracer::addObject(SceneObject &object) {
	objects.push_back(&object);
}

void RayTracer::addLight(Light &light) {
	lights.push_back(&light);
}

void RayTracer::remove(string name) {
	for (int i = 0; i < objects.size(); i++) {
		if (objects[i]->objName == name) {
			objects.erase(std::remove(objects.begin(), objects.end(), objects[i]), objects.end());
		}
	}
}