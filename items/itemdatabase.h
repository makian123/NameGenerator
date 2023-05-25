#pragma once

#include <map>
#include <memory>
#include "item.h"

class ItemDatabase {
	private:
	static std::map<int, std::shared_ptr<BaseItem>> db;

	public:
	ItemDatabase() {}

	static std::shared_ptr<BaseItem> GetItem(int id);
	static void AddItem(std::shared_ptr<BaseItem> itm);
	static int NextID();
};