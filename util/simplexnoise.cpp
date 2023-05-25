#include "SimplexNoise.h"
#include "customrandom.h"

SimplexNoiseOctave::SimplexNoiseOctave (size_t seed)
	:seed(seed) {
	for (auto &itm: permutations) {
		itm = (char)Random::GenerateRand<short>(0, 255);
	}
}

SimplexNoise::SimplexNoise(long largestFeature, double persistence, size_t seed)
	:largestFeature(largestFeature), persistence(persistence), seed(seed) {
	engine = std::mt19937_64(seed);
	db = std::uniform_real_distribution<float>(0, 1);
}

float SimplexNoise::GetNoise(size_t x, size_t y) {
	float res = 0;

	return res;
}