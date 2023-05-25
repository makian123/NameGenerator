#pragma once

#include <string>
#include <vector>
#include <memory>
#include <array>

#include "../items/item.h"

class Entity {
	private:
	std::string name;
	float hp, maxhp;
	std::vector<std::shared_ptr<BaseItem>> inventory;
	std::array<std::shared_ptr<BaseItem>, 4> armor;
	std::shared_ptr<BaseItem> mainHand, offHand;

	public:
	Entity(const std::string &name, float hp);

	std::vector<std::shared_ptr<BaseItem>> &GetInventory() { return inventory; }
	std::shared_ptr<BaseItem> &GetMainHand() { return mainHand; }
	std::shared_ptr<BaseItem> &GetOffHand() { return offHand; }

	bool PickUp(const std::shared_ptr<BaseItem> &item);

	void TakeDamage(float dmg);

	std::string GetName() const { return name; }
	float GetHP() const { return hp; }
};