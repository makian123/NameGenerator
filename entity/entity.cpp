#include "entity.h"
#include <utility>

Entity::Entity(const std::string &name, float hp, sf::Vector3i pos)
	:name(name), hp(hp), maxhp(hp), position(pos) {}

void Entity::TakeDamage(float dmg) {
	hp = std::max<float>(0, hp - dmg);
}

bool Entity::PickUp(const std::shared_ptr<BaseItem> &item) {
	return false;
}