#include "WaterPool.h"

WaterPool::WaterPool(glm::vec3 p, float r, ofColor diffuse) {
	position = p;
	radius = r;
	diffuseColor = diffuse;
}

bool WaterPool::intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal) {
	return (glm::intersectRaySphere(ray.p, ray.d, position, radius, point, normal));
}

void WaterPool::draw() {
	// Proxy object for 3D scene
	ofDrawSphere(position, radius);
}

float WaterPool::sdf(glm::vec3 p, int amp, int octaves) {
	float total = 0;
	float freq = 0.06f;
	float ampl = 1;
	float maxVal = 0;
	float noise = 0;
	float pers = 0.5;
	for (int i = 0; i < 4; i++) {
		total += (ampl / 2) * glm::perlin(freq * p);
		maxVal += ampl;
		ampl *= pers;
		freq *= 2;
	}
	//noise = total / maxVal;
	noise = total;
	return p.y - (position.y + noise);
}