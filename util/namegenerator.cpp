#include "namegenerator.h"
#include "customrandom.h"
#include <fstream>
#include <iostream>
#include <numeric>
#include <array>
#include <thread>
#include <future>

static const std::array<char, 5> vowels = { {'a', 'e', 'i', 'o', 'u'} };

char WeighedRandom(const std::vector<size_t> &weights) {
	std::vector<float> modifiedW;
	for (auto &w : weights) modifiedW.push_back((float)w);

	float weightSum = std::accumulate(modifiedW.begin(), modifiedW.end(), 0);
	for (auto &weight : modifiedW) {
		weight = (float)weight * (1 / weightSum);
	}

	float rand = Random::GenerateRand<float>(0, *std::max_element(modifiedW.begin(), modifiedW.end()));
	for (size_t i = 0; i < weights.size(); ++i) {
		if (rand <= modifiedW[i]) return i;
	}

	return '\0';
}

NameGenerator::NameGenerator(const std::string &folderPath):folderPath(folderPath), maleMatrix() {
	std::ifstream maleFile = std::ifstream(folderPath + std::string("name_male.txt"));
	if (!maleFile.is_open()) throw new std::invalid_argument("File '" + folderPath + "name_male.txt' not found");
	std::ifstream femaleFile = std::ifstream(folderPath + std::string("name_female.txt"));
	if (!femaleFile.is_open()) throw new std::invalid_argument("File '" + folderPath + "name_female.txt' not found");
	std::ifstream surnameFile = std::ifstream(folderPath + std::string("surname.txt"));
	if (!surnameFile.is_open()) throw new std::invalid_argument("File '" + folderPath + "surname.txt' not found");

	std::future<void> maleGenerator = std::async(
		std::launch::async, 
		[&]() {
			std::string tmpLine;
			size_t ctr = 0;
			while (maleFile >> tmpLine && ctr++ < 5e5) {
				if (tmpLine.length() < 3) continue;
				std::transform(tmpLine.begin(), tmpLine.end(), tmpLine.begin(),
							   [](unsigned char c) { return std::tolower(c); });

				for (size_t i = 2; i < tmpLine.size() - 1; ++i) {
					if (!maleMatrix[tmpLine.substr(i - 2, 2)].size()) maleMatrix[tmpLine.substr(i - 2, 2)] = std::vector<size_t>(256, 0);

					maleMatrix[tmpLine.substr(i - 2, 2)][tmpLine[i]]++;
				}
			}
		});

	std::future<void> femaleGenerator = std::async(
		std::launch::async,
		[&]() {
			std::string tmpLine;
			size_t ctr = 0;
			while (femaleFile >> tmpLine) {
				if (tmpLine.length() < 3 && ctr++ < 5e5) continue;
				std::transform(tmpLine.begin(), tmpLine.end(), tmpLine.begin(),
							   [](unsigned char c) { return std::tolower(c); });

				for (size_t i = 2; i < tmpLine.size() - 1; ++i) {
					if (!femaleMatrix[tmpLine.substr(i - 2, 2)].size()) femaleMatrix[tmpLine.substr(i - 2, 2)] = std::vector<size_t>(256, 0);
					femaleMatrix[tmpLine.substr(i - 2, 2)][tmpLine[i]]++;
				}
			}
		});
	
	std::future<void> surnameGenerator = std::async(
		std::launch::async,
		[&]() {
			std::string tmpLine;
			size_t ctr = 0;
			while (surnameFile >> tmpLine && ctr++ < 5e5) {
				if (tmpLine.length() < 3) continue;
				std::transform(tmpLine.begin(), tmpLine.end(), tmpLine.begin(),
							   [](unsigned char c) { return std::tolower(c); });

				for (size_t i = 2; i < tmpLine.size() - 1; ++i) {
					if (!surnameMatrix[tmpLine.substr(i - 2, 2)].size()) surnameMatrix[tmpLine.substr(i - 2, 2)] = std::vector<size_t>(256, 0);
					surnameMatrix[tmpLine.substr(i - 2, 2)][tmpLine[i]]++;
				}
			}
	});

	maleGenerator.get();
	femaleGenerator.get();
	surnameGenerator.get();

	maleFile.close();
	femaleFile.close();
	surnameFile.close();
}

std::string NameGenerator::GenerateName(bool male) {
	std::string ret = std::string(1, (char)Random::GenerateRand<short>('a', 'z'));
	ret += vowels.at(Random::GenerateRand<size_t>(0, vowels.size() - 1));
	size_t len = Random::GenerateRand<size_t>(3, 10);

	while (len--) {
		std::string lastChs = ret.substr(ret.length() - 2, 2);
		std::vector<size_t> &mostLikely = (male ? maleMatrix[lastChs] : femaleMatrix[lastChs]);

		while (!mostLikely.size()) {
			ret[ret.size() - 2] = (char)Random::GenerateRand<short>('a', 'z');
			ret[ret.size() - 1] = (char)Random::GenerateRand<short>('a', 'z');

			mostLikely = (male ? maleMatrix[ret.substr(ret.length() - 2, 2)] : femaleMatrix[ret.substr(ret.length() - 2, 2)]);
		}
		ret += WeighedRandom(mostLikely);

		if (!ret.back()) return "";
	}

	ret[0] = std::toupper(ret[0]);
	return ret;
}
std::string NameGenerator::GenerateSurname() {
	std::string ret = std::string(1, (char)Random::GenerateRand<short>('a', 'z'));
	ret += vowels.at(Random::GenerateRand<size_t>(0, vowels.size() - 1));
	size_t len = Random::GenerateRand<size_t>(3, 10);

	while (len--) {
		std::string lastChs = ret.substr(ret.length() - 2, 2);
		std::vector<size_t> &mostLikely = surnameMatrix[lastChs];

		while (!mostLikely.size()) {
			ret[ret.size() - 2] = (char)Random::GenerateRand<short>('a', 'z');
			ret[ret.size() - 1] = (char)Random::GenerateRand<short>('a', 'z');

			mostLikely = surnameMatrix[ret.substr(ret.length() - 2, 2)];
		}
		ret += WeighedRandom(mostLikely);

		if (!ret.back()) return "";
	}

	ret[0] = std::toupper(ret[0]);
	return ret;
}