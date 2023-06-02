#pragma once

#include <string>
#include <map>
#include <SFML/Graphics.hpp>

class TextureDB {
private:
	static std::map<std::string, sf::Texture> textures;
public:
	static sf::Texture &GetTexture(const std::string &path);
};