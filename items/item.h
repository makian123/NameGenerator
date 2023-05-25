#pragma once

#include <string>

class BaseItem {
	protected:
	int id;
	std::string name;
	std::string description;

	public:
	BaseItem(const std::string &name = "", const std::string &desc = "");

	int GetID() const { return id; }
	std::string GetName() const { return name; }
	std::string GetDescription() const { return description; }

	virtual void Use() {}
};