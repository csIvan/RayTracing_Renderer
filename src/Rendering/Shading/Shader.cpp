#include "Shader.h"
#include "../../Scene/Scene.h"

//--------------------------------------------------------------
ofColor Shader::getColor(const Ray &ray, const HitInfo &hitInfo, const Material &mat, const Scene &scene, int depth) {
	ofColor color;
	switch (mat.getType()) {
	case Mat::MATTE:
		color = lambert(hitInfo, mat, scene);
		break;
	case Mat::MIRROR:
	case Mat::GLASS:
	case Mat::REFLECTIVE:
		color = phong(ray, hitInfo, mat, scene, depth);
		break;
	default:
		break;
	}

	return color;
}

//--------------------------------------------------------------
ofColor Shader::lambert(const HitInfo &hitInfo, const Material &mat, const Scene &scene) {
	ofColor kd = hitInfo.surfaceColor;
	float ambientCoeff = mat.getAmbient();
	double totalLambert = 0;

	for (Light* light : scene.getLights()) {
		glm::vec3 lightDirection;
		double distance, intensity;

		AreaLight *areaLight = dynamic_cast<AreaLight *>(light);
		SpotLight *spotLight = dynamic_cast<SpotLight *>(light);

		if (areaLight != nullptr) {
			int uSteps = areaLight->getUSteps();
			int vSteps = areaLight->getVSteps();
			for (int u = 0; u < uSteps; u++) {
				for (int v = 0; v < vSteps; v++) {
					glm::vec3 sample = areaLight->pointOnLight(u, v);
					lightDirection = areaLight->getLightDirection(sample, hitInfo.point);
					distance = areaLight->getDistance(sample, hitInfo.point);
					intensity = areaLight->getIntensity(distance);

					Ray shadowRay = Ray(hitInfo.point + lightDirection * RAY_BIAS, lightDirection);
					if (!inShadow(shadowRay, hitInfo.point, scene, distance)) {
						totalLambert += intensity * glm::max(0.0f, glm::dot(hitInfo.normal, (light->getPosition() - hitInfo.point)));
					}
				}
			}
		}
		else {
			lightDirection = light->getLightDirection(hitInfo.point);
			distance = light->getDistance(hitInfo.point);
			intensity = light->getIntensity(distance);

			Ray shadowRay = Ray(hitInfo.point + lightDirection * RAY_BIAS, lightDirection);
			if (!inShadow(shadowRay, hitInfo.point, scene, distance)) {
				if (spotLight != nullptr) {
					glm::vec3 rayDirection = spotLight->getRotateMatrix() * glm::vec4(spotLight->getDirection(), 1.0f);
					float spotFactor = glm::dot(-lightDirection, glm::normalize(rayDirection));
					totalLambert += intensity * glm::max(0.0f, glm::dot(hitInfo.normal, (light->getPosition() - hitInfo.point))) * spotLight->falloff(spotFactor);
				}
				else {
					totalLambert += intensity * glm::max(0.0f, glm::dot(hitInfo.normal, (light->getPosition() - hitInfo.point)));
				}
			}
		}	
	}
	ofColor color = (kd * ambientCoeff) + (kd * totalLambert);
	return color;
}

//--------------------------------------------------------------
ofColor Shader::phong(const Ray &ray, const HitInfo &hitInfo, const Material &mat, const Scene &scene, int depth) {
	ofColor kd = hitInfo.surfaceColor;

	ofColor ks = mat.getSpecular();
	float roughness = mat.getRoughness();
	float ambientCoeff = mat.getAmbient();
	float specularCoeff = mat.getSpecularCoeff();
	float refractCoeff = mat.getRefraction();
	int shininess = mat.getShininess();

	double totalLambert = 0;
	double totalPhong = 0;

	for (Light *light : scene.getLights()) {
		glm::vec3 lightDirection;
		double distance, intensity;

		AreaLight *areaLight = dynamic_cast<AreaLight *>(light);
		SpotLight *spotLight = dynamic_cast<SpotLight *>(light);

		if (areaLight != nullptr) {
			int uSteps = areaLight->getUSteps();
			int vSteps = areaLight->getVSteps();
			for (int u = 0; u < uSteps; u++) {
				for (int v = 0; v < vSteps; v++) {
					glm::vec3 sample = areaLight->pointOnLight(u, v);
					lightDirection = areaLight->getLightDirection(sample, hitInfo.point);
					distance = areaLight->getDistance(sample, hitInfo.point);
					intensity = areaLight->getIntensity(distance);
					glm::vec3 viewDirection = glm::normalize(ray.p - hitInfo.point);
					glm::vec3 halfVector = glm::normalize(viewDirection + lightDirection);

					Ray shadowRay = Ray(hitInfo.point + lightDirection * RAY_BIAS, lightDirection);
					if (!inShadow(shadowRay, hitInfo.point, scene, distance)) {
						totalLambert += intensity * glm::max(0.0f, glm::dot(hitInfo.normal, (light->getPosition() - hitInfo.point)));
						totalPhong += intensity * (glm::pow(glm::max(0.0f, glm::dot(hitInfo.normal, halfVector)), shininess));
					}
				}
			}
		}
		else {
			lightDirection = light->getLightDirection(hitInfo.point);
			distance = light->getDistance(hitInfo.point);
			intensity = light->getIntensity(distance);
			glm::vec3 viewDirection = glm::normalize(ray.p - hitInfo.point);
			glm::vec3 halfVector = glm::normalize(viewDirection + lightDirection);

			Ray shadowRay = Ray(hitInfo.point + lightDirection * RAY_BIAS, lightDirection);
			if (!inShadow(shadowRay, hitInfo.point, scene, distance)) {
				totalPhong += intensity * (glm::pow(glm::max(0.0f, glm::dot(hitInfo.normal, halfVector)), shininess));

				if (spotLight != nullptr) {
					glm::vec3 rayDirection = spotLight->getRotateMatrix() * glm::vec4(spotLight->getDirection(), 1.0f);
					float spotFactor = glm::dot(-lightDirection, glm::normalize(rayDirection));
					totalLambert += intensity * glm::max(0.0f, glm::dot(hitInfo.normal, (light->getPosition() - hitInfo.point))) * spotLight->falloff(spotFactor);
				}
				else {
					totalLambert += intensity * glm::max(0.0f, glm::dot(hitInfo.normal, (light->getPosition() - hitInfo.point)));
				}
			}
		}
	}

	ofColor color = (kd * ambientCoeff) + (kd * totalLambert * roughness) + (ks * totalPhong * specularCoeff);


	// Handle reflection and refraction
	bool outside = (glm::dot(ray.d, hitInfo.normal) < 0);
	float kr = getFresnelRatio(ray, hitInfo, refractCoeff);
	ofColor reflectColor = getReflectColor(ray, hitInfo, scene, outside, depth);
	ofColor refractColor = (kr < 1) ? getRefractColor(ray, hitInfo, scene, refractCoeff, outside, depth) : ofColor(0, 0, 0);

	float reflectionCoeff = mat.isReflective() ? mat.getReflective() : kr;
	float transmissionCoeff = 1.0f - reflectionCoeff;

	color = (color * transmissionCoeff) + (reflectColor * reflectionCoeff) + (refractColor * transmissionCoeff);

	return color;
}

//--------------------------------------------------------------
bool Shader::inShadow(const Ray &ray, const glm::vec3 &hitPoint, const Scene &scene, float lightDistance) {
	for (SceneObject* object : scene.getObjects()) {
		HitInfo hitInfo;
		if (object->intersect(ray, hitInfo)) {
			float objectDistance = glm::distance(hitInfo.point, hitPoint);
			if (objectDistance < lightDistance && objectDistance > RAY_BIAS) {
				return true;
			}
		}
	}

	return false;
}

//--------------------------------------------------------------
bool Shader::castSurfaceRay(const Ray &ray, HitInfo &hitInfo, const Scene &scene, int depth) {
	float nearestDist = FLT_MAX;

	if (depth > 2)
		return false;

	for (SceneObject *object : scene.getObjects()) {
		HitInfo objHit;
		if (object->intersect(ray, objHit)) {
			float dist = glm::distance(ray.p, objHit.point);
			if (dist <= nearestDist) {
				nearestDist = dist;
				hitInfo.hit = objHit.hit;
				hitInfo.point = objHit.point;
				hitInfo.normal = objHit.normal;
				hitInfo.surfaceColor = getColor(ray, objHit, object->getMaterial(), scene, depth);
			}
		}
	}

	return hitInfo.hit;
}

//--------------------------------------------------------------
ofColor Shader::getReflectColor(const Ray &ray, const HitInfo &hitInfo, const Scene &scene, bool outside, int depth) {
	Ray reflectRay = getReflectRay(ray, hitInfo, outside);
	HitInfo reflectHit;
	if (castSurfaceRay(reflectRay, reflectHit, scene, depth + 1)) {
		return reflectHit.surfaceColor;
	}

	return ofColor::black;
}

//--------------------------------------------------------------
ofColor Shader::getRefractColor(const Ray &ray, const HitInfo &hitInfo, const Scene &scene, float refractCoeff, bool outside, int depth) {
	Ray refractRay = getRefractRay(ray, hitInfo, refractCoeff, outside);
	HitInfo refractHit;
	ofColor finalColor = ofColor::black;
	if (castSurfaceRay(refractRay, refractHit, scene, depth + 1)) {
		Ray insideRefractRay = getRefractRay(refractRay, refractHit, refractCoeff, true);
		finalColor = refractHit.surfaceColor;
	}

	return finalColor;
}

//--------------------------------------------------------------
Ray Shader::getReflectRay(const Ray &ray, const HitInfo &hitInfo, bool outside) {
	glm::vec3 reflectDirection = glm::reflect(ray.d, hitInfo.normal);
	glm::vec3 reflectOrigin = outside ? hitInfo.point + hitInfo.normal * RAY_BIAS : hitInfo.point - hitInfo.normal * RAY_BIAS;
	
	return Ray(reflectOrigin, reflectDirection);
}

//--------------------------------------------------------------
// Refraction
// Code source from: https://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-to-shading/reflection-refraction-fresnel
Ray Shader::getRefractRay(const Ray &ray, const HitInfo &hitInfo, float ior, bool outside) {
	float cosi = glm::clamp(-1.0f, 1.0f, glm::dot(hitInfo.normal, ray.d));
	float etai = 1;
	float etat = ior;
	glm::vec3 n = hitInfo.normal;

	if (cosi < 0) {
		cosi = -cosi;
	}
	else {
		swap(etai, etat);
		n = -hitInfo.normal;
	}

	float eta = etai / etat;
	float k = 1 - eta * eta * (1 - cosi * cosi);
	glm::vec3 refractDirection = (k < 0) ? ZERO_VECTOR : eta * ray.d + (eta * cosi - sqrtf(k)) * n;
	glm::vec3 refractOrigin = outside ? hitInfo.point - hitInfo.normal * RAY_BIAS : hitInfo.point + hitInfo.normal * RAY_BIAS;

	return Ray(refractOrigin, refractDirection);
}

//--------------------------------------------------------------
// Fresnel
// Code source from: https://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-to-shading/reflection-refraction-fresnel
float Shader::getFresnelRatio(const Ray &ray, const HitInfo &hitInfo, float ior) {
	float cosi = glm::clamp(-1.0f, 1.0f, glm::dot(hitInfo.normal, ray.d));
	float etai = 1;
	float etat = ior;
	float kr = 0;
	if (cosi > 0) {
		swap(etai, etat);
	}

	// Compute sint using Snell's law
	float sint = etai / etat * sqrtf(std::max(0.0f, 1 - cosi * cosi));

	// Total internal reflection
	if (sint >= 1) {
		kr = 1;
	}
	else {
		float cost = sqrtf(std::max(0.0f, 1 - sint * sint));
		cosi = fabsf(cosi);
		float Rs = ((etat * cosi) - (etai * cost)) / ((etat * cosi) + (etai * cost));
		float Rp = ((etai * cosi) - (etat * cost)) / ((etai * cosi) + (etat * cost));
		kr = (Rs * Rs + Rp * Rp) / 2;
	}
	return kr;
}