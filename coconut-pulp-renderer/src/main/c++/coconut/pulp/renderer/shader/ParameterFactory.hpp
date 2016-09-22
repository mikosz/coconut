#ifndef _COCONUT_PULP_RENDERER_SHADER_PARAMETERFACTORY_HPP_
#define _COCONUT_PULP_RENDERER_SHADER_PARAMETERFACTORY_HPP_

#include <mutex>
#include <memory>
#include <string>
#include <functional>

#include <coconut-tools/factory.hpp>
#include <coconut-tools/policy/creation/Functor.hpp>

#include "coconut/milk/graphics/Renderer.hpp"

#include "Parameter.hpp"

namespace coconut {
namespace pulp {
namespace renderer {
namespace shader {

namespace detail {

class ParameterCreator :
	public coconut_tools::factory::CreatorRegistry<
		std::string,
		coconut_tools::policy::creation::Functor<std::function<std::unique_ptr<UnknownParameter> ()>>,
		coconut_tools::factory::error_policy::ExceptionThrowing
		>
{
public:

	ParameterCreator();

protected:

	std::unique_ptr<UnknownParameter> doCreate(const std::string& id);

private:

	using Super = coconut_tools::factory::CreatorRegistry<
		std::string,
		coconut_tools::policy::creation::Functor<std::function<std::unique_ptr<UnknownParameter> ()>>,
		coconut_tools::factory::error_policy::ExceptionThrowing
		>;

	void registerBuiltins();

};

} // namespace detail

using ParameterFactory = 
	coconut_tools::Factory<
		std::string,
		UnknownParameter,
		coconut_tools::factory::storage::Volatile,
		detail::ParameterCreator,
		std::mutex
		>;

} // namespace shader
} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_SHADER_PARAMETERFACTORY_HPP_ */
