#pragma once

#include <random>
#include <type_traits>

class Random {
	private:
	static std::random_device device;
	static std::mt19937_64 generator;

	public:
	template<typename T>
	static T GenerateRand(T min, T max){
		static_assert(std::is_fundamental<T>::value);
		
		using randType = std::conditional_t<
			std::is_integral_v<T>,
			std::uniform_int_distribution<T>,
			std::uniform_real_distribution<T>>;

		randType dist(min, max);

		return dist(generator);
	}
};