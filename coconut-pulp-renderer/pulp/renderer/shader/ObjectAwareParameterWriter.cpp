#include "ObjectAwareParameterWriter.hpp"

#include <string>
#include <stdexcept>

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::renderer;
using namespace coconut::pulp::renderer::shader;

void* ObjectAwareParameterWriter::write(void* target, ParameterId parameterId) {
	milk::math::Matrix* matrix = reinterpret_cast<milk::math::Matrix*>(target);

	switch (parameterId) {
	case ParameterId::WORLD_MATRIX:
		*matrix = model_->worldTransformation().transposed();
		break;
	default:
		throw std::logic_error("Unexpected parameter id " + std::to_string(static_cast<int>(parameterId)));
	}

	return ++matrix;
}
