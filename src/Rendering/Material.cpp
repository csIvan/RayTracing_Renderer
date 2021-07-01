#include "Material.h"

Material::Material() {
	mat = MATTE;
}

void Material::setMatte() {
	roughness = 1.0f;
	refraction = 0.0f;
	ambient = 0.08f;
}
void Material::setMirror() {
	roughness = 0.0f;
	refraction = 0.0f;
	shininess = 800;
	ambient = 0.0f;
	specularCoeff = 5.0f;
}
void Material::setGlass() {
	roughness = 0.0f;
	refraction = 1.3f;
	shininess = 3000;
	ambient = 0.0f;
	specularCoeff = 4.0f;
}
void Material::setReflective() {
	roughness = 1.0f;
	refraction = 0.0f;
	shininess = 250;
	ambient = 0.08f;
	specularCoeff = 2.0f;
}

Material::Mat Material::getType() {
	if (materialString == "Matte") {
		setMatte();
		mat = MATTE;
	}
	else if (materialString == "Mirror") {
		setMirror();
		mat = MIRROR;
	}
	else if (materialString == "Glass") {
		setGlass();
		mat = GLASS;
	}
	else if (materialString == "Reflective") {
		setReflective();
		mat = REFLECTIVE;
	}

	return mat;
}