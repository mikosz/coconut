#ifndef _COCONUT_PULP_RENDERER_SHADER_INPUTELEMENTFACTORY_HPP_
#define _COCONUT_PULP_RENDERER_SHADER_INPUTELEMENTFACTORY_HPP_

#include <string>
#include <mutex>
#include <memory>

#include <unordered_map>

#include <coconut-tools/factory.hpp>
#include <coconut-tools/policy/creation/Functor.hpp>
#include <coconut-tools/utils/hash-combine.hpp>

#include "coconut/milk/graphics/Renderer.hpp"

#include "Input.hpp"

namespace coconut {
namespace pulp {
namespace renderer {
namespace shader {

struct InputElementFactoryInstanceDetails {
	
	std::string semantic;

	size_t semanticIndex;

	milk::graphics::PixelFormat format;

	InputElementFactoryInstanceDetails(
		std::string semantic,
		size_t semanticIndex,
		milk::graphics::PixelFormat format
		) :
		semantic(std::move(semantic)),
		semanticIndex(semanticIndex),
		format(format)
	{
	}

};

bool operator==(const InputElementFactoryInstanceDetails& lhs, const InputElementFactoryInstanceDetails& rhs);
std::ostream& operator<<(std::ostream& os, const InputElementFactoryInstanceDetails& instanceDetails);

} // namespace shader
} // namespace renderer
} // namespace pulp
} // namespace coconut

namespace std {

template <>
struct hash<coconut::pulp::renderer::shader::InputElementFactoryInstanceDetails> {

	size_t operator()(
		const coconut::pulp::renderer::shader::InputElementFactoryInstanceDetails& instanceDetails) const
	{
		size_t seed = 0;
		coconut_tools::utils::hashCombine(seed, std::hash_value(instanceDetails.semantic));
		coconut_tools::utils::hashCombine(seed, std::hash_value(instanceDetails.semanticIndex));
		coconut_tools::utils::hashCombine(seed, std::hash_value(instanceDetails.format));
		return seed;
	}

};

} // namespace std

namespace coconut {
namespace pulp {
namespace renderer {
namespace shader {

namespace detail {

class InputElementCreator :
	public coconut_tools::factory::CreatorRegistry<
		InputElementFactoryInstanceDetails,
		coconut_tools::policy::creation::Functor<
			std::function<std::unique_ptr<Input::Element> (const InputElementFactoryInstanceDetails&)>,
			const InputElementFactoryInstanceDetails&
			>,
		coconut_tools::factory::error_policy::ExceptionThrowing
		>
{
public:

	InputElementCreator();

private:

	using Super = coconut_tools::factory::CreatorRegistry<
		InputElementFactoryInstanceDetails,
		coconut_tools::policy::creation::Functor<
			std::function<std::unique_ptr<Input::Element> (const InputElementFactoryInstanceDetails&)>,
			const InputElementFactoryInstanceDetails&
			>,
		coconut_tools::factory::error_policy::ExceptionThrowing
		>;

	void registerBuiltins();

};

} // namespace detail

using InputElementFactory = 
	coconut_tools::Factory<
		InputElementFactoryInstanceDetails,
		Input::Element,
		coconut_tools::factory::storage::Volatile,
		detail::InputElementCreator,
		std::mutex
		>;

} // namespace shader
} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_SHADER_INPUTELEMENTFACTORY_HPP_ */
