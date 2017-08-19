#include "PerlinNoise.hpp"

#include <algorithm>
#include <random>
#include <cmath>

#include "common.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::math;

/*
 * Perlin-noise implementation based on https://github.com/sol-prog/Perlin_Noise
 */

namespace /* anonymous */ {

const size_t VALUES = 256;

float fade(float t) { 
	return t * t * t * (t * (t * 6 - 15) + 10);
}

float grad(size_t hash, float x, float y, float z) {
	auto h = static_cast<int>(hash & 15);
	
	float u = h < 8 ? x : y;
	float v = h < 4 ? y : (h == 12 || h == 14) ? x : z;

	return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

} // anonymous namespace

PerlinNoise::PerlinNoise(unsigned int seed) {
	data_.reserve(VALUES * 2);
	data_.resize(VALUES);
	std::iota(data_.begin(), data_.end(), 0);

	auto randomEngine = std::default_random_engine(seed);
	std::shuffle(data_.begin(), data_.end(), randomEngine);

	data_.insert(data_.end(), data_.begin(), data_.end());
}

float PerlinNoise::sample(const Vec3& where) const {
	const auto cubeIdxX = static_cast<size_t>(std::floor(where.x())) & 255;
	const auto cubeIdxY = static_cast<size_t>(std::floor(where.y())) & 255;
	const auto cubeIdxZ = static_cast<size_t>(std::floor(where.z())) & 255;

	const auto cubeX = where.x() - std::floor(where.x());
	const auto cubeY = where.y() - std::floor(where.y());
	const auto cubeZ = where.z() - std::floor(where.z());

	const auto fadeX = fade(cubeX);
	const auto fadeY = fade(cubeY);
	const auto fadeZ = fade(cubeZ);

	const auto a = data_[cubeIdxX] + cubeIdxY;
	const auto aa = data_[a] + cubeIdxZ;
	const auto ab = data_[a + 1] + cubeIdxZ;
	const auto b = data_[cubeIdxX + 1] + cubeIdxY;
	const auto ba = data_[b] + cubeIdxZ;
	const auto bb = data_[b + 1] + cubeIdxZ;

	const auto result = lerp(
		fadeZ,
		lerp(fadeY,
			lerp(grad(data_[aa], cubeX, cubeY, cubeZ), grad(data_[ba], cubeX - 1, cubeY, cubeZ), fadeX),
			lerp(grad(data_[ab], cubeX, cubeY - 1, cubeZ), grad(data_[bb], cubeX - 1, cubeY - 1, cubeZ), fadeX)
			),
		lerp(fadeY,
			lerp(grad(data_[aa + 1], cubeX, cubeY, cubeZ - 1), grad(data_[ba + 1], cubeX - 1, cubeY, cubeZ - 1), fadeX),
			lerp(grad(data_[ab + 1], cubeX, cubeY-1, cubeZ - 1), grad(data_[bb + 1], cubeX - 1, cubeY - 1, cubeZ - 1), fadeX)
			)
		);
	return (result + 1.0f) / 2.0f;
}
