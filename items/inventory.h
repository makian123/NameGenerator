#pragma once

#include <vector>
#include <memory>

#include "item.h"

class Slot {
	private:
	std::shared_ptr<BaseItem> item;
	int amt, maxAmt;

	public:
	Slot(int amt, std::shared_ptr<BaseItem> item = nullptr)
		: amt(amt), maxAmt(amt), item(item) {}

	std::shared_ptr<BaseItem> GetItem() { return item; }
	void SetItem(std::shared_ptr<BaseItem> itm) { item = itm; }

	int GetAmount() const { return amt; }
	int GetMaxAmount() const { return maxAmt; }
	void SetAmount(int amt) { this->amt = amt; }
	void SetMaxAmount(int amt) { this->maxAmt = amt; }
};

class Inventory {
	private:
	std::vector<Slot> slots;

	public:
	Inventory(size_t slots)
		:slots(slots, Slot(10)) {}

	size_t AddItem(int id, unsigned int quantity);
	Slot TakeItem(int id, unsigned amount);
};