#include "Texture.h"

ofColor Texture::getTextureColor(glm::vec2 uv, ofColor diffuse) {
	if (hasTexture) {
		int texHeight = texture.getHeight();
		int texWidth = texture.getWidth();

		int x = fmod(uv.x * texWidth * uvTileFactor, texWidth);
		int y = fmod(uv.y * texHeight * uvTileFactor, texHeight);
		if (x > texWidth || x < -texWidth || y > texHeight || y < -texHeight) {
			return diffuse;
		}

		return texture.getColor(x, y);
	}
	
	return diffuse;
}

ofColor Texture::getMeshTextureColor(glm::vec2 uv, ofImage &tex) {
	int texHeight = tex.getHeight();
	int texWidth = tex.getWidth();

	int x = fmod(uv.x * texWidth, texWidth);
	int y = fmod(uv.y * texHeight, texHeight);

	return tex.getColor(x, y);
}

void Texture::addTexture(string path) {
	string extension = path.substr(path.find_last_of(".") + 1);
	if(extension == "jpg" || extension == "png") {
		texture.load(path);
		hasTexture = true;
	}
}


void Texture::removeTexture() {
	texture.clear();
	hasTexture = false;
}

