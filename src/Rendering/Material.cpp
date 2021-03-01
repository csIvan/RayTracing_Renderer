#include "Material.h"

Material::Material() {
	mat = MATTE;
}

ofColor Material::Matte() {
	return diffuseColor;
}
ofColor Material::Mirror() {
	return diffuseColor;
}
ofColor Material::Glass() {
	return diffuseColor;
}
ofColor Material::Metal() {
	return diffuseColor;
}