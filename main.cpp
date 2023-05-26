#include <iostream>

#include "items/itemdatabase.h"
#include "items/item.h"
#include "items/weapon/meleeweapon.h"
#include "items/inventory.h"
#include "util/namegenerator.h"

int main() {
	NameGenerator humanNames("data/elf/");

	std::cout << "Male:\n";
	for (size_t i = 0; i < 10; ++i) {
		std::cout << humanNames.GenerateName(true) << "\n";
	}
	std::cout << "\nFemale:\n";
	for (size_t i = 0; i < 10; ++i) {
		std::cout << humanNames.GenerateName(false) << "\n";
	}

	return 0;
}