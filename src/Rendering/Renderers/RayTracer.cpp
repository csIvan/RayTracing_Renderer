#include "RayTracer.h"
#include "../../Scene/Scene.h"

bool RayTracer::castRay(const Ray &ray, ofColor &color, const Scene &scene, int depth) {
	bool hit = false;
	float nearestDist = FLT_MAX;

	if (depth > 2) {
		return false;
	}

	vector<SceneObject *> nodeObjs;
	if (scene.getBVH().intersect(ray, nodeObjs)) {		// use acceleration structure
		for (SceneObject *object : nodeObjs) {
			HitInfo hitInfo;
			if (object->intersect(ray, hitInfo)) {
				float dist = glm::distance(ray.p, hitInfo.point);
				if (dist <= nearestDist) {
					nearestDist = dist;
					hit = true;
					color = shader.getColor(ray, hitInfo, object->getMaterial(), scene, depth);
				}
			}
		}
	}
	
	return hit;
}