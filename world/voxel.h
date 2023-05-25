#pragma once

enum class VoxelType: size_t {
	AIR,

	SOIL_BEGIN,
	SOIL = SOIL_BEGIN,
	SOIL_END = SOIL,

	STONE_BEGIN,
	STONE = STONE_BEGIN,
	STONE_END = STONE
};