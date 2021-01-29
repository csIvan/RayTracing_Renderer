#include "Mesh.h"

Mesh::Mesh(glm::vec3 p, vector<Triangle> t, vector<glm::vec3> v, vector<glm::vec3> vn, string name, ofColor diffuse) {
	position = p;
	tris = t;
	vertices = v;
	vertNormals = vn;
	objName = name;
	diffuseColor = diffuse;

	// Create scene mesh
	for (int i = 0; i < tris.size(); i++) {
		mesh.addIndex(tris[i].i);
		mesh.addIndex(tris[i].j);
		mesh.addIndex(tris[i].k);
	}
	mesh.addVertices(vertices);
	mesh.addNormals(vertNormals);
}

/**
* Mesh Intersection function. It iterates through every triangle in the mesh
* to check if the ray intersects the Triangle. If it does interset a triangle
* then it takes the barycentric coordinates and using the equation from the slides
* and the book it interpolates the normal and finds the point on the triangle based
* on the barycentric coordinates. Also, it takes the closest hit triangle, so that
* triangles behind it are not shown in front.
*/
bool Mesh::intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal) {
	glm::vec3 rdd, roo;

	// Apply Transformation
	glm::vec4 p = glm::inverse(Transform) * glm::vec4(ray.p.x, ray.p.y, ray.p.z, 1.0);
	glm::vec4 p1 = glm::inverse(Transform) * glm::vec4(ray.p + ray.d, 1.0);
	roo = glm::vec4(p.x, p.y, p.z, 1.0);
	rdd = glm::normalize(p1 - p);

	bool insideTri = false;
	float maxLocalDist = FLT_MAX;
	float dist;
	glm::vec3 po, no;
	for (Triangle triangle : tris) {
		glm::vec3 na, nb, nc;
		glm::vec3 bary;
		if (glm::intersectRayTriangle(roo, rdd, vertices[triangle.i], vertices[triangle.j], vertices[triangle.k], bary)) {
			//get the vertex normals from the mesh vector
			na = vertNormals[triangle.in];
			nb = vertNormals[triangle.jn];
			nc = vertNormals[triangle.kn];

			//Use barycentric coordinates and vertex normals to interpolate the normal
			normal = glm::normalize((1 - bary.x - bary.y) * na + bary.x *nb + bary.y * nc);

			//Use barycentric coordinates to find the point on the triangle
			point = vertices[triangle.i] + bary.x * (vertices[triangle.j] - vertices[triangle.i])
				+ bary.y * (vertices[triangle.k] - vertices[triangle.i]);
			dist = glm::distance(roo, point);
			if (dist < maxLocalDist) {
				maxLocalDist = dist;
				insideTri = true;
				po = point;
				no = normal;
			}
		}
	}
	point = Transform * glm::vec4(po, 1.0);
	normal = glm::normalize(getRotateMatrix() * glm::vec4(no, 1.0));
	return insideTri;
}


void Mesh::draw() {
	applyMatrix();
	ofDisableLighting();
	for (int i = 0; i < points.size(); i++) {
		ofSetColor(ofColor::red);
		ofDrawSphere(points[i], 0.025);
		ofSetColor(ofColor::yellow);
		ofDrawLine(points[i], normals[i]);
	}


	ofEnableLighting();
	if (isSelected) {
		ofDisableLighting();
		ofSetColor(ofColor::yellow);
		ofNoFill();
		ofPushMatrix();
			ofMultMatrix(Transform);
			mesh.drawWireframe();
		ofPopMatrix();
		ofFill();
		ofEnableLighting();
	}

	ofSetColor(ofColor::white);
	material.begin();
	material.setDiffuseColor(diffuseColor);
	ofPushMatrix();
		ofMultMatrix(Transform);
		mesh.drawFaces();
	ofPopMatrix();
	material.end();
}

float Mesh::sdf(const glm::vec3 p1) {
	glm::vec4 pp = glm::inverse(Transform) * glm::vec4(p1.x, p1.y, p1.z, 1.0);
	glm::vec3 p = glm::vec3(pp.x, pp.y, pp.z);

	float dist = FLT_MAX;

	for (Triangle triangle : tris) {
		glm::vec3 a = vertices[triangle.i];
		glm::vec3 b = vertices[triangle.j];
		glm::vec3 c = vertices[triangle.k];
		glm::vec3 ba = b - a; glm::vec3 pa = p - a;
		glm::vec3 cb = c - b; glm::vec3 pb = p - b;
		glm::vec3 ac = a - c; glm::vec3 pc = p - c;
		glm::vec3 nor = glm::cross(ba, ac);

		float triDist = sqrt(
			(glm::sign(glm::dot(glm::cross(ba, nor), pa)) +
			glm::sign(glm::dot(glm::cross(cb, nor), pb)) +
			glm::sign(glm::dot(glm::cross(ac, nor), pc)) < 2.0)
			?
			min(min(
			glm::dot(ba * glm::clamp(glm::dot(ba, pa) / glm::dot(ba, ba), 0.0f, 1.0f) - pa, ba * glm::clamp(glm::dot(ba, pa) / glm::dot(ba, ba), 0.0f, 1.0f) - pa),
			glm::dot(cb * glm::clamp(glm::dot(cb, pb) / glm::dot(cb, cb), 0.0f, 1.0f) - pb, cb * glm::clamp(glm::dot(cb, pb) / glm::dot(cb, cb), 0.0f, 1.0f) - pb)),
			glm::dot(ac * glm::clamp(glm::dot(ac, pc) / glm::dot(ac, ac), 0.0f, 1.0f) - pc, ac * glm::clamp(glm::dot(ac, pc) / glm::dot(ac, ac), 0.0f, 1.0f) - pc))
			:
			glm::dot(nor, pa) * glm::dot(nor, pa) / glm::dot(nor, nor)
		);

		if (triDist < dist) {
			dist = triDist;
		}
	}
	return dist;
}
