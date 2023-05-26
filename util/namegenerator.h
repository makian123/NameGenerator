#pragma once

#include <string>
#include <map>
#include <vector>

class NameGenerator {
private:
	std::string folderPath;
	std::map<std::string, std::vector<size_t>> maleMatrix;
	std::map<std::string, std::vector<size_t>> femaleMatrix;
	std::map<std::string, std::vector<size_t>> surnameMatrix;
public:
	NameGenerator(const std::string &folderPath);

	std::string GenerateName(bool male = true);
	std::string GenerateSurname();
};