#include "Plane.h"

Plane::Plane() {
	normal = glm::vec3(0, 1, 0);
	plane.rotateDeg(90, 1, 0, 0);
	setBounds();
}

Plane::Plane(glm::vec3 p, glm::vec3 n, string name, ofColor diffuse, float w, float h) {
	position = p;
	normal = n;
	width = w;
	height = h;
	objName = name;
	objMaterial.diffuseColor = diffuse;
	if (normal == glm::vec3(0, 1, 0)) {
		plane.rotateDeg(90, 1, 0, 0);
	}
	box = new Box();
	applyMatrix();
	setBounds();
}

void Plane::setBounds() {
	min = glm::vec4(-width/2, 0, height/2, 1.0);
	max = glm::vec4(width/2, 0, -height/2, 1.0);
	box->setParameters(min, max);
	box->transformBox(Transform);
}

// Similar to the cube intersect function
bool Plane::intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal, ofColor &surfaceColor) {
	glm::vec3 rdd, roo, invdir, sign, t, tMinV, tMaxV, tMin, tMax;

	glm::vec4 p = glm::inverse(Transform) * glm::vec4(ray.p.x, ray.p.y, ray.p.z, 1.0);
	glm::vec4 p1 = glm::inverse(Transform) * glm::vec4(ray.p + ray.d, 1.0);
	roo = glm::vec4(p.x, p.y, p.z, 1.0);
	rdd = glm::normalize(p1 - p);
	Ray r = Ray(roo, rdd);


	// Calculate intersection
	invdir = 1.0f / rdd;
	sign = glm::vec3((rdd.x < 0.0) ? 1.0 : -1.0, (rdd.y < 0.0) ? 1.0 : -1.0, (rdd.z < 0.0) ? 1.0 : -1.0);
	t = sign * glm::vec3(glm::abs(max.x - min.x) / 2, glm::abs(max.y - min.y) / 2, glm::abs(max.z - min.z) / 2);

	tMinV = glm::vec3(-roo.x + t.x, -roo.y + t.y, -roo.z + t.z);
	tMaxV = glm::vec3(-roo.x - t.x, -roo.y - t.y, -roo.z - t.z);
	tMin = invdir * tMinV;
	tMax = invdir * tMaxV;

	// Calculate entering and exiting points
	float tN = glm::max(glm::max(tMin.x, tMin.y), tMin.z);
	float tF = glm::min(glm::min(tMax.x, tMax.y), tMax.z);

	// Cube distance
	glm::vec3 d = abs(roo) - glm::vec3(glm::abs(max.x - min.x) / 2, glm::abs(max.y - min.y) / 2, glm::abs(max.z - min.z) / 2);
	float cubeDist = MIN(MAX(d.x, glm::max(d.y, d.z)), 0.0) + length(glm::max(d, 0.0f));

	// No intersection
	if (tN > tF || tF < 0.0 || isnan(tF) || tN < cubeDist)
		return false;

	// Point
	point = r.evalPoint(tN);
	surfaceColor = objTexture.getTextureColor(getUV(point), objMaterial.diffuseColor);

	point = Transform * glm::vec4(point.x, point.y, point.z, 1.0);
	// Normal
	if (tMin.x > tMin.y && tMin.x > tMin.z)
		normal = glm::vec3(Transform[0].x * sign.x, Transform[0].y*sign.x, Transform[0].z*sign.x);
	else if (tMin.y > tMin.z)
		normal = glm::vec3(Transform[1].x * sign.y, Transform[1].y*sign.y, Transform[1].z*sign.y);
	else
		normal = glm::vec3(Transform[2].x * sign.z, Transform[2].y*sign.z, Transform[2].z*sign.z);


	return true;
}

void Plane::draw() {
	applyMatrix();
	ofDisableLighting();
	for (int i = 0; i < points.size(); i++) {
		ofSetColor(ofColor::red);
		ofDrawSphere(points[i], 0.015);
		ofSetColor(ofColor::yellow);
		//ofDrawLine(points[i], normals[i]);
	}


	ofEnableLighting();
	plane.setPosition(glm::vec3(0, 0, 0));
	plane.setWidth(width);
	plane.setHeight(height);
	plane.setResolution(4, 4);
	plane.setOrientation(glm::vec3(270, 0, 0));

	if (isSelected) {
		ofDisableLighting();
		ofSetColor(ofColor::yellow);
		ofPushMatrix();
			ofMultMatrix(Transform);
			ofDrawAxis(1.0f);
			plane.drawWireframe();
		ofPopMatrix();
		ofEnableLighting();
	}
	ofSetColor(ofColor::white);
	sceneMaterial.begin();
	sceneMaterial.setDiffuseColor(objMaterial.diffuseColor);
	ofPushMatrix();
		ofMultMatrix(Transform);
		plane.drawFaces();
	ofPopMatrix();
	sceneMaterial.end();
}

// sdf modified from Inigo Quilez's version found in https://www.iquilezles.org/www/articles/distfunctions/distfunctions.htm
float Plane::sdf(const glm::vec3 p1) {
	glm::vec4 p = glm::inverse(Transform) * glm::vec4(p1.x, p1.y, p1.z, 1.0);
	return p.y;
}

glm::vec3 Plane::getNormal(const glm::vec3 &p) {
	return this->normal;
}

glm::vec2 Plane::getUV(glm::vec3 p) {
	glm::vec4 pp = glm::vec4(p.x, p.y, p.z, 1.0) * getTranslateMatrix();
	glm::vec3 hit = glm::vec4(pp.x, pp.y, pp.z, 1.0);
	

	float u = (hit.x + width / 2) / (width);
	float v = (hit.z + height / 2) / (height);

	return  glm::vec2(glm::abs(u), glm::abs(v));
}