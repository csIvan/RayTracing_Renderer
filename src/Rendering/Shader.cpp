#include "Shader.h"
#include "RayTracer.h"
#include "RayMarcher.h"

Shader::Shader(Renderer *renderer, vector<Light *> lights, vector<SceneObject *> objects) {
	this->lights = lights;
	this->objects = objects;
	this->renderer = renderer;
}

ofColor Shader::getColor(Ray &ray, const glm::vec3 &point, const glm::vec3 &norm, SceneObject *obj, int depth) {
	// Check material
	ofColor color;
	switch (obj->objMaterial.getType()) {
		case Material::MATTE:
			color = lambert(ray, point, norm, obj, depth);
			break;
		case Material::MIRROR:
		case Material::GLASS:
		case Material::METAL:
		case Material::CUSTOM:
			color = phong(ray, point, norm, renderer->renderCam->position, obj, depth);
			break;
	}

	return color;
}

ofColor Shader::lambert(Ray &ray, const glm::vec3 &point, const glm::vec3 &normal, SceneObject* obj, int depth) {
	ofColor kd = obj->objMaterial.diffuseColor;
	float ambientCo = 0.08;
	ofColor lambertColor = kd * ambientCo;
	for (int i = 0; i < lights.size(); i++) {
		glm::vec3 L;
		float D, I;

		if (dynamic_cast<AreaLight*>(lights[i]) != nullptr) {
			AreaLight *areaLight = (AreaLight*)lights[i];
			glm::vec3 shadowColor = glm::vec3(0.0f, 0.0f, 0.0f);
			glm::vec3 diffuseVec = glm::vec3(kd.r, kd.g, kd.b);

			for (int u = 0; u < areaLight->usteps; u++) {
				for (int v = 0; v < areaLight->vsteps; v++) {
					glm::vec3 sample = areaLight->pointOnLight(u, v);
					L = areaLight->getLightDir(sample, point);
					D = areaLight->getLightDist(sample, point);
					I = areaLight->getLightIntensity((areaLight->intensity / areaLight->samples), D);

					Ray shadRay = Ray(point + normal * 0.001f, L);
					glm::vec3 lambertCalculation = diffuseVec * I * glm::max(0.0f, glm::dot(normal, L));

					if (!inShadow(shadRay, point, D, false)) {
						shadowColor += lambertCalculation;
					}
				}
			}

			lambertColor += ofColor(shadowColor.x, shadowColor.y, shadowColor.z);
		}
		else {
			L = lights[i]->getLightDir(lights[i]->position, point);
			D = lights[i]->getLightDist(lights[i]->position, point);
			I = lights[i]->getLightIntensity(lights[i]->intensity, D);

			Ray shadRay = Ray(point + normal * 0.001f, L);

			ofColor lambertCalculation = kd * I * glm::max(0.0f, glm::dot(normal, L));
			opoint = point;
			onormal = normal;
			if (!inShadow(shadRay, point, D, false)) {
				if (dynamic_cast<SpotLight*>(lights[i]) != nullptr) {
					SpotLight *spotLight = (SpotLight*)lights[i];
					glm::vec3 dir = spotLight->direction;
					glm::vec4 rdd = spotLight->getRotateMatrix() * glm::vec4(dir.x, dir.y, dir.z, 1.0f);
					float SpotFactor = glm::dot(-L, glm::normalize(glm::vec3(rdd.x, rdd.y, rdd.z)));

					//call to calculate the falloff factor for the spotlight
					float falloff = spotLight->falloff(SpotFactor);
					lambertColor += lambertCalculation * falloff;
				}
				else
					lambertColor += lambertCalculation;
			}
		}

	}
	return lambertColor;
}

ofColor Shader::lambert(Ray &ray, const glm::vec3 &point, const glm::vec3 &normal, SceneObject* obj, float reflectV, int depth) {
	ofColor kd = obj->objMaterial.diffuseColor;
	float ambientCo = 0.08;
	ofColor lambertColor = kd * ambientCo;
	for (int i = 0; i < lights.size(); i++) {
		glm::vec3 L;
		float D, I;

		if (dynamic_cast<AreaLight*>(lights[i]) != nullptr) {
			AreaLight *areaLight = (AreaLight*)lights[i];
			glm::vec3 shadowColor = glm::vec3(0.0f, 0.0f, 0.0f);
			glm::vec3 diffuseVec = glm::vec3(kd.r, kd.g, kd.b);

			for (int u = 0; u < areaLight->usteps; u++) {
				for (int v = 0; v < areaLight->vsteps; v++) {
					glm::vec3 sample = areaLight->pointOnLight(u, v);
					L = areaLight->getLightDir(sample, point);
					D = areaLight->getLightDist(sample, point);
					I = areaLight->getLightIntensity((areaLight->intensity / areaLight->samples), D);

					Ray shadRay = Ray(point + normal * 0.001f, L);
					Ray ReflectRay = reflect(point, -ray.d, normal, false);

					ofColor reflectColor = 0;
					ofColor cTemp;
					bool reflected = false;

					if (reflectV > 0.0) {
						glm::vec3 inter;
						glm::vec3 Nor;
						if (renderer->castRay(ReflectRay, cTemp, inter, Nor, depth + 1)) {
							reflectColor = cTemp;
							reflected = true;
						}
					}

					glm::vec3 reflectVec = glm::vec3(reflectColor.r / 80.0, reflectColor.g / 80.0, reflectColor.b / 80.0);

					glm::vec3 lambertCalculation = diffuseVec * I * glm::max(0.0f, glm::dot(normal, L));

					if (!inShadow(shadRay, point, D, false)) {
						if (reflected) {
							lambertCalculation += reflectV * reflectVec;
						}
						shadowColor += lambertCalculation;
					}
					else {
						if (reflected) {
							shadowColor += lambertCalculation + reflectV * reflectVec;
						}
					}
				}
			}

			lambertColor += ofColor(shadowColor.x, shadowColor.y, shadowColor.z);
		}
		else {
			L = lights[i]->getLightDir(lights[i]->position, point);
			D = lights[i]->getLightDist(lights[i]->position, point);
			I = lights[i]->getLightIntensity(lights[i]->intensity, D);

			Ray shadRay = Ray(point + normal * 0.001f, L);
			Ray ReflectRay = reflect(point, -ray.d, normal, false);

			ofColor reflectColor = 0;
			ofColor cTemp;
			bool reflected = false;
			if (reflectV > 0.0) {
				glm::vec3 inter;
				glm::vec3 Nor;
				if (renderer->castRay(ReflectRay, cTemp, inter, Nor, depth + 1)) {
					reflectColor = cTemp;
					reflected = true;
				}
			}

			ofColor lambertCalculation = kd * I * glm::max(0.0f, glm::dot(normal, L));
			opoint = point;
			onormal = normal;
			if (!inShadow(shadRay, point, D, false)) {
				if (reflected) {
					lambertCalculation += reflectV * reflectColor;
				}
				
				if (dynamic_cast<SpotLight*>(lights[i]) != nullptr) {
					SpotLight *spotLight = (SpotLight*)lights[i];
					glm::vec3 dir = spotLight->direction;
					glm::vec4 rdd = spotLight->getRotateMatrix() * glm::vec4(dir.x, dir.y, dir.z, 1.0f);
					float SpotFactor = glm::dot(-L, glm::normalize(glm::vec3(rdd.x, rdd.y, rdd.z)));

					//call to calculate the falloff factor for the spotlight
					float falloff = spotLight->falloff(SpotFactor);
					lambertColor += lambertCalculation * falloff;
				}
				else
					lambertColor += lambertCalculation;
			}
			else {
				if (reflected) {
					lambertColor += lambertCalculation + reflectV * reflectColor;
				}

			}
		}

	}
	return lambertColor;
}

ofColor Shader::phong(Ray &ray, const glm::vec3 &point, const glm::vec3 &normal, const glm::vec3 camPos, SceneObject* obj, int depth) {
	ofColor kd = obj->objMaterial.diffuseColor;
	ofColor ks = obj->objMaterial.specularColor;

	float ambientCo = 0.08;
	ofColor phongColor = kd * ambientCo;
	for (int i = 0; i < lights.size(); i++) {
		glm::vec3 L;
		float D, I;

		L = lights[i]->getLightDir(lights[i]->position, point);
		D = lights[i]->getLightDist(lights[i]->position, point);
		//float SpotFactor = glm::dot(-L, lights[i]->direction);
		glm::vec3 v = -glm::normalize(point - camPos);
		glm::vec3 h = glm::normalize(v + L);
		Ray shadRay = Ray(point + normal * 0.001f, L);
		//if (normal == glm::vec3(0, 1, 0))
			//shadRay = Ray(glm::vec3(point.x, point.y + .01f, point.z), l);

		//float kr = obj->objMaterial.reflection;
		float kr;
		fresnel(point, ray.d, normal, obj->objMaterial.refraction, kr);
		bool outside = (glm::dot(-ray.d, normal) < 0);
		glm::vec3 bias = 0.0001f * normal;

		ofColor refractColor = 0;
		ofColor colorTemp;
		bool refracted = false;
		if (kr < 1) {
			Ray refractRay = refract(point, ray.d, normal, obj->objMaterial.refraction, outside);
			glm::vec3 inter;
			glm::vec3 Nor;
			if (renderer->castRay(refractRay, colorTemp, inter, Nor, depth + 1)) {
				glm::vec3 newInter;
				glm::vec3 newNor;
				Ray refractRay2 = refract(inter, refractRay.d, Nor, obj->objMaterial.refraction, true);
				if (renderer->castRay(refractRay2, refractColor, newInter, newNor, depth + 1)) {
					opoint = refractRay.p;
					opoint2 = newInter;
					onormal = refractRay.d;
					refracted = true;
				}
			} 
		}

		Ray ReflectRay = reflect(point, -ray.d, normal, outside);

		ofColor reflectColor = 0;
		bool reflected = false;
		if (kr > 0.0) {
			glm::vec3 inter;
			glm::vec3 Nor;
			if (renderer->castRay(ReflectRay, reflectColor, inter, Nor, depth + 1)) {
				reflected = true;
			}
		}
		//call to calculate the falloff factor for the spotlight
		//float falloff = lights[i]->falloff(SpotFactor);

		ofColor phongCalculation = obj->objMaterial.roughness * (kd * (lights[i]->intensity / glm::pow(D, 2)) * glm::max(0.0f, glm::dot(normal, L))) +
			ks * (lights[i]->intensity / glm::pow(D, 2)) * (glm::pow(glm::max(0.0f, glm::dot(normal, h)), obj->objMaterial.shininess));

		if (!inShadow(shadRay, point, D, refracted)) {
			if (refracted || reflected) {
				phongCalculation += reflectColor * kr + refractColor * (1 - kr);
			}

			phongColor += phongCalculation;
			//phongColor += phongCalculation * falloff;
		}
		else {
			if (refracted || reflected) {
				phongColor += phongCalculation + reflectColor * kr + refractColor * (1 - kr);;
			}

		}
	}
	return phongColor;
}

bool Shader::inShadow(const Ray &r, glm::vec3 hitPoint, float lightDistance, bool ref) {
	float objectDistance = 0.0f;
	float dist = FLT_MAX;
	for (int index = 0; index < objects.size(); index++) {
		glm::vec3 point;
		glm::vec3 normal;

		//calculate shadows using rayMarch()
		//if (rayMarch(r, point)) {
		//	blocked = true;
		//}
		//if (dynamic_cast<Plane*>(objects[index]) != nullptr) {
		//	Plane *sphereSelected = (Plane*)objects[index];
		//	//sphereSelected->rays.push_back(r.d);
		//	sphereSelected->points.push_back(opoint);
		//	sphereSelected->normals.push_back((opoint + onormal / 2));
		//}
		if (dynamic_cast<Sphere*>(objects[index]) != nullptr) {
			Sphere *sphereSelected = (Sphere*)objects[index];
			if (ref) {
				sphereSelected->points.push_back(opoint);
				sphereSelected->points2.push_back(opoint2);
				sphereSelected->normals.push_back(onormal);
			}
			
		}
		if (objects[index]->intersect(r, point, normal)) {
			objectDistance = glm::distance(point, hitPoint);
			if (objectDistance < dist) {
				dist = objectDistance;
			}
			//if (dynamic_cast<Sphere*>(objects[index]) != nullptr) {
			//Sphere *sphereSelected = (Sphere*)objects[index];
			//sphereSelected->points.push_back(opoint);
			//sphereSelected->normals.push_back((opoint + onormal / 2));
			//}
			//if (dynamic_cast<Cylinder*>(objects[index]) != nullptr) {
			//	Cylinder *sphereSelected = (Cylinder*)objects[index];
			//	sphereSelected->points.push_back(point);
			//	sphereSelected->normals.push_back((point + normal / 2));
			//}
		}
	}

	if (dist < lightDistance) {
		return true;
	}

	return false;
}


Ray Shader::reflect(glm::vec3 point, glm::vec3 viewRay, glm::vec3 normal, bool outside) {
	glm::vec3 reflectDir = 2 * glm::dot(normal, viewRay) * normal - viewRay;
	glm::vec3 reflectOrigin = outside ? point - normal * 0.001f : point + normal * 0.001f;
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

	glm::vec3 refractOrigin = outside ? point + normal * 0.001f : point - normal * 0.001f;
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