#include "Deserialiser.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::file_io;

void coconut::pulp::file_io::serialise(Deserialiser& deserialiser, milk::math::Vector2d& vector) {
	deserialiser(Deserialiser::Label("x"), vector.x());
	deserialiser(Deserialiser::Label("y"), vector.y());
}

void coconut::pulp::file_io::serialise(Deserialiser& deserialiser, milk::math::Vector3d& vector) {
	deserialiser(Deserialiser::Label("x"), vector.x());
	deserialiser(Deserialiser::Label("y"), vector.y());
	deserialiser(Deserialiser::Label("z"), vector.z());
}

void coconut::pulp::file_io::serialise(Deserialiser& deserialiser, milk::math::Vector4d& vector) {
	deserialiser(Deserialiser::Label("x"), vector.x());
	deserialiser(Deserialiser::Label("y"), vector.y());
	deserialiser(Deserialiser::Label("z"), vector.z());
	deserialiser(Deserialiser::Label("w"), vector.w());
}
