#pragma once

#include "customrandom.h"
#include <random>
#include <vector>
#include <array>

struct Grad {
	float x, y, z;

	Grad(float x = 0, float y = 0, float z = 0): x(x), y(y), z(z) {}
};

class SimplexNoiseOctave {
	private:
	size_t seed;
	static constexpr size_t swaps = 400;
	std::array<Grad, 12> grad3 = {{
		Grad(1,1,0), Grad(-1,1,0), Grad(1,-1,0), Grad(-1,-1,0),
		Grad(1,0,1), Grad(-1,0,1), Grad(1,0,-1), Grad(-1,0,-1),
		Grad(0,1,1), Grad(0,-1,1), Grad(0,1,-1), Grad(0,-1,-1)
	}};
	std::array<char, 256> permutations;

	public:
	SimplexNoiseOctave(size_t seed);
};

class SimplexNoise {
	private:
	std::mt19937_64 engine;
	std::uniform_real_distribution<float> db;
	size_t seed;
	long largestFeature;
	double persistence;

	std::vector<float> freq, amplitudes;
	std::vector<SimplexNoiseOctave> octaves;

	public:
	SimplexNoise(long largestFeature, double persistence, size_t seed = 0);

	float GetNoise(size_t x, size_t y);
};