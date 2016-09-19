#ifndef _COCONUT_PULP_RENDERER_SHADER_PARAMETERFACTORY_HPP_
#define _COCONUT_PULP_RENDERER_SHADER_PARAMETERFACTORY_HPP_

#include <mutex>
#include <memory>
#include <string>

#include <coconut-tools/factory.hpp>

#include "coconut/milk/graphics/Renderer.hpp"

#include "Parameter.hpp"

namespace coconut {
namespace pulp {
namespace renderer {
namespace shader {

namespace detail {

template <class... UpdateArguments>
class ParameterCreator { // TODO: use CreatorRegistry and let it accept params?
public:

	std::unique_ptr<Parameter> doCreate(
		const std::string& id,
		UpdateArguments&&... arguments
		) {
	}

};

} // namespace detail

template <class... UpdateArguments>
using ParameterFactory = 
	coconut_tools::Factory<
		std::string,
		Parameter<UpdateArguments...>,
		coconut_tools::factory::storage::Volatile,
		detail::ParameterCreator<UpdateArguments...>,
		std::mutex
		>;

} // namespace shader
} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_SHADER_PARAMETERFACTORY_HPP_ */
