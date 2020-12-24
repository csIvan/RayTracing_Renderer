#include "Mesh.h"

/**
* Mesh Intersection function. It iterates through every triangle in the mesh
* to check if the ray intersects the Triangle. If it does interset a triangle
* then it takes the barycentric coordinates and using the equation from the slides
* and the book it interpolates the normal and finds the point on the triangle based
* on the barycentric coordinates. Also, it takes the closest hit triangle, so that
* triangles behind it are not shown in front.
*/
bool Mesh::intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal) {
	bool insideTri = false;
	float maxLocalDist = FLT_MAX;
	float dist;
	glm::vec3 p, n;
	for (Triangle triangle : tris) {
		glm::vec3 na, nb, nc;
		glm::vec3 bary;
		if (glm::intersectRayTriangle(ray.p, ray.d, vertices[triangle.i], vertices[triangle.j], vertices[triangle.k], bary)) {
			//get the vertex normals from the mesh vector
			na = vertNormals[triangle.in];
			nb = vertNormals[triangle.jn];
			nc = vertNormals[triangle.kn];

			//Use barycentric coordinates and vertex normals to interpolate the normal
			normal = glm::normalize((1 - bary.x - bary.y) * na + bary.x *nb + bary.y * nc);

			//Use barycentric coordinates to find the point on the triangle
			point = vertices[triangle.i] + bary.x * (vertices[triangle.j] - vertices[triangle.i])
				+ bary.y * (vertices[triangle.k] - vertices[triangle.i]);
			dist = glm::distance(ray.p, point);
			if (dist < maxLocalDist) {
				maxLocalDist = dist;
				insideTri = true;
				p = point;
				n = normal;
			}
		}
	}
	point = p;
	normal = n;
	return insideTri;
}


void Mesh::draw() {
	for (Triangle tri : tris) {
		ofNoFill();
		ofDrawTriangle(vertices[tri.i], vertices[tri.j], vertices[tri.k]);
	}
}

