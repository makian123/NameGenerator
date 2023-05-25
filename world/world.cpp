#include "world.h"

std::vector<std::vector<VoxelType>> soilCell = {

};

World::World(size_t w, size_t h) {
	world.resize(h);

	for (auto &line : world) {
		line.resize(w);
	}
}

void World::Generate() {

}