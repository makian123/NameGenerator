#include "itemdatabase.h"

std::map<int, std::shared_ptr<BaseItem>> ItemDatabase::db;

std::shared_ptr<BaseItem> ItemDatabase::GetItem(int id) {
	if (db.find(id) == db.end()) return nullptr;

	return db.at(id);
}
void ItemDatabase::AddItem(std::shared_ptr<BaseItem> itm) {
	if (!itm.get()) return;
	if (db.find(itm.get()->GetID()) != db.end()) return;

	db[itm.get()->GetID()] = itm;
}

int ItemDatabase::NextID() {
	for (auto &[key, itm]: db) {
		if (!itm.get()) return key;
	}

	return db.size();
}