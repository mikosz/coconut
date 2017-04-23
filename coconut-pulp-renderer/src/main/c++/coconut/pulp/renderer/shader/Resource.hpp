#ifndef _COCONUT_PULP_RENDERER_SHADER_RESOURCE_HPP_
#define _COCONUT_PULP_RENDERER_SHADER_RESOURCE_HPP_

#include <functional>

#include "coconut/milk/utils/MakePointerDefinitionsMacro.hpp"

#include "coconut/milk/graphics/Resource.hpp"

#include "../DrawCommand.hpp"

namespace coconut {
namespace pulp {
namespace renderer {

class PassContext;

namespace shader {

class Resource {
public:

	virtual ~Resource() = default;

	virtual void bind(DrawCommand& drawCommand, const PassContext& context) = 0;

};

CCN_MAKE_POINTER_DEFINITIONS(Resource);

// TODO: should sampler be a resource? thanks to this we have this weird ResourceResource class
class DataResource : public Resource {
public:

	// TODO: pointer?
	using DataCallback = std::function<const milk::graphics::Resource* (const PassContext&)>;

	DataResource(
		DataCallback callback,
		milk::graphics::ShaderType shaderType,
		size_t slot
		) :
		callback_(callback),
		stage_(shaderType),
		slot_(slot)
	{
	}

	void bind(DrawCommand& drawCommand, const PassContext& context) override {
		auto* resource = callback_(context);
		if (resource) {
			drawCommand.addResource(resource, stage_, slot_);
		}
	}
	
private:

	DataCallback callback_;

	milk::graphics::ShaderType stage_;

	size_t slot_;

};

class SamplerResource : public Resource {
public:

	// TODO: pointer?
	using SamplerCallback = std::function<const milk::graphics::Sampler* (const PassContext&)>;

	SamplerResource(
		SamplerCallback callback,
		milk::graphics::ShaderType shaderType,
		size_t slot
		) :
		callback_(callback),
		stage_(shaderType),
		slot_(slot)
	{
	}

	void bind(DrawCommand& drawCommand, const PassContext& context) override {
		auto* sampler = callback_(context);
		if (sampler) {
			drawCommand.addSampler(*sampler, stage_, slot_);
		}
	}
	
private:

	SamplerCallback callback_;

	milk::graphics::ShaderType stage_;

	size_t slot_;

};

} // namespace shader
} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_SHADER_RESOURCE_HPP_ */
