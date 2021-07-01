#include "Shader.h"
#include "RayTracer.h"
#include "RayMarcher.h"

Shader::Shader(Renderer *renderer, vector<Light *> lights, vector<SceneObject *> objects) {
	this->lights = lights;
	this->objects = objects;
	this->renderer = renderer;
}

ofColor Shader::getColor(Ray &ray, const glm::vec3 &point, const glm::vec3 &norm, const ofColor &surfaceColor, SceneObject *obj, int depth) {
	// Check material
	ofColor color;
	switch (obj->objMaterial.getType()) {
		case Material::MATTE:
			color = lambert(point, norm, surfaceColor, obj);
			break;
		case Material::MIRROR:
		case Material::GLASS:
		case Material::REFLECTIVE:
			color = phong(ray, point, norm, surfaceColor, obj, depth);
			break;
	}

	return color;
}

ofColor Shader::lambert(const glm::vec3 &point, const glm::vec3 &normal, const ofColor &surfaceColor, SceneObject* obj) {
	ofColor kd = surfaceColor;
	float ambientCo = obj->objMaterial.ambient;

	double totalLambert = 0;
	for (int i = 0; i < lights.size(); i++) {
		glm::vec3 L;
		double D, I;

		if (dynamic_cast<AreaLight*>(lights[i]) != nullptr) {
			AreaLight *areaLight = (AreaLight*)lights[i];

			for (int u = 0; u < areaLight->usteps; u++) {
				for (int v = 0; v < areaLight->vsteps; v++) {
					glm::vec3 sample = areaLight->pointOnLight(u, v);
					L = areaLight->getLightDir(sample, point);
					D = areaLight->getLightDist(sample, point);
					I = areaLight->getLightIntensity((areaLight->intensity / areaLight->samples), D);

					Ray shadRay = Ray(point + glm::normalize(L) * RAY_BIAS, glm::normalize(L));
					if (!inShadow(shadRay, point, D)) {
						totalLambert += I * glm::max(0.0f, glm::dot(normal, L));
					}
				}
			}
		}
		else {
			L = lights[i]->getLightDir(lights[i]->position, point);
			D = lights[i]->getLightDist(lights[i]->position, point);
			I = lights[i]->getLightIntensity(lights[i]->intensity, D);

			Ray shadRay = Ray(point + glm::normalize(L) * RAY_BIAS, glm::normalize(L));
			if (!inShadow(shadRay, point, D)) {
				if (dynamic_cast<SpotLight*>(lights[i]) != nullptr) {
					SpotLight *spotLight = (SpotLight*)lights[i];
					glm::vec3 dir = spotLight->direction;
					glm::vec4 rdd = spotLight->getRotateMatrix() * glm::vec4(dir.x, dir.y, dir.z, 1.0f);
					float spotFactor = glm::dot(-glm::normalize(L), glm::normalize(glm::vec3(rdd.x, rdd.y, rdd.z)));

					//call to calculate the falloff factor for the spotlight
					totalLambert += I * glm::max(0.0f, glm::dot(normal, L)) * spotLight->falloff(spotFactor);
				}
				else
					totalLambert += I * glm::max(0.0f, glm::dot(normal, L));
			}
		}
	}

	ofColor color = kd * ambientCo + kd * totalLambert;

	return color;
}


ofColor Shader::phong(Ray &ray, const glm::vec3 &point, const glm::vec3 &normal, const ofColor &surfaceColor, SceneObject* obj, int depth) {
	ofColor kd = surfaceColor;

	ofColor ks = obj->objMaterial.specularColor;
	float roughness = obj->objMaterial.roughness;
	float ambientCo = obj->objMaterial.ambient;
	float speculatCo = obj->objMaterial.specularCoeff;
	int power = obj->objMaterial.shininess;

	double totalLambert = 0, totalPhong = 0;
	for (int i = 0; i < lights.size(); i++) {
		glm::vec3 L;
		double D, I;

		if (dynamic_cast<AreaLight*>(lights[i]) != nullptr) {
			AreaLight *areaLight = (AreaLight*)lights[i];

			for (int u = 0; u < areaLight->usteps; u++) {
				for (int v = 0; v < areaLight->vsteps; v++) {
					glm::vec3 sample = areaLight->pointOnLight(u, v);
					L = areaLight->getLightDir(sample, point);
					D = areaLight->getLightDist(sample, point);
					I = areaLight->getLightIntensity((areaLight->intensity / areaLight->samples), D);
					glm::vec3 va = renderer->renderCam->position - point;
					glm::vec3 h = (va + L) / glm::length(va + L);

					Ray shadRay = Ray(point + glm::normalize(L) * RAY_BIAS, glm::normalize(L));
					if (!inShadow(shadRay, point, D)) {
						totalLambert += I * glm::max(0.0f, glm::dot(normal, L));
						totalPhong += I * (glm::pow(glm::max(0.0f, glm::dot(normal, glm::normalize(h))), power));
					}
				}
			}
		}
		else {
			L = lights[i]->getLightDir(lights[i]->position, point);
			D = lights[i]->getLightDist(lights[i]->position, point);
			I = lights[i]->getLightIntensity(lights[i]->intensity, D);
			glm::vec3 v = renderer->renderCam->position - point;
			glm::vec3 h = (v + L) / glm::length(v + L);

			Ray shadRay = Ray(point + glm::normalize(L) * RAY_BIAS, glm::normalize(L));
			if (!inShadow(shadRay, point, D)) {
				totalPhong += I * (glm::pow(glm::max(0.0f, glm::dot(normal, glm::normalize(h))), power));

				if (dynamic_cast<SpotLight*>(lights[i]) != nullptr) {
					SpotLight *spotLight = (SpotLight*)lights[i];
					glm::vec3 dir = spotLight->direction;
					glm::vec4 rdd = spotLight->getRotateMatrix() * glm::vec4(dir.x, dir.y, dir.z, 1.0f);
					float spotFactor = glm::dot(-glm::normalize(L), glm::normalize(glm::vec3(rdd.x, rdd.y, rdd.z)));

					//call to calculate the falloff factor for the spotlight
					totalLambert += I * glm::max(0.0f, glm::dot(normal, L)) * spotLight->falloff(spotFactor);
				}
				else
					totalLambert += I * glm::max(0.0f, glm::dot(normal, L));
			}
		}
	}

	ofColor color = kd * ambientCo + (roughness * kd * totalLambert) + ks * (totalPhong * speculatCo);

	ofColor refractColor = 0, reflectColor = 0;
	bool outside = (glm::dot(-ray.d, normal) < 0);
	float kr;

	fresnel(point, ray.d, normal, obj->objMaterial.refraction, kr);
	Ray refractRay = refract(point, ray.d, normal, obj->objMaterial.refraction, outside);
	Ray reflectRay = reflect(point, ray.d, normal, outside);

	if (kr < 1) {
		glm::vec3 refractHit, refractNor;
		if (renderer->castNewRay(refractRay, refractColor, refractHit, refractNor, depth + 1)) {

			Ray insideRefractRay = refract(refractHit, refractRay.d, refractNor, obj->objMaterial.refraction, true);
			renderer->castNewRay(insideRefractRay, refractColor, refractHit, refractNor, depth + 1);
		}
	}

	glm::vec3 refleftHit, refleftNor;
	if (renderer->castNewRay(reflectRay, reflectColor, refleftHit, refleftNor, depth + 1)) {
		opoint = reflectRay.p;
		opoint2 = refleftHit;
		onormal = reflectRay.d;
	}

	if (obj->objMaterial.materialString == "Reflective") {
		kr = obj->objMaterial.reflectiveCoeff;
		color = color * (1.0 - kr) + reflectColor * kr;
	}
	else {
		color += reflectColor * kr + refractColor * (1.0 - kr);
	}

	return color;
}

bool Shader::inShadow(const Ray &r, glm::vec3 hitPoint, float lightDistance) {
	float objectDistance = 0.0f;
	float dist = FLT_MAX;
	for (int index = 0; index < objects.size(); index++) {
		glm::vec3 point, normal;
		ofColor surfaceColor;

		if (objects[index]->intersect(r, point, normal, surfaceColor)) {
			objectDistance = glm::distance(point, hitPoint);
			if (objectDistance < dist) {
				dist = objectDistance;
			}
		}
	}

	if (dist < lightDistance) {
		return true;
	}

	return false;
}


Ray Shader::reflect(glm::vec3 point, glm::vec3 viewRay, glm::vec3 normal, bool outside) {
	glm::vec3 reflectDir = 2 * glm::dot(normal, -viewRay) * normal + viewRay;
	glm::vec3 reflectOrigin = outside ? point - normal * RAY_BIAS : point + normal * RAY_BIAS;
	return Ray(reflectOrigin, reflectDir);
}

Ray Shader::refract(glm::vec3 point, glm::vec3 viewRay, glm::vec3 normal, float ior, bool outside) {
	float cosi = glm::clamp(-1.0f, 1.0f, glm::dot(normal, viewRay));
	float etai = 1;
	float etat = ior;
	glm::vec3 n = normal;

	if (cosi < 0) { 
		cosi = -cosi; 
	}
	else { 
		std::swap(etai, etat); 
		n = -normal; 
	}

	float eta = etai / etat;
	float k = 1 - eta * eta * (1 - cosi * cosi);
	glm::vec3 refractDir = (k < 0) ? glm::vec3(0, 0, 0) : eta * viewRay + (eta * cosi - sqrtf(k)) * n;

	glm::vec3 refractOrigin = outside ? point + normal * RAY_BIAS : point - normal * RAY_BIAS;
	return Ray(refractOrigin, glm::normalize(refractDir));
}

void Shader::fresnel(glm::vec3 point, glm::vec3 viewRay, glm::vec3 normal, float ior, float &kr) {
	float cosi = glm::clamp(-1.0f, 1.0f, glm::dot(normal, viewRay));
	float etai = 1;
	float etat = ior;
	if (cosi > 0) { 
		std::swap(etai, etat); 
	}
	// Compute sini using Snell's law
	float sint = etai / etat * sqrtf(std::max(0.f, 1 - cosi * cosi));
	// Total internal reflection
	if (sint >= 1) {
		kr = 1;
	}
	else {
		float cost = sqrtf(std::max(0.f, 1 - sint * sint));
		cosi = fabsf(cosi);
		float Rs = ((etat * cosi) - (etai * cost)) / ((etat * cosi) + (etai * cost));
		float Rp = ((etai * cosi) - (etat * cost)) / ((etai * cosi) + (etat * cost));
		kr = (Rs * Rs + Rp * Rp) / 2;
	}
}