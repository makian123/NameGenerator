#include "inventory.h"
#include "itemdatabase.h"

size_t Inventory::AddItem(int id, unsigned int quantity) {
	size_t originalQuantity = quantity;

	if (!quantity || id < 0) return quantity;

	for (auto &slot : slots) {
		if (!slot.GetItem().get() || slot.GetItem().get()->GetID() != id) continue;

		size_t toPut = std::min<size_t>(slot.GetMaxAmount() - slot.GetAmount(), quantity);
		slot.SetAmount(slot.GetAmount() + toPut);
		quantity -= toPut;

		if (!quantity) return 0;
	}
	for (auto &slot : slots) {
		if (slot.GetItem().get()) continue;

		size_t toPut = (quantity > slot.GetMaxAmount() ? slot.GetMaxAmount() : quantity);
		
		slot.SetItem(ItemDatabase::GetItem(id));
		slot.SetAmount(toPut);
		quantity -= toPut;

		if (!quantity) return 0;
	}

	return originalQuantity - quantity;
}
Slot Inventory::TakeItem(int id, unsigned amount) {
	Slot itm(0);
	if (!amount) return itm;

	for (auto &slot : slots) {
		if (!slot.GetItem().get()) continue;
		if (slot.GetItem().get()->GetID() != id) continue;

		itm.SetItem(slot.GetItem());
	}

	for (auto &currSlot : slots) {
		std::shared_ptr<BaseItem> tmpItm = currSlot.GetItem();
		if (!tmpItm.get()) continue;
		if (tmpItm.get()->GetID() != id) continue;

		unsigned int toTake = (amount - itm.GetAmount());

		if (currSlot.GetAmount() <= toTake) {
			itm.SetAmount(itm.GetAmount() + currSlot.GetAmount());
			currSlot.SetAmount(0);
			currSlot.SetItem(nullptr);
		}
		else {
			itm.SetAmount(amount);
			currSlot.SetAmount(currSlot.GetAmount() - toTake);
		}

		if (itm.GetAmount() == amount) break;
	}

	return itm;
}