#include "customrandom.h"

std::random_device Random::device;
std::mt19937_64 Random::generator(device());