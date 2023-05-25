#include "entity.h"
#include <utility>

Entity::Entity(const std::string &name, float hp) 
	:name(name), hp(hp), maxhp(hp) {}

void Entity::TakeDamage(float dmg) {
	hp = std::max<float>(0, hp - dmg);
}

bool Entity::PickUp(const std::shared_ptr<BaseItem> &item) {
	return false;
}