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

void coconut::pulp::file_io::serialise(
	Deserialiser& deserialiser,
	milk::graphics::Rasteriser::Configuration& rasteriserConfiguration
	) {
	deserialiser(Deserialiser::Label("cullMode"), rasteriserConfiguration.cullMode);
	deserialiser(Deserialiser::Label("fillMode"), rasteriserConfiguration.fillMode);
	deserialiser(Deserialiser::Label("frontCounterClockwise"), rasteriserConfiguration.frontCounterClockwise);
}

void coconut::pulp::file_io::serialise(
	Deserialiser& deserialiser,
	milk::graphics::Sampler::Configuration& samplerConfiguration
	) {
	deserialiser(Deserialiser::Label("addressModeU"), samplerConfiguration.addressModeU);
	deserialiser(Deserialiser::Label("addressModeV"), samplerConfiguration.addressModeV);
	deserialiser(Deserialiser::Label("addressModeW"), samplerConfiguration.addressModeW);
	deserialiser(Deserialiser::Label("filter"), samplerConfiguration.filter);
}
