#include "namegenerator.h"
#include <fstream>
#include <vector>
#include <iostream>
#include "customrandom.h"

static std::string GeneratePhonem(
	const std::vector<std::string> &consonants,
	const std::vector<std::string> &vowels) {
	if (!consonants.size() || !vowels.size()) return "";

	std::string ret = "";

	size_t pos = Random::GenerateRand<size_t>(0, vowels.size() - 1);
	ret += consonants.at(Random::GenerateRand<size_t>(0, consonants.size() - 1));
	
	pos = Random::GenerateRand<size_t>(0, vowels.size() - 1);
	ret += vowels.at(pos);

	return ret;
}

std::string GenerateName(const std::string &path, bool male) {
	std::ifstream file = std::ifstream(path);
	if (!file.is_open()) return "";

	std::string ret = "";

	std::vector<std::string> consonants;
	std::vector<std::string> vowels;

	std::string tmpLine;
	file >> tmpLine;
	size_t pos = 0;
	while ((pos = tmpLine.find(',')) != std::string::npos) {
		vowels.push_back(tmpLine.substr(0, pos));
		tmpLine.erase(0, pos + 1);
	}
	if (tmpLine.size()) vowels.push_back(tmpLine);
	tmpLine.clear();

	file >> tmpLine;
	while ((pos = tmpLine.find(',')) != std::string::npos) {
		consonants.push_back(tmpLine.substr(0, pos));
		tmpLine.erase(0, pos + 1);
	}
	if (tmpLine.size()) consonants.push_back(tmpLine);

	size_t phonemes = Random::GenerateRand<size_t>(1, 5);
	while(phonemes--)
		std::cout << GeneratePhonem(consonants, vowels);
	std::cout << " ";
	phonemes = Random::GenerateRand<size_t>(2, 4);
	while (phonemes--)
		std::cout << GeneratePhonem(consonants, vowels);

	return ret;
}