#ifndef _COCONUT_PULP_RENDERER_SHADER_RESOURCEFACTORY_HPP_
#define _COCONUT_PULP_RENDERER_SHADER_RESOURCEFACTORY_HPP_

#include <string>
#include <mutex>
#include <memory>
#include <functional>
#include <unordered_map>

#include <coconut-tools/factory.hpp>
#include <coconut-tools/factory/CreatorRegistry.hpp>
#include <coconut-tools/policy/creation/Functor.hpp>

#include "coconut/milk/graphics/ShaderType.hpp"

#include "Resource.hpp"

namespace coconut {
namespace pulp {
namespace renderer {

class PassContext;

namespace shader {

namespace detail {

class ResourceCreator :
	public coconut_tools::factory::CreatorRegistry<
		std::string,
		// TODO: these template parameters are terribly inconvenient
		coconut_tools::policy::creation::Functor<
			std::function<std::unique_ptr<Resource>(milk::graphics::ShaderType, size_t)>,
			milk::graphics::ShaderType,
			size_t
			>,
		coconut_tools::factory::error_policy::ExceptionThrowing
		>
{
public:

	using Instance = std::unique_ptr<Resource>;

	ResourceCreator();

protected:

	Instance doCreate(
		const std::string& id,
		milk::graphics::ShaderType shaderType,
		size_t slot
		);

private:

	using Super = coconut_tools::factory::CreatorRegistry<
		std::string,
		coconut_tools::policy::creation::Functor<
			std::function<std::unique_ptr<Resource>(milk::graphics::ShaderType, size_t)>,
			milk::graphics::ShaderType,
			size_t
			>,
		coconut_tools::factory::error_policy::ExceptionThrowing
		>;

	void registerBuiltins();

};

} // namespace detail

using ResourceFactory = 
	coconut_tools::Factory<
		std::string,
		detail::ResourceCreator,
		coconut_tools::factory::storage::Volatile,
		std::mutex
		>;

} // namespace shader
} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_SHADER_RESOURCEFACTORY_HPP_ */
