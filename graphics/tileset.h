#pragma once
#include <SFML/Graphics.hpp>

class Tileset: public sf::Drawable, public sf::Transformable {
private:
	sf::VertexArray vertices;
	sf::Texture tileset;

public:
	Tileset():vertices(sf::Quads, 0), tileset() {}
	bool Load(const std::string &tileset, sf::Vector2u tileSize, const std::vector<int> &tiles, unsigned int width, unsigned int height);
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
};

