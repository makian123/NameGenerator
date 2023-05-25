#pragma once

#include <vector>
#include "voxel.h"

class World {
	private:
	size_t w, h;
	std::vector<std::vector<VoxelType>> world;

	public:
	World(size_t w = 10, size_t h = 10);

	void Generate();
};