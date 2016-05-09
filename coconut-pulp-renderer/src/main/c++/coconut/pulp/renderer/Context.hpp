#ifndef _COCONUT_PULP_RENDERER_CONTEXT_HPP_
#define _COCONUT_PULP_RENDERER_CONTEXT_HPP_

#include "MaterialManager.hpp"

namespace coconut {
namespace pulp {
namespace renderer {

class Context {
public:
	
	Context() = default;

	Context(const Context&) = delete;

	void operator=(const Context&) = delete;

	MaterialManager& materialManager() {
		return materialManager_;
	}

private:

	MaterialManager materialManager_;

};

} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_CONTEXT_HPP_ */
