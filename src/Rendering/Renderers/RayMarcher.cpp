#include "RayMarcher.h"
#include "../../Scene/Scene.h"

bool RayMarcher::castRay(const Ray &ray, ofColor &color, const Scene &scene, int depth) {
	int objectHitIndex;
	HitInfo hitInfo;
	if (rayMarch(ray, scene, hitInfo, objectHitIndex)) {
		color = shader.getColor(ray, hitInfo, scene.getObjects()[objectHitIndex]->getMaterial(), scene, depth);
	}
	return hitInfo.hit;
}


bool RayMarcher::rayMarch(const Ray &ray, const Scene &scene, HitInfo &hitInfo, int &objectHitIndex) {
	glm::vec3 point = ray.p;
	for (int i = 0; i < MAX_RAY_STEPS; i++) {
		float dist = sceneSDF(point, scene, objectHitIndex);
		if (dist < DIST_THRESHOLD) {
			hitInfo.hit = true;
			break;
		}
		else if (dist > MAX_THRESHOLD) {
			break;
		}
		else {
			point = point + ray.d * dist;
		}
	}

	if (hitInfo.hit) {
		hitInfo.point = point;
		hitInfo.normal = getNormalRM(point, scene);
		hitInfo.surfaceColor = scene.getObjects()[objectHitIndex]->getMaterial().getDiffuse();
	}
	
	return hitInfo.hit;
}

float RayMarcher::sceneSDF(const glm::vec3 &point, const Scene &scene, int &objectHitIndex) {
	float nearestDist = FLT_MAX;
	float dist = 0.0f;
	for (int i = 0; i < scene.getObjects().size(); i++) {
		dist = scene.getObjects()[i]->sdf(point);
		if (dist <= nearestDist) {
			nearestDist = dist;
			objectHitIndex = i;
		}
	}
	return nearestDist;
}

// Calculates Normal (using Tetrahedron Technique)
// Code source from: https://iquilezles.org/articles/normalsSDF/
glm::vec3 RayMarcher::getNormalRM(const glm::vec3 &point, const Scene &scene) {
	double eps = 0.000005;
	glm::vec2 k = glm::vec2(1, -1);
	int temp;

	glm::vec3 normal(
		glm::vec3(k.x, k.y, k.y) * sceneSDF(point + glm::vec3(k.x, k.y, k.y) * eps, scene, temp) +
		glm::vec3(k.y, k.y, k.x) * sceneSDF(point + glm::vec3(k.y, k.y, k.x) * eps, scene, temp) +
		glm::vec3(k.y, k.x, k.y) * sceneSDF(point + glm::vec3(k.y, k.x, k.y) * eps, scene, temp) +
		glm::vec3(k.x, k.x, k.x) * sceneSDF(point + glm::vec3(k.x, k.x, k.x) * eps, scene, temp)
	);

	return glm::normalize(normal);
}

