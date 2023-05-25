#include "item.h"
#include "itemdatabase.h"

BaseItem::BaseItem(const std::string &name, const std::string &desc)
	:name(name), description(desc), id(ItemDatabase::NextID()) {}