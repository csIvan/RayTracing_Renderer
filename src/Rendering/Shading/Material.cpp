#include "Material.h"

Material::Material() {
	setMatte();
}

void Material::setMatte() {
	mat = Mat::MATTE;
	roughness = 0.8f;
	ambientCoeff = 0.1f;
}
void Material::setMirror() {
	mat = Mat::MIRROR;
	roughness = 0.0f;
	shininess = 1000;
	ambientCoeff = 0.0f;
	specularCoeff = 1.0f;
	refractionCoeff = 0.0f;
	reflectiveCoeff = 1.0f;
}
void Material::setGlass() {
	mat = Mat::GLASS;
	roughness = 0.0f;
	shininess = 3000;
	ambientCoeff = 0.0f;
	specularCoeff = 0.9f;
	refractionCoeff = 1.3f;
	reflectiveCoeff = 0.04f;
}
void Material::setReflective() {
	mat = Mat::REFLECTIVE;
	roughness = 0.9f;
	shininess = 350;
	ambientCoeff = 0.08f;
	specularCoeff = 1.0f;
	refractionCoeff = 0.0f;
	reflectiveCoeff = 0.2f;
}


void Material::SetMat(const string &material) {
	materialString = material;
	if (materialString == "Matte") {
		setMatte();
	}
	else if (materialString == "Mirror") {
		setMirror();
	}
	else if (materialString == "Glass") {
		setGlass();
	}
	else if (materialString == "Reflective") {
		setReflective();
	}
}