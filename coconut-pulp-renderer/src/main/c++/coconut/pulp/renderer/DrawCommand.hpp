#ifndef _COCONUT_PULP_RENDERER_DRAWCOMMAND_HPP_
#define _COCONUT_PULP_RENDERER_DRAWCOMMAND_HPP_

#include <cstdint>
#include <vector>

#include "coconut/milk/graphics/CommandList.hpp"
#include "coconut/milk/graphics/Rasteriser.hpp"
#include "coconut/milk/graphics/Sampler.hpp"
#include "coconut/milk/graphics/ShaderType.hpp"
#include "coconut/milk/graphics/Shader.hpp"

#include "coconut/milk/utils/MakePointerDefinitionsMacro.hpp"

namespace coconut {
namespace pulp {
namespace renderer {

// TODO: figure out the draw command types. Compute command should also be incorporated here, but wont
// use the stuff from this class (add GPUCommand as superclass with run and key?)
class DrawCommand {
public:

	using Key = std::uint64_t;

	DrawCommand(const DrawCommand&) = delete;

	virtual ~DrawCommand() = default;

	void operator=(const DrawCommand&) = delete;

	virtual Key key() const = 0; // TODO: key should not be here, or should be provided as a setter

	virtual void run(milk::graphics::CommandList& commandList);

	void setVertexShader(milk::graphics::VertexShader* vertexShader) {
		vertexShader_ = vertexShader;
	}

	void setPixelShader(milk::graphics::PixelShader* pixelShader) {
		pixelShader_ = pixelShader;
	}

	void setRasteriser(milk::graphics::Rasteriser* rasteriser) {
		rasteriser_ = rasteriser;
	}

	void addSampler(milk::graphics::Sampler* sampler, milk::graphics::ShaderType stage, size_t slot) {
		samplers_.emplace_back(sampler, stage, slot);
	}

	void addConstantBufferData(
		milk::graphics::ConstantBuffer* constantBuffer,
		std::uint8_t* data,
		size_t size,
		milk::graphics::ShaderType stage,
		size_t slot
		) {
		constantBuffersData_.emplace_back(data, size, stage, slot);
	}

private:

	struct Sampler {

		milk::graphics::Sampler* sampler; // TODO: pointer

		milk::graphics::ShaderType stage;

		size_t slot;

		Sampler(milk::graphics::Sampler* sampler, milk::graphics::ShaderType stage, size_t slot) :
			sampler(sampler),
			stage(stage),
			slot(slot)
		{
		}

	};

	struct ConstantBufferData {

		using Data = std::vector<std::uint8_t>; // TODO: array?

		milk::graphics::ConstantBuffer* constantBuffer;

		Data data;

		milk::graphics::ShaderType stage;

		size_t slot;

		ConstantBufferData(std::uint8_t* dataPtr, size_t size, milk::graphics::ShaderType stage, size_t slot);

	};

	using Samplers = std::vector<Sampler>; // TODO: use array to avoid allocs

	using ConstantBuffersData = std::vector<ConstantBufferData>; // TODO: ,,

	// TODO: pointers
	milk::graphics::VertexShader* vertexShader_ = nullptr;

	milk::graphics::PixelShader* pixelShader_ = nullptr;

	milk::graphics::Rasteriser* rasteriser_ = nullptr;

	Samplers samplers_;

	ConstantBuffersData constantBuffersData_;

};

CCN_MAKE_POINTER_DEFINITIONS(DrawCommand);

} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_DRAWCOMMAND_HPP_ */
