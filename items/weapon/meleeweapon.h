#pragma once

#include "../item.h"

class Entity;

enum class MeleeType: size_t {
	LONGSWORD = 0,
	GREATSWORD,
	MACE,
	CLUB,
	FLAIL,
	MORNING_STAR,
	WAR_HAMMER,
	QUARTERSTAFF,
	BARDICHE,
	BILL,
	SPEAR,

	ARMING_SWORD,
	DAGGER,
	ESTOC,
	FALCHION,
	KNIFE,
	MESSER,
	RAPIER,
	SABRE,
	SHORTSWORD,

	LAST = SHORTSWORD
};

class MeleeWeapon: public BaseItem {
	private:
	float dmg;
	MeleeType type;
	int currDurability, maxdurability;

	public:
	friend class BuffDamage;
	MeleeWeapon(float dmg, int durability);

	void Attack(Entity &entity);

	int GetDurability() const { return currDurability; }
};