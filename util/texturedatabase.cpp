#include "texturedatabase.h"
#include <stdexcept>

std::map<std::string, sf::Texture> TextureDB::textures;

sf::Texture &TextureDB::GetTexture(const std::string &path) {
	if (textures.find(path) == textures.end()) {
		sf::Texture txture;
		if (!txture.loadFromFile(path)) throw new std::exception("Cannot open the file");
		
		textures[path] = txture;
	}

	return textures[path];
}