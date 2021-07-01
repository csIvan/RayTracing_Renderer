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
		o->meshTex = nullptr;
		for (MeshTextureMap *m : maps) {
			if (m->name == o->mtlName) {
				o->meshTex = m;
				if (o->meshTex->hasTexture) {
					string extension = o->meshTex->path.substr(o->meshTex->path.find_last_of(".") + 1);
					if (extension == "jpg" || extension == "png" || extension == "PNG" || extension == "tga" || extension == "TGA") {
						o->meshTex->tex.load(o->meshTex->path);
					}
					else {
						o->meshTex->hasTexture = false;
					}
				}
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

	}
	box = new Box();
	applyMatrix();
	glm::vec3 min = glm::vec3(100, 100, -100);
	glm::vec3 max = glm::vec3(-100, -100, 100);
	for (MeshObject *o : mObjects) {
		for (int i = 0; i < o->vertices.size(); i++) {
			glm::vec3 v = o->vertices[i];
			min.x = (v.x < min.x) ? v.x : min.x;
			min.y = (v.y < min.y) ? v.y : min.y;
			min.z = (v.z > min.z) ? v.z : min.z;

			max.x = (v.x > max.x) ? v.x : max.x;
			max.y = (v.y > max.y) ? v.y : max.y;
			max.z = (v.z < max.z) ? v.z : max.z;
		}
	}

	box->setParameters(min, max);
	setBounds();

	// Display diagnostic information
	int verts = 0;
	int tris = 0;
	for (MeshObject *o : mObjects) {
		verts += o->vertices.size();
		tris += o->tris.size();
	}
	cout << "Number of Vertices: " << verts << endl;
	cout << "Number of Faces: " << tris << endl;
}


// Mesh Bounding Box for the entire Mesh  
void Mesh::setBounds() {
	box->transformBox(Transform);
}


/**
* Mesh Intersection function. It iterates through every triangle in the mesh
* to check if the ray intersects the Triangle. If it does interset a triangle
* then it takes the barycentric coordinates and using the equation from the slides
* and the book it interpolates the normal and finds the point on the triangle based
* on the barycentric coordinates. Also, it takes the closest hit triangle, so that
* triangles behind it are not shown in front.
*/
bool Mesh::intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal, ofColor &surfaceColor) {
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
	glm::vec2 barySelected = glm::vec2(0.0, 0.0);
	glm::vec3 texCoors = glm::vec3(0, 0, 0);
	surfaceColor = objMaterial.diffuseColor;

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
					barySelected = glm::vec2(bary.x, bary.y);
					if(o->meshTex != nullptr && o->meshTex->hasTexture)
						surfaceColor = objTexture.getMeshTextureColor(getMeshUV(point, o->vertTextures[triangle.it], o->vertTextures[triangle.jt], o->vertTextures[triangle.kt], barySelected), o->meshTex->tex);
				}
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

// sdf modified from Inigo Quilez's version found in https://www.iquilezles.org/www/articles/distfunctions/distfunctions.htm
float Mesh::sdf(const glm::vec3 p1) {
	glm::vec4 pp = glm::inverse(Transform) * glm::vec4(p1.x, p1.y, p1.z, 1.0);
	glm::vec3 p = glm::vec3(pp.x, pp.y, pp.z);

	float dist = FLT_MAX;

	for (MeshObject *o : mObjects) {
		for (Triangle triangle : o->tris) {
			glm::vec3 a = o->vertices[triangle.i];
			glm::vec3 b = o->vertices[triangle.j];
			glm::vec3 c = o->vertices[triangle.k];
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
	}
	return dist;
}


glm::vec2 Mesh::getMeshUV(glm::vec3 p, glm::vec2 t1, glm::vec2 t2, glm::vec2 t3, glm::vec2 bary) {
	glm::vec2 uv = (t2 - t1) * bary.x + (t3 - t1) * bary.y + t1;
	float u = uv.x;
	float v = 1.0f - uv.y;

	return glm::vec2(glm::abs(u), glm::abs(v));
}

