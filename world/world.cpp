#include "world.h"

#include "../math/perlin.h"
#include <iostream>
#include <fstream>
#include <map>
#include <cmath>
#include <sstream>
#include <future>
#include <thread>
#include "../util/customrandom.h"
#include "../util/texturedatabase.h"

static constexpr int tileSize = 32;
static constexpr size_t oceanBegin = 30;

static const std::array<std::array<Biome, 6>, 6> biomes {{
	{ Biome::ICE, Biome::TUNDRA, Biome::GRASSLAND, Biome::DESERT, Biome::DESERT, Biome::DESERT },
	{ Biome::ICE, Biome::TUNDRA, Biome::GRASSLAND, Biome::DESERT, Biome::DESERT, Biome::DESERT },
	{ Biome::ICE, Biome::TUNDRA, Biome::LIGHT_FOREST, Biome::WOODLAND, Biome::SAVANNA, Biome::SAVANNA },
	{ Biome::ICE, Biome::TUNDRA, Biome::WOODLAND, Biome::WOODLAND, Biome::WOODLAND, Biome::WOODLAND },
	{ Biome::ICE, Biome::TUNDRA, Biome::WOODLAND, Biome::WOODLAND, Biome::WOODLAND, Biome::WOODLAND },
}};
static size_t GetHeight(float continentalness) {
	static const std::map<float, size_t> heights = { 
		{-1.0f, 0}, 
		{-0.7f, 0},
		{-0.6f, 40},
		{-0.4f, 50},
		{-0.3f, 70},
		{0.5f, 73},
		{1.0f, 150}
	};
	std::pair<float, size_t> last, next;

	for (auto val: heights) {
		if (continentalness == val.first) return val.second;

		if (continentalness > val.first) last = val;
		else if (continentalness < val.first) {
			next = val;
			break;
		}
	}

	float toLerp = std::clamp((continentalness - last.first), 0.0f, 1.0f);
	size_t ret = (size_t)std::lerp(last.second, next.second, toLerp);
	return ret;
}

static Biome GetBiomeType(size_t seed, float x, float z) {
	FastNoiseLite humidityNoise, temperatureNoise;
	humidityNoise.SetNoiseType(FastNoiseLite::NoiseType_Cellular);
	temperatureNoise.SetNoiseType(FastNoiseLite::NoiseType_Cellular);

	humidityNoise.SetSeed(seed >> 1);
	temperatureNoise.SetSeed(seed >> 2);

	float humidity = ((size_t)((humidityNoise.GetNoise(x, z) + 1) * 100) % 6);
	float temperature = ((size_t)((temperatureNoise.GetNoise(x, z) + 1) * 100) % 6);

	return biomes[humidity][temperature];
}

template<typename T>
static T clamp(T val, T min, T max) {
	return (val < min ? min : 
			(val > max ? max : val));
}

World::World(size_t x, size_t y)
	:x(x), y(y) {
	seed = Random::GenerateRand<size_t>(16, 124412);

	world.resize(y);
	for (auto &row : world) {
		row.resize(x);
	}
}

Voxel &World::BlockAt(size_t _x, size_t _y, size_t _z) {
	if (_x >= x * 16) throw new std::out_of_range("x out of range");
	if (_y >= 256) throw new std::out_of_range("y out of range");
	if (_z >= y * 16) throw new std::out_of_range("z out of range");

	size_t chunkX = _x / 16;
	size_t chunkZ = _z / 16;

	size_t blockX = _x % 16;
	size_t blockZ = _z % 16;

	return (world[chunkX][chunkZ].voxels[_y][blockX][blockZ]);
}

void World::Generate() {
	FastNoiseLite noise;
	noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2S);
	noise.SetSeed(seed);

	//Generates the biome map
	for (size_t chunkY = 0; chunkY < y; ++chunkY)
		for (size_t chunkX = 0; chunkX < x; ++chunkX)
			world[chunkY][chunkX].biomeType = GetBiomeType(seed, chunkX, chunkY);

	for (size_t chunkZ = 0; chunkZ < y; ++chunkZ) {
		for (size_t chunkX = 0; chunkX < x; ++chunkX) {
			Chunk &chunk = world[chunkX][chunkZ];

			for (size_t tmpZ = 0; tmpZ < 16; ++tmpZ) {
				for (size_t tmpX = 0; tmpX < 16; ++tmpX) {
					size_t posX = (chunkX * 16 + tmpX);
					size_t posZ = (chunkZ * 16 + tmpZ);
					float continentalness = noise.GetNoise((float)posX, (float)posZ);
					size_t height = GetHeight(continentalness);

					chunk.continentalness = continentalness;
					chunk.voxels[height][tmpX][tmpZ] = { 1, 0 };
					SetVoxelData(chunk.voxels[height][tmpX][tmpZ], VoxelDataType::IS_DISCOVERED);
					
					if(Random::GenerateRand<int>(0, 1000) > 900)
						npcs.emplace_back("Temp", 100, sf::Vector3i(posX, height, posZ));

					for (size_t i = height + 1; i < chunk.voxels.size(); ++i) {
						chunk.voxels[i][tmpX][tmpZ] = { 0, 0 };
						SetVoxelData(chunk.voxels[i][tmpX][tmpZ], VoxelDataType::IS_DISCOVERED);
					}

					for (size_t i = 0; i < height && i < chunk.voxels.size(); ++i) {
						chunk.voxels[i][tmpX][tmpZ] = { 2, 0 };
						SetVoxelData(chunk.voxels[i][tmpX][tmpZ], VoxelDataType::IS_DISCOVERED);
					}
				}
			}
		}
	}
}
void World::Save() {
	std::ofstream saveFile("save.obj");
	if (!saveFile.is_open()) return;

	saveFile << "o World\n";
	for (size_t chunkY = 0; chunkY < 16; ++chunkY) {
		for (size_t chunkX = 0; chunkX < 16; ++chunkX) {
			for (size_t tmpY = 0; tmpY < world[chunkY][chunkX].voxels.size(); ++tmpY) {
				for (size_t tmpZ = 0; tmpZ < 16; ++tmpZ) {
					for (size_t tmpX = 0; tmpX < 16; ++tmpX) {
						size_t posX = (chunkX * 16) + tmpX;
						size_t posY = (chunkY * 16) + tmpZ;
						const Voxel &voxel = world[chunkY][chunkX].voxels[tmpY][tmpX][tmpZ];

						float coordX = posX;
						float coordY = tmpY;
						float coordZ = posY;

						if (voxel.id != 0) {
							saveFile << "v " << coordX << " " << coordY << " " << coordZ << "\n";
						}
					}
				}
			}
		}
	}

	saveFile.close();
}

void World::Draw(sf::RenderWindow &window, size_t height, const Vector2 &topLeft, const Vector2 &bottomRight) {
	std::vector<int> tiles;

	for (size_t i = 0; i <= (bottomRight.y - topLeft.y) / tileSize; ++i) {
		for (size_t j = 0; j < (bottomRight.x - topLeft.x) / tileSize; ++j) {
			Voxel &voxel = BlockAt(j + topLeft.x, height, i + topLeft.y);

			if (!GetVoxelData(voxel, VoxelDataType::IS_DISCOVERED))
				tiles.emplace_back(4);
			else
				tiles.emplace_back(voxel.id);
		}
	}

	tileset.Load("data/textures/tileset.png", 
				 sf::Vector2u(tileSize, tileSize), 
				 tiles, 
				 ceil(window.getSize().x / tileSize), 
				 ceil(window.getSize().y / tileSize));
	window.draw(tileset);

	//Dimensions in which sizes to render
	size_t fromX = (size_t)ceilf((float)topLeft.x / tileSize);
	size_t toX = (size_t)ceilf((float)bottomRight.x / tileSize);
	size_t fromZ = (size_t)ceilf((float)topLeft.y / tileSize);
	size_t toZ = (size_t)ceilf((float)bottomRight.y / tileSize);

	for (const auto &npc : npcs) {
		sf::Vector3i npcPos = npc.GetPos();
		if (npcPos.y != height) continue;
		if (npcPos.x < topLeft.x || npcPos.x > (bottomRight.x / 32 + topLeft.x)) continue;
		if (npcPos.z < topLeft.y || npcPos.z >= (bottomRight.y / 32 + topLeft.y)) continue;

		sf::Sprite tmpSprite(TextureDB::GetTexture("data/textures/dwarfMale.png"));
		tmpSprite.setPosition((npcPos.x - topLeft.x) * tileSize, (npcPos.z - topLeft.y) * tileSize);

		window.draw(tmpSprite);
	}
}