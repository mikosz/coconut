#include "Serialiser.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::file_io;

void coconut::pulp::file_io::serialise(Serialiser& serialiser, const milk::math::Vector2d& vector) {
	serialiser(Serialiser::Label("x"), vector.x());
	serialiser(Serialiser::Label("y"), vector.y());
}

void coconut::pulp::file_io::serialise(Serialiser& serialiser, const milk::math::Vector3d& vector) {
	serialiser(Serialiser::Label("x"), vector.x());
	serialiser(Serialiser::Label("y"), vector.y());
	serialiser(Serialiser::Label("z"), vector.z());
}

void coconut::pulp::file_io::serialise(Serialiser& serialiser, const milk::math::Vector4d& vector) {
	serialiser(Serialiser::Label("x"), vector.x());
	serialiser(Serialiser::Label("y"), vector.y());
	serialiser(Serialiser::Label("z"), vector.z());
	serialiser(Serialiser::Label("w"), vector.w());
}
