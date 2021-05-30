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
	setBounds();
}

void Plane::setBounds() {
	applyMatrix();
	glm::vec4 min = getTranslateMatrix() * glm::vec4(-width/2, 0, -height/2, 1.0);
	glm::vec4 max = getTranslateMatrix() * glm::vec4(width/2, 0, height/2, 1.0);
	box = Box(min, max);
}

bool Plane::intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal, glm::vec2 &uv) {
	glm::vec3 rdd, roo;
	glm::vec4 p = glm::inverse(Transform) * glm::vec4(ray.p.x, ray.p.y, ray.p.z, 1.0);
	glm::vec4 p1 = glm::inverse(Transform) * glm::vec4(ray.p + ray.d, 1.0);
	roo = glm::vec4(p.x, p.y, p.z, 1.0);
	rdd = glm::normalize(p1 - p);
	Ray r = Ray(roo, rdd);

	float dist1, dist2;
	bool insidePlane = false;
	bool hit = false;
	float d = glm::dot(rdd, this->normal);
	if (glm::abs(d) > -0.001f) {
		float t = glm::dot(-roo, this->normal) / d;
		point = r.evalPoint(t);
		normal = this->normal;
		if(t >= 0)
			hit = true;
	}

	uv = getUV(point);
	point = Transform * glm::vec4(point.x, point.y, point.z, 1.0);
	normal = glm::normalize(getRotateMatrix() * glm::vec4(normal.x, normal.y, normal.z, 1.0));
	glm::vec2 xRange = glm::vec2(position.x - width / 2, position.x + width / 2);
	glm::vec2 zRange = glm::vec2(position.z - height / 2, position.z + height / 2);
	if (hit && point.x < xRange[1] && point.x > xRange[0] && point.z < zRange[1] && point.z > zRange[0]) {
		insidePlane = true;
	}
	return insidePlane;
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

	//int x = glm::mod(u * texWidth, texWidth);
	//int y = glm::mod(v * texHeight, texHeight);

	//return  objTexture.texture.getColor(x, y);


	//int textConst = 16;
	////get projected texture dimensions
	//float projTextHeight = width / textConst;
	//float projTextWidth = height / textConst;
	////get x,y coordinates of point of intersection (add dimension/2 to get coordinates to start at corner)
	//float x = point.x + width / 2;
	//float y = point.z + height / 2;
	////get u,v coordinates on texture
	//float u = x / (projTextWidth);
	//float v = y / (projTextHeight);
	////transform u,v to i,j
	//int i = fmod(u, 1) * texWidth;
	//int j = fmod(v, 1) * texHeight;
	//return objTexture.texture.getColor(i, j);
}