#include "Material.h"

Material::Material() {
	mat = MATTE;
}

void Material::setMatte() {
	roughness = 1.0f;
	refraction = 0.0f;
	reflection = 0.0f;
}
void Material::setMirror() {
	roughness = 0.0f;
	refraction = 0.2f;
	reflection = 1.0f;
	shininess = 100;
}
void Material::setGlass() {
	roughness = 0.0f;
	refraction = 1.2f;
	reflection = 0.0f;
	shininess = 700;
}
void Material::setMetal() {
	roughness = 0.5f;
	refraction = 0.0f;
	reflection = 0.5f;
	shininess = 30;
}
void Material::setCustom() {

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
	else if (materialString == "Metal") {
		setMetal();
		mat = METAL;
	}
	else if (materialString == "Custom") {
		setCustom();
		mat = CUSTOM;
	}

	return mat;
}