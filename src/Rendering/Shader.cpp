#include "Shader.h"
#include "RayTracer.h"
#include "RayMarcher.h"

Shader::Shader(Renderer *renderer, vector<Light *> lights, vector<SceneObject *> objects) {
	this->lights = lights;
	this->objects = objects;
	this->renderer = renderer;
}

ofColor Shader::lambert(Ray &ray, const glm::vec3 &point, const glm::vec3 &normal, const ofColor diffuse, float reflect, int depth) {
	float ambientCo = 0.08;
	ofColor lambertColor = diffuse * ambientCo;
	for (int i = 0; i < lights.size(); i++) {
		glm::vec3 L, lightToPixel;

		if (dynamic_cast<AreaLight*>(lights[i]) != nullptr) {
			AreaLight *areaLight = (AreaLight*)lights[i];
			//TODO: Refactor this into its own function in AreaLight class returning ofColor
			for (int u = 0; u < areaLight->usteps; u++) {
				for (int v = 0; v < areaLight->vsteps; v++) {
					glm::vec3 sample = areaLight->pointOnLight(u, v);
					L = glm::normalize(sample - point);
					float dist = glm::distance(sample, point);
					Ray shadRay = Ray(point + normal * 0.0001f, L);

					ofColor lambertCalculation = diffuse * ((lights[i]->intensity/ areaLight->samples)/ (4 * PI * dist)) * glm::max(0.0f, glm::dot(normal, L));
					if (!inShadow(shadRay)) {
						lambertColor += lambertCalculation;
					}
				}
			}
		}
		else {
			L = glm::normalize(lights[i]->position - point);
			lightToPixel = glm::normalize(point - lights[i]->position);
			float dist = glm::distance(lights[i]->position, point);
			Ray shadRay = Ray(point + normal * 0.0001f, L);
			float I = (lights[i]->intensity / (4 * PI * dist));
			glm::vec3 viewRay = -ray.d;
			glm::vec3 reflectDir = 2 * glm::dot(normal, viewRay) * normal - viewRay;

			Ray ReflectRay = Ray(point + normal * 0.0001f, reflectDir);
			ofColor reflectColor, cTemp;
			//if (rayT->castRay(ReflectRay, cTemp, depth + 1)) {
				reflectColor = cTemp;
			//}
			//else {
				reflectColor = 0;
			//}

			ofColor lambertCalculation = diffuse * I * glm::max(0.0f, glm::dot(normal, L)) + reflect * reflectColor;
			opoint = point;
			onormal = normal;
			if (!inShadow(shadRay)) {
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

bool Shader::inShadow(const Ray &r) {
	bool blocked = false;
	for (int index = 0; index < objects.size(); index++) {
		glm::vec3 point;
		glm::vec3 normal;

		//calculate shadows using rayMarch()
		//if (rayMarch(r, point)) {
		//	blocked = true;
		//}
		//if (dynamic_cast<Plane*>(objects[index]) != nullptr) {
		//	Plane *sphereSelected = (Plane*)objects[index];
		//	sphereSelected->rays.push_back(r.d);
		//	//sphereSelected->points.push_back(opoint);
		//	//sphereSelected->normals.push_back((opoint + onormal / 2));
		//}
		if (objects[index]->intersect(r, point, normal)) {
			blocked = true; 
			
			//if (dynamic_cast<Cylinder*>(objects[index]) != nullptr) {
			//	Cylinder *sphereSelected = (Cylinder*)objects[index];
			//	sphereSelected->points.push_back(point);
			//	sphereSelected->normals.push_back((point + normal / 2));
			//}
		}
	
	}
	return blocked;
}
