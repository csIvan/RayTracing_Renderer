#include "Texture.h"

//--------------------------------------------------------------
ofColor Texture::getTextureColor(const glm::vec2 &uv, const ofColor &diffuse) {
	if (hasTexture) {
		int texHeight = texture.getHeight();
		int texWidth = texture.getWidth();

		int x = fmod(uv.x * texWidth * uvTileFactor, texWidth);
		int y = fmod(uv.y * texHeight * uvTileFactor, texHeight);
		if (x >= 0 && x < texWidth && y >= 0 && y < texHeight) {
			return texture.getColor(x, y);
		}
	}

	return diffuse;
}

//--------------------------------------------------------------
ofColor Texture::getMeshTextureColor(const glm::vec2 &uv, const ofImage &tex) {
	int texHeight = tex.getHeight();
	int texWidth = tex.getWidth();

	int x = fmod(uv.x * texWidth, texWidth);
	int y = fmod(uv.y * texHeight, texHeight);

	return tex.getColor(x, y);
}

//--------------------------------------------------------------
void Texture::addTexture(const string &path) {
	string extension = path.substr(path.find_last_of(".") + 1);
	if (extension == "jpg" || extension == "png") {
		texture.load(path);
		hasTexture = true;
	}
}

//--------------------------------------------------------------
void Texture::removeTexture() {
	texture.clear();
	hasTexture = false;
}