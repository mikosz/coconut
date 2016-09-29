#ifndef _COCONUT_PULP_RENDERER_SHADER_PARAMETERFACTORY_HPP_
#define _COCONUT_PULP_RENDERER_SHADER_PARAMETERFACTORY_HPP_

#include <mutex>
#include <memory>
#include <string>
#include <functional>
#include <iosfwd>

#include <coconut-tools/factory.hpp>
#include <coconut-tools/policy/creation/Functor.hpp>
#include <coconut-tools/exceptions/RuntimeError.hpp>
#include <coconut-tools/utils/hash-combine.hpp>

#include "coconut/milk/graphics/Renderer.hpp"

#include "Parameter.hpp"

namespace coconut {
namespace pulp {
namespace renderer {
namespace shader {

struct ParameterFactoryInstanceDetails {
	
	std::string id;

	int arrayedElementIndex;

	std::string structurePrefix;

	ParameterFactoryInstanceDetails(
		std::string id,
		std::string structurePrefix = "",
		int arrayedElementIndex = -1
		) :
		id(std::move(id)),
		arrayedElementIndex(arrayedElementIndex),
		structurePrefix(std::move(structurePrefix))
	{
	}

};

// NOTE: comparison ignores the arrayedElementIndex field
bool operator==(const ParameterFactoryInstanceDetails& lhs, const ParameterFactoryInstanceDetails& rhs);
std::ostream& operator<<(std::ostream& os, const ParameterFactoryInstanceDetails& instanceDetails);

} // namespace shader
} // namespace renderer
} // namespace pulp
} // namespace coconut

namespace std {

template <>
struct hash<coconut::pulp::renderer::shader::ParameterFactoryInstanceDetails> {

	size_t operator()(const coconut::pulp::renderer::shader::ParameterFactoryInstanceDetails& instanceDetails) const {
		size_t seed = 0;
		coconut_tools::utils::hashCombine(seed, std::hash_value(instanceDetails.id));
		coconut_tools::utils::hashCombine(seed, std::hash_value(instanceDetails.arrayedElementIndex));
		coconut_tools::utils::hashCombine(seed, std::hash_value(instanceDetails.structurePrefix));
		return seed;
	}

};

} // namespace std

namespace coconut {
namespace pulp {
namespace renderer {
namespace shader {

namespace detail {

// TODO: make coconut_tools::policy::creation::Functor take <ReturnType, Arguments...> as template arguments,
// otherwise have to specify arguments twice.
class ParameterCreator :
	public coconut_tools::factory::CreatorRegistry<
		ParameterFactoryInstanceDetails,
		coconut_tools::policy::creation::Functor<
			std::function<std::unique_ptr<UnknownParameter> (const ParameterFactoryInstanceDetails&)>,
			const ParameterFactoryInstanceDetails&
			>,
		coconut_tools::factory::error_policy::ExceptionThrowing
		>
{
public:

	ParameterCreator();

protected:

	std::unique_ptr<UnknownParameter> doCreate(const ParameterFactoryInstanceDetails& instanceDetails);

private:

	using Super = coconut_tools::factory::CreatorRegistry<
		ParameterFactoryInstanceDetails,
		coconut_tools::policy::creation::Functor<
			std::function<std::unique_ptr<UnknownParameter> (const ParameterFactoryInstanceDetails&)>,
			const ParameterFactoryInstanceDetails&
			>,
		coconut_tools::factory::error_policy::ExceptionThrowing
		>;

	void registerBuiltins();

};

} // namespace detail

class BadParameterType : public coconut_tools::exceptions::RuntimeError {
public:

	BadParameterType(UnknownParameter::OperandType type) :
		coconut_tools::exceptions::RuntimeError("Provided type: " + toString(type) + " cannot be used as a parameter basic type"),
		type_(type)
	{
	}

	UnknownParameter::OperandType type() const noexcept {
		return type_;
	}

private:

	UnknownParameter::OperandType type_;

};

using ParameterFactory = 
	coconut_tools::Factory<
		ParameterFactoryInstanceDetails,
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
