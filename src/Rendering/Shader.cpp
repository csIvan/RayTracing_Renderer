#include "Shader.h"
#include "RayTracer.h"
#include "RayMarcher.h"

Shader::Shader(Renderer *renderer, vector<Light *> lights, vector<SceneObject *> objects) {
	this->lights = lights;
	this->objects = objects;
	this->renderer = renderer;
}

ofColor Shader::lambert(Ray &ray, const glm::vec3 &point, const glm::vec3 &normal, const ofColor diffuse, float reflectV, int depth) {
	float ambientCo = 0.08;
	ofColor lambertColor = diffuse * ambientCo;
	for (int i = 0; i < lights.size(); i++) {
		glm::vec3 L, Li, lightToPixel;

		if (dynamic_cast<AreaLight*>(lights[i]) != nullptr) {
			AreaLight *areaLight = (AreaLight*)lights[i];
			glm::vec3 shadowColor = glm::vec3(0.0f, 0.0f, 0.0f);
			glm::vec3 diffuseVec = glm::vec3(diffuse.r, diffuse.g, diffuse.b);

			for (int u = 0; u < areaLight->usteps; u++) {
				for (int v = 0; v < areaLight->vsteps; v++) {
					glm::vec3 sample = areaLight->pointOnLight(u, v);
					Li = glm::normalize(sample - point);
					float dist2 = glm::distance(sample, point);
					float I2 = ((lights[i]->intensity / areaLight->samples) / (4 * PI * dist2));
					Ray shadRay = Ray(point + normal * 0.001f, Li);
					Ray ReflectRay = reflect(point, -ray.d, normal);

					ofColor reflectColor = 0;
					ofColor cTemp;
					bool reflected = false;

					if (reflectV > 0.0) {
						if (renderer->castRay(ReflectRay, cTemp, depth + 1)) {
							reflectColor = cTemp;
							reflected = true;
						}
					}

					glm::vec3 reflectVec = glm::vec3(reflectColor.r / 80.0, reflectColor.g / 80.0, reflectColor.b / 80.0);

					glm::vec3 lambertCalculation = diffuseVec * I2 * glm::max(0.0f, glm::dot(normal, Li));

					if (!inShadow(shadRay, point, dist2)) {
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
			L = glm::normalize(lights[i]->position - point);
			lightToPixel = glm::normalize(point - lights[i]->position);
			float dist = glm::distance(lights[i]->position, point);
			float I = (lights[i]->intensity / (4 * PI * dist));
			Ray shadRay = Ray(point + normal * 0.001f, L);
			Ray ReflectRay = reflect(point, -ray.d, normal);

			ofColor reflectColor = 0;
			ofColor cTemp;
			bool reflected = false;
			if (reflectV > 0.0) {
				if (renderer->castRay(ReflectRay, cTemp, depth + 1)) {
					reflectColor = cTemp;
					reflected = true;
				}
			}

			ofColor lambertCalculation = diffuse * I * glm::max(0.0f, glm::dot(normal, L));
			opoint = point;
			onormal = normal;
			if (!inShadow(shadRay, point, dist)) {
				if (reflected) {
					lambertCalculation += reflectV * reflectColor;
				}
				
				if (dynamic_cast<SpotLight*>(lights[i]) != nullptr) {
					SpotLight *spotLight = (SpotLight*)lights[i];
					glm::vec3 dir = spotLight->direction;
					glm::vec4 rdd = spotLight->getRotateMatrix() * glm::vec4(dir.x, dir.y, dir.z, 1.0f);
					float SpotFactor = glm::dot(lightToPixel, glm::normalize(glm::vec3(rdd.x, rdd.y, rdd.z)));

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

ofColor Shader::phong(const glm::vec3 &point, const glm::vec3 &normal, const glm::vec3 camPos, const ofColor diffuse, const ofColor specular, float power) {
	float ambientCo = 0.05;
	ofColor phongColor = diffuse * ambientCo;
	for (int i = 0; i < lights.size(); i++) {
		glm::vec3 l = glm::normalize(lights[i]->position - point);
		glm::vec3 lightToPixel = glm::normalize(point - lights[i]->position);
		//float SpotFactor = glm::dot(lightToPixel, lights[i]->direction);
		float distance = glm::distance(lights[i]->position, point);
		glm::vec3 v = -glm::normalize(point - camPos);
		glm::vec3 h = glm::normalize(v + l);
		Ray shadRay = Ray(glm::vec3(point.x, point.y, point.z + 0.001f), l);;
		//if (normal == glm::vec3(0, 1, 0))
			//shadRay = Ray(glm::vec3(point.x, point.y + .01f, point.z), l);


		//call to calculate the falloff factor for the spotlight
		//float falloff = lights[i]->falloff(SpotFactor);

		ofColor phongCalculation = diffuse * (lights[i]->intensity / glm::pow(distance, 2)) * glm::max(0.0f, glm::dot(normal, l)) +
			specular * (lights[i]->intensity / glm::pow(distance, 2)) * (glm::pow(glm::max(0.0f, glm::dot(normal, h)), power));

		//if (!inShadow(shadRay)) {
			phongColor += phongCalculation;
			//phongColor += phongCalculation * falloff;
		//}
	}
	return phongColor;
}

bool Shader::inShadow(const Ray &r, glm::vec3 hitPoint, float lightDistance) {
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
		//if (dynamic_cast<Sphere*>(objects[index]) != nullptr) {
		//	Sphere *sphereSelected = (Sphere*)objects[index];
		//	//sphereSelected->points.push_back(opoint);
		//	//sphereSelected->normals.push_back((opoint + onormal / 2));
		//}
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


Ray Shader::reflect(glm::vec3 point, glm::vec3 viewRay, glm::vec3 normal) {
	glm::vec3 reflectDir = 2 * glm::dot(normal, viewRay) * normal - viewRay;
	return Ray(point + normal * 0.001f, reflectDir);
}