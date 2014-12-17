#include "ParameterWriterList.hpp"

#include <string>
#include <stdexcept>

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::renderer;
using namespace coconut::pulp::renderer::shader;

void* ParameterWriterList::write(void* target, ParameterId parameterId) {
	Writers::iterator it = writers_.find(parameterId);
	if (it != writers_.end()) {
		return it->second->write(target, parameterId);
	} else {
		throw std::logic_error(
			"No handler for parameter " + std::to_string(static_cast<int>(parameterId)));
	}
}

void ParameterWriterList::add(ParameterId parameterId, ParameterWriterSharedPtr writer) {
	Writers::iterator it = writers_.find(parameterId);
	if (it != writers_.end()) {
		throw std::logic_error(
			"A handler for " + std::to_string(static_cast<int>(parameterId)) + " already exists");
	} else {
		writers_.insert(it, std::make_pair(parameterId, writer));
	}
}
