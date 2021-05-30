#include "Mesh.h"

void MeshObject::processData(int vstart, int nstart, int tstart, vector<int> verti, vector<int> normi, vector<int> texi) {
	vector<glm::vec3> meshVertices;
	vector<glm::vec3> meshVerticesNormals;
	vector<glm::vec2> meshVerticesTex;
	vector<Triangle> meshTris;

	for (unsigned int i = 0; i < verti.size(); i++) {
		unsigned int vertexIndex = verti[i];
		glm::vec3 vertex = vertices[vertexIndex - vstart];
		meshVertices.push_back(vertex);
	}

	for (unsigned int i = 0; i < normi.size(); i++) {
		unsigned int vertNormIndex = normi[i];
		glm::vec3 vn = vertNormals[vertNormIndex - nstart];
		meshVerticesNormals.push_back(vn);
	}

	for (unsigned int i = 0; i < texi.size(); i++) {
		unsigned int vertTexIndex = texi[i];
		glm::vec2 vn = vertTextures[vertTexIndex - tstart];
		meshVerticesTex.push_back(vn);
	}

	int count = 0;
	vector<int> temp;
	for (unsigned int i = 0; i < meshVertices.size(); i++) {
		temp.push_back(i);
		count++;
		if (count == 3) {
			Triangle triangle;
			triangle.i = temp[0];
			triangle.in = temp[0];
			triangle.it = temp[0];
			triangle.j = temp[1];
			triangle.jn = temp[1];
			triangle.jt = temp[1];
			triangle.k = temp[2];
			triangle.kn = temp[2];
			triangle.kt = temp[2];
			meshTris.push_back(triangle);
			temp.clear();
			count = 0;
		}
	}
	vertices = meshVertices;
	vertNormals = meshVerticesNormals;
	vertTextures = meshVerticesTex;
	tris = meshTris;

}

Mesh::Mesh(glm::vec3 p, vector<MeshObject *> objs, vector<MeshTextureMap *> maps, string name, ofColor diffuse) {
	position = p;
	mObjects = objs;
	objName = name;
	objMaterial.diffuseColor = diffuse;

	// Create scene mesh
	for (MeshObject *o : mObjects) {
		for (MeshTextureMap *m : maps) {
			if (m->name == o->mtlName) {
				o->meshTex = m;
			}
		}

		ofMesh *mesh = new ofMesh();
		for (int i = 0; i < o->tris.size(); i++) {
			mesh->addIndex(o->tris[i].i);
			mesh->addIndex(o->tris[i].j);
			mesh->addIndex(o->tris[i].k);
		}
		mesh->addVertices(o->vertices);
		mesh->addNormals(o->vertNormals);
		meshes.push_back(mesh);


		cout << endl << "Mesh: " << o->mtlName << endl;
		cout << "newmtl: " << o->meshTex->name << endl;
		cout << "Kd: " << o->meshTex->kd << endl;
		cout << "Kd_map: " << o->meshTex->path << endl;
	}
}


// return a Mesh Bounding Box for the entire Mesh
//
void Mesh::setBounds() {
	//int n = mesh.getNumVertices();
	//ofVec3f v = mesh.getVertex(0);
	//ofVec3f max = v;
	//ofVec3f min = v;
	//for (int i = 1; i < n; i++) {
	//	ofVec3f v = mesh.getVertex(i);

	//	if (v.x > max.x) max.x = v.x;
	//	else if (v.x < min.x) min.x = v.x;

	//	if (v.y > max.y) max.y = v.y;
	//	else if (v.y < min.y) min.y = v.y;

	//	if (v.z > max.z) max.z = v.z;
	//	else if (v.z < min.z) min.z = v.z;
	//}
	//cout << "vertices: " << n << endl;
	////	cout << "min: " << min << "max: " << max << endl;
	//box =  Box(glm::vec3(min.x, min.y, min.z), glm::vec3(max.x, max.y, max.z));
}

// getMeshPointsInBox:  return an array of indices to points in mesh that are contained 
//                      inside the Box.  Return count of points found;
//
int Mesh::getMeshPointsInBox(const vector<int>& points, Box & box, vector<int> & pointsRtn) {
	int count = 0;
	//for (int i = 0; i < points.size(); i++) {
	//	ofVec3f v = mesh.getVertex(points[i]);
	//	if (box.inside(glm::vec3(v.x, v.y, v.z))) {
	//		count++;
	//		pointsRtn.push_back(points[i]);
	//	}
	//}
	return count;
}

/**
* Mesh Intersection function. It iterates through every triangle in the mesh
* to check if the ray intersects the Triangle. If it does interset a triangle
* then it takes the barycentric coordinates and using the equation from the slides
* and the book it interpolates the normal and finds the point on the triangle based
* on the barycentric coordinates. Also, it takes the closest hit triangle, so that
* triangles behind it are not shown in front.
*/
bool Mesh::intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal, glm::vec2 &uv) {
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
	Triangle *tri;
	objSel = nullptr;
	selectedTri = new Triangle();
	barySelected = glm::vec2(0.0, 0.0);
	glm::vec3 texCoors = glm::vec3(0, 0, 0);

	for (MeshObject *o : mObjects) {
		for (Triangle triangle : o->tris) {
			glm::vec3 na, nb, nc;
			glm::vec3 bary;
			if (glm::intersectRayTriangle(roo, rdd, o->vertices[triangle.i], o->vertices[triangle.j], o->vertices[triangle.k], bary)) {

				//get the vertex normals from the mesh vector
				na = o->vertNormals[triangle.in];
				nb = o->vertNormals[triangle.jn];
				nc = o->vertNormals[triangle.kn];

				//Use barycentric coordinates and vertex normals to interpolate the normal
				normal = glm::normalize((1 - bary.x - bary.y) * na + bary.x *nb + bary.y * nc);

				//Use barycentric coordinates to find the point on the triangle
				point = o->vertices[triangle.i] + bary.x * (o->vertices[triangle.j] - o->vertices[triangle.i])
					+ bary.y * (o->vertices[triangle.k] - o->vertices[triangle.i]);
				dist = glm::distance(roo, point);
				if (dist < maxLocalDist) {
					maxLocalDist = dist;
					insideTri = true;
					po = point;
					no = normal;
					texCoors = glm::vec3(triangle.it, triangle.jt, triangle.kt);
					//cout << selectedTri->it << ", " << selectedTri->jt << ", " << selectedTri->kt << endl;
					barySelected = glm::vec2(bary.x, bary.y);
					objSel = o;
				}
			}
		}
	}
	
	//cout << texCoors << endl;
	point = Transform * glm::vec4(po, 1.0);
	normal = glm::normalize(getRotateMatrix() * glm::vec4(no, 1.0));
	if (objSel) {
		uv = getMeshUV(point, objSel, texCoors, barySelected);
	}
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
			for (ofMesh *m : meshes) {
				m->drawWireframe();
			}
		ofPopMatrix();
		ofFill();
		ofEnableLighting();
	}

	ofSetColor(ofColor::white);
	sceneMaterial.begin();
	sceneMaterial.setDiffuseColor(objMaterial.diffuseColor);
	ofPushMatrix();
		ofMultMatrix(Transform);
		for (ofMesh *m : meshes) {
			m->drawFaces();
		}
	ofPopMatrix();
	sceneMaterial.end();
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

glm::vec2 Mesh::getUV(glm::vec3 p) {

	//glm::vec2 tC0 = vertTextures[selectedTri->it];
	//glm::vec2 tC1 = vertTextures[selectedTri->it];
	//glm::vec2 tC2 = vertTextures[selectedTri->it];

	////this is a relatively simple operation, we just remap our berycentric points to fall within the bounds of the uvw triangle
	//glm::vec2 uv = (tC1 - tC0) * barySelected.x + (tC2 - tC0) * barySelected.y + tC0;

	return  glm::vec2(1, 1);
}


glm::vec2 Mesh::getMeshUV(glm::vec3 p, MeshObject *o, glm::vec3 texCoors, glm::vec2 bary) {
	glm::vec2 tC0 = o->vertTextures[texCoors.x];
	glm::vec2 tC1 = o->vertTextures[texCoors.y];
	glm::vec2 tC2 = o->vertTextures[texCoors.z];

	//this is a relatively simple operation, we just remap our berycentric points to fall within the bounds of the uvw triangle
	glm::vec2 uv = (tC1 - tC0) * bary.x + (tC2 - tC0) * bary.y + tC0;
	//cout << uv << endl;
	//cout << tri->it << ", " << tri->jt << ", " << tri->kt << endl;
	//for (glm::vec2 v : o->vertTextures) {
	//	cout << "vt " << v << endl;
	//}
	float u = uv.x;
	float v = 1.0f - uv.y;

	return glm::vec2(glm::abs(u), glm::abs(v));
}

