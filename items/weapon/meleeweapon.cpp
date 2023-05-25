#include "meleeweapon.h"
#include "../../entity/entity.h"
#include "../../util/customrandom.h"

static const char *TypeNames[] = {
	"LONGSWORD",
	"GREATSWORD",
	"MACE",
	"CLUB",
	"FLAIL",
	"MORNING STAR",
	"WAR_HAMMER",
	"QUARTERSTAFF",
	"BARDICHE",
	"BILL",
	"SPEAR",
	"ARMING SWORD",
	"DAGGER",
	"ESTOC",
	"FALCHION",
	"KNIFE",
	"MESSER",
	"RAPIER",
	"SABRE",
	"SHORTSWORD"
};

static std::string GetMeleeTypeName(MeleeType type) {
	return std::string(TypeNames[(size_t)type]);
}

MeleeWeapon::MeleeWeapon(float dmg, int durability)
	:dmg(dmg), currDurability(durability), maxdurability(durability) {
	this->type = (MeleeType)Random::GenerateRand<size_t>(0, (size_t)MeleeType::LAST - 1);
	this->name = GetMeleeTypeName(type);
}

void MeleeWeapon::Attack(Entity &entity) {
	if (!currDurability) return;
	currDurability--;

	entity.TakeDamage(dmg);
}