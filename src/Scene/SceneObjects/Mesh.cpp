#include "Mesh.h"

//--------------------------------------------------------------
void MeshObject::processData(int vertOffset, int normOffset, int texOffset, const vector<int> &vertIndices, const vector<int> &normIndices, const vector<int> &texIndices) {
	vector<glm::vec3> meshVerts;
	vector<glm::vec3> meshVertNormals;
	vector<glm::vec2> meshVertTexs;
	vector<Triangle> meshTris;

	for (int vertexIndex : vertIndices) {
		meshVerts.push_back(vertices[vertexIndex - vertOffset]);
	}

	for (int vertNormIndex : normIndices) {
		meshVertNormals.push_back(vertNormals[vertNormIndex - normOffset]);
	}

	for (int vertTexIndex : texIndices) {
		meshVertTexs.push_back(vertTextures[vertTexIndex - texOffset]);
	}

	for (int i = 0; i < meshVerts.size(); i += 3) {
		Triangle tri;
		tri.i = i;
		tri.in = i;
		tri.it = i;
		tri.j = i + 1;
		tri.jn = i + 1;
		tri.jt = i + 1;
		tri.k = i + 2;
		tri.kn = i + 2;
		tri.kt = i + 2;
		meshTris.push_back(tri);
	}

	vertices = meshVerts;
	vertNormals = meshVertNormals;
	vertTextures = meshVertTexs;
	tris = meshTris;
}


//--------------------------------------------------------------
Mesh::Mesh(const glm::vec3 &position, const vector<MeshObject *> &objs, const vector<MeshTexture *> &textures, const string &name, const ofColor &diffuse) {
	this->position = position;
	this->meshObjects = objs;
	objName = name;
	objMaterial.setDiffuse(diffuse);
	createMesh(textures);
	setBounds();
}

//--------------------------------------------------------------
void Mesh::setBounds() {
	applyMatrix();
	glm::vec3 min = glm::vec3(100, 100, -100);
	glm::vec3 max = glm::vec3(-100, -100, 100);
	for (MeshObject *object : meshObjects) {
		for (int i = 0; i < object->vertices.size(); i++) {
			glm::vec3 vertex = object->vertices[i];
			min.x = glm::min(vertex.x, min.x);
			min.y = glm::min(vertex.y, min.y);
			min.z = glm::max(vertex.z, min.z);

			max.x = glm::max(vertex.x, max.x);
			max.y = glm::max(vertex.y, max.y);
			max.z = glm::min(vertex.z, max.z);
		}
	}
	box.setParameters(min, max);
	box.transformBox(Transform);
}


//--------------------------------------------------------------
void Mesh::draw() {
	applyMatrix();

	// check if object is selected
	if (isSelected) {
		ofDisableLighting();
		glLineWidth(0.1f);

		// Draw selection outline
		ofSetColor(SELECTED_COLOR);
		ofNoFill();
		ofPushMatrix();
		ofMultMatrix(Transform);
		for (ofMesh *mesh : ofMeshes) {
			mesh->drawWireframe();
		}
		ofPopMatrix();
		ofFill();

		glLineWidth(1.0f);
		ofEnableLighting();
	}

	// Draw 3D solid cube
	ofSetColor(ofColor::white);
	sceneMaterial.begin();
	sceneMaterial.setDiffuseColor(objMaterial.getDiffuse());
	ofPushMatrix();
	ofMultMatrix(Transform);
	for (ofMesh *mesh : ofMeshes) {
		mesh->drawFaces();
	}
	ofPopMatrix();
	sceneMaterial.end();
}


//--------------------------------------------------------------
bool Mesh::intersect(const Ray &ray, HitInfo &hitInfo) {
	// Apply Transformation
	glm::vec3 rayOriginLocal = glm::inverse(Transform) * glm::vec4(ray.p, 1.0f);
	glm::vec3 rayDirLocal = glm::normalize(glm::inverse(Transform) * glm::vec4(ray.d, 0.0f));
	float nearestDist = FLT_MAX;

	for (MeshObject *o : meshObjects) {
		for (Triangle tri : o->tris) {
			glm::vec2 bary;
			float temp;
			if (glm::intersectRayTriangle(rayOriginLocal, rayDirLocal, o->vertices[tri.i], o->vertices[tri.j], o->vertices[tri.k], bary, temp)) {
				//get the vertex normals from the mesh vector
				glm::vec3 na = o->vertNormals[tri.in];
				glm::vec3 nb = o->vertNormals[tri.jn];
				glm::vec3 nc = o->vertNormals[tri.kn];

				//Use barycentric coordinates and vertex normals to interpolate the normal
				glm::vec3 normal = glm::normalize((1 - bary.x - bary.y) * na + bary.x * nb + bary.y * nc);

				//Use barycentric coordinates to find the point on the triangle
				glm::vec3 point = o->vertices[tri.i] + bary.x * (o->vertices[tri.j] - o->vertices[tri.i]) + bary.y * (o->vertices[tri.k] - o->vertices[tri.i]);

				float dist = glm::distance(rayOriginLocal, point);
				if (dist < nearestDist) {
					nearestDist = dist;
					hitInfo.hit = true;
					hitInfo.point = point;
					hitInfo.normal = normal;
					glm::vec2 barySelected = glm::vec2(bary.x, bary.y);
					if (o->meshTex.hasTexture) {
						hitInfo.surfaceColor = objTexture.getMeshTextureColor(getMeshUV(hitInfo.point, o->vertTextures[tri.it], o->vertTextures[tri.jt], o->vertTextures[tri.kt], barySelected), o->meshTex.texture);
					}
					else {
						hitInfo.surfaceColor = objMaterial.getDiffuse();
					}
				}
			}
		}
	}

	if (hitInfo.hit) {
		hitInfo.point = Transform * glm::vec4(hitInfo.point, 1.0f);
		hitInfo.normal = glm::normalize(getRotateMatrix() * glm::vec4(hitInfo.normal, 0.0f));
	}

	return hitInfo.hit;
}


//--------------------------------------------------------------
// Triangle Signed Distance Function modified from Inigo Quilez's version 
// Source: https://www.iquilezles.org/www/articles/distfunctions/distfunctions.htm
float Mesh::sdf(const glm::vec3 &point) {
	glm::vec3 localPoint = glm::inverse(Transform) * glm::vec4(point, 1.0f);
	float dist = FLT_MAX;

	for (MeshObject *o : meshObjects) {
		for (Triangle triangle : o->tris) {
			glm::vec3 a = o->vertices[triangle.i];
			glm::vec3 b = o->vertices[triangle.j];
			glm::vec3 c = o->vertices[triangle.k];
			glm::vec3 ba = b - a; glm::vec3 pa = localPoint - a;
			glm::vec3 cb = c - b; glm::vec3 pb = localPoint - b;
			glm::vec3 ac = a - c; glm::vec3 pc = localPoint - c;
			glm::vec3 nor = glm::cross(ba, ac);

			float triDist = sqrt(
				(static_cast<float>(glm::sign(glm::dot(glm::cross(ba, nor), pa))) +
					static_cast<float>(glm::sign(glm::dot(glm::cross(cb, nor), pb))) +
					static_cast<float>(glm::sign(glm::dot(glm::cross(ac, nor), pc))) < 2.0f)
				?
				glm::min(glm::min(
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


//--------------------------------------------------------------
glm::vec2 Mesh::getMeshUV(const glm::vec3 &point, const glm::vec2 &t1, const glm::vec2 &t2, const glm::vec2 &t3, const glm::vec2 &bary) const {
	glm::vec2 uv = (t2 - t1) * bary.x + (t3 - t1) * bary.y + t1;
	float u = uv.x;
	float v = 1.0f - uv.y;

	return glm::vec2(glm::abs(u), glm::abs(v));
}


//--------------------------------------------------------------
void Mesh::createMesh(const vector<MeshTexture *> &textures) {
	unordered_map<string, MeshTexture > textureMap;
	for (MeshTexture *texture : textures) {
		textureMap[texture->name] = *texture;
	}

	for (MeshObject *object : meshObjects) {
		if (textureMap.find(object->mtlName) != textureMap.end()) {
			object->meshTex = textureMap[object->mtlName];
			if (object->meshTex.hasTexture) {
				string extension = object->meshTex.path.substr(object->meshTex.path.find_last_of(".") + 1);
				if (extension == "jpg" || extension == "png" || extension == "PNG" || extension == "tga" || extension == "TGA") {
					object->meshTex.texture.load(object->meshTex.path);
				}
				else {
					object->meshTex.hasTexture = false;
				}
			}
		}

		ofMesh *mesh = new ofMesh();
		for (Triangle tri : object->tris) {
			mesh->addIndex(tri.i);
			mesh->addIndex(tri.j);
			mesh->addIndex(tri.k);
		}
		mesh->addVertices(object->vertices);
		mesh->addNormals(object->vertNormals);
		ofMeshes.push_back(mesh);
	}

	// Display diagnostic information
	int verts = 0;
	int tris = 0;
	for (MeshObject *object : meshObjects) {
		verts += object->vertices.size();
		tris += object->tris.size();
	}

	cout << "Number of Vertices: " << verts << endl;
	cout << "Number of Faces: " << tris << endl;
}