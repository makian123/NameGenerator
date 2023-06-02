#include "tileset.h"
#include "../util/texturedatabase.h"

bool Tileset::Load(const std::string &tileset, sf::Vector2u tileSize, const std::vector<int> &tiles, unsigned int width, unsigned int height) {
    try { this->tileset = TextureDB::GetTexture(tileset); }
    catch (...) { return false; }

    vertices.setPrimitiveType(sf::Quads);
    vertices.resize(width * height * 4);

    for (size_t i = 0; i < width; ++i) {
        for (size_t j = 0; j < height; ++j) {
            int tileNumber = tiles.at(i + j * width);

            // find its position in the tileset texture
            int tu = tileNumber % (this->tileset.getSize().x / tileSize.x);
            int tv = tileNumber / (this->tileset.getSize().x / tileSize.x);

            // get a pointer to the current tile's quad
            sf::Vertex *quad = &vertices[(i + j * width) * 4];

            // define its 4 corners
            quad[0].position = sf::Vector2f(i * tileSize.x, j * tileSize.y);
            quad[1].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
            quad[2].position = sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);
            quad[3].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);

            // define its 4 texture coordinates
            quad[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
            quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
            quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
            quad[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
        }
    }

    return true;
}

void Tileset::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    states.transform *= getTransform();

    states.texture = &tileset;

    target.draw(vertices, states);
}