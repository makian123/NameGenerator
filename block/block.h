#pragma once

#include <string>
#include "../math/vector3.h"

class Block {
protected:
	Vector3 coords;
	size_t id;

public:
	Block(size_t id, const Vector3 &coord):id(id), coords(coord) {}
	virtual ~Block() {}

	size_t GetID() const { return id; }
	Vector3 GetPos() const { return coords; }
};