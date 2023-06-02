#pragma once

#include "block.h"

class Air: public Block {
	Air(const Vector3 &pos)
		:Block(0, pos) {}
};