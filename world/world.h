#pragma once

#include <vector>
#include <array>
#include <SFML/Graphics.hpp>

#include "../math/vector2.h"
#include "../block/block.h"
#include "../graphics/tileset.h"
#include "../entity/entity.h"

enum class VoxelDataType: uint16_t {
	IS_DISCOVERED = (1 << 0)
};
struct Voxel {
	uint16_t id;
	uint16_t data;
};
inline bool GetVoxelData(const Voxel &vox, VoxelDataType type) { return vox.data & (uint16_t)type; }
inline void SetVoxelData(Voxel &vox, VoxelDataType type) { vox.data |= (uint16_t)type; }
inline void UnsetVoxelData(Voxel &vox, VoxelDataType type) { vox.data &= ~(uint16_t)type; }

enum class Biome: size_t {
	ICE = 0,
	TUNDRA,
	GRASSLAND,
	LIGHT_FOREST,
	DESERT,
	WOODLAND,
	SAVANNA
};
struct Chunk {
	// [y][x][z]
	std::array<std::array<std::array<Voxel, 16>, 16>, 256> voxels;
	Biome biomeType;
	float continentalness;
};

class World {
	private:
	size_t x, y;
	std::vector<std::vector<Chunk>> world;
	std::vector<Entity> npcs;

	size_t seed = 0;
	Tileset tileset;

	Voxel &BlockAt(size_t _x, size_t _y, size_t _z);

	public:
	World(size_t x = 10, size_t y = 10);

	void Generate();
	void Save();
	void Draw(sf::RenderWindow &window, size_t height, const Vector2 &topLeft, const Vector2 &bottomRight);

	sf::Vector2<size_t> GetDims() const { return sf::Vector2<size_t>(x, y); }
};