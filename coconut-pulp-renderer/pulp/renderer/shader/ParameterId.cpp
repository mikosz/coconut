#include "ParameterId.hpp"

#include <string>
#include <stdexcept>

#include "milk/math/Matrix.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::renderer;
using namespace coconut::pulp::renderer::shader;

size_t coconut::pulp::renderer::shader::size(ParameterId id) {
	switch (id) {
	case ParameterId::WORLD_MATRIX:
	case ParameterId::PROJECTION_MATRIX:
	case ParameterId::VIEW_MATRIX:
		return sizeof(milk::math::Matrix);
	default:
		throw std::logic_error("Unknown parameter id " + std::to_string(static_cast<int>(id)));
	}
}
