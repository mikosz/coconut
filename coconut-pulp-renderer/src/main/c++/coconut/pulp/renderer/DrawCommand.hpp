#ifndef _COCONUT_PULP_RENDERER_DRAWCOMMAND_HPP_
#define _COCONUT_PULP_RENDERER_DRAWCOMMAND_HPP_

#include <cstdint>
#include <vector>

#include "coconut/milk/graphics/CommandList.hpp"
#include "coconut/milk/graphics/ConstantBuffer.hpp"
#include "coconut/milk/graphics/Rasteriser.hpp"
#include "coconut/milk/graphics/Sampler.hpp"
#include "coconut/milk/graphics/ShaderType.hpp"
#include "coconut/milk/graphics/InputLayout.hpp"
#include "coconut/milk/graphics/Shader.hpp"
#include "coconut/milk/graphics/Texture.hpp"
#include "coconut/milk/graphics/Viewport.hpp"

#include "coconut/milk/utils/MakePointerDefinitionsMacro.hpp"

namespace coconut {
namespace pulp {
namespace renderer {

// TODO: figure out the draw command types. Compute command should also be incorporated here, but wont
// use the stuff from this class (add GPUCommand as superclass with run and key?)
class DrawCommand {
public:

	using Key = std::uint64_t;

	DrawCommand() = default;

	DrawCommand(const DrawCommand&) = delete;

	virtual ~DrawCommand() = default;

	void operator=(const DrawCommand&) = delete;

	virtual Key key() const = 0; // TODO: key should not be here, or should be provided as a setter

	// TODO: submit should be purely virtual: put common stuff in other function, or have a purely virtual doSubmit
	virtual void submit(milk::graphics::CommandList& commandList);

	void setInputLayout(milk::graphics::InputLayout* inputLayout) {
		inputLayout_ = inputLayout;
	}

	void setVertexShader(milk::graphics::VertexShader* vertexShader) {
		vertexShader_ = vertexShader;
	}

	void setPixelShader(milk::graphics::PixelShader* pixelShader) {
		pixelShader_ = pixelShader;
	}

	void setRasteriser(milk::graphics::Rasteriser* rasteriser) {
		rasteriser_ = rasteriser;
	}

	void addSampler(milk::graphics::Sampler sampler, milk::graphics::ShaderType stage, size_t slot) {
		samplers_.emplace_back(std::move(sampler), stage, slot);
	}

	void addConstantBufferData(
		milk::graphics::ConstantBuffer* constantBuffer,
		std::uint8_t* data,
		size_t size,
		milk::graphics::ShaderType stage,
		size_t slot
		) {
		constantBuffersData_.emplace_back(constantBuffer, data, size, stage, slot);
	}

	void addTexture(
		milk::graphics::Texture* texture,
		milk::graphics::ShaderType stage,
		size_t slot
		) {
		textures_.emplace_back(texture, stage, slot);
	}

	void setRenderTarget(milk::graphics::Texture2d* texture) {
		renderTarget_ = texture;
	}

	void setDepthStencil(milk::graphics::Texture2d* texture) {
		depthStencil_ = texture;
	}

	void setViewport(milk::graphics::Viewport* viewport) {
		viewport_ = viewport;
	}

private:

	struct Sampler {

		milk::graphics::Sampler sampler;

		milk::graphics::ShaderType stage;

		size_t slot;

		Sampler(milk::graphics::Sampler sampler, milk::graphics::ShaderType stage, size_t slot) :
			sampler(sampler),
			stage(stage),
			slot(slot)
		{
		}

	};

	struct ConstantBufferData {

		using Data = std::vector<std::uint8_t>; // TODO: array? pointer?

		milk::graphics::ConstantBuffer* constantBuffer;

		Data data;

		milk::graphics::ShaderType stage;

		size_t slot;

		ConstantBufferData(
			milk::graphics::ConstantBuffer* constantBuffer,
			std::uint8_t* dataPtr,
			size_t size,
			milk::graphics::ShaderType stage,
			size_t slot
			);

	};

	struct Texture {

		milk::graphics::Texture* texture; // TODO: pointer

		milk::graphics::ShaderType stage;

		size_t slot;

		Texture(milk::graphics::Texture* texture, milk::graphics::ShaderType stage, size_t slot) :
			texture(texture),
			stage(stage),
			slot(slot)
		{
		}

	};

	using Samplers = std::vector<Sampler>; // TODO: use array to avoid allocs

	using ConstantBuffersData = std::vector<ConstantBufferData>; // TODO: ,,

	using Textures = std::vector<Texture>; // TODO: ,,

	// TODO: pointers
	milk::graphics::Viewport* viewport_ = nullptr;

	milk::graphics::Texture2d* renderTarget_ = nullptr;

	milk::graphics::Texture2d* depthStencil_ = nullptr;

	milk::graphics::InputLayout* inputLayout_ = nullptr;

	milk::graphics::VertexShader* vertexShader_ = nullptr;

	milk::graphics::PixelShader* pixelShader_ = nullptr;

	milk::graphics::Rasteriser* rasteriser_ = nullptr;

	Samplers samplers_;

	ConstantBuffersData constantBuffersData_;

	Textures textures_;

};

CCN_MAKE_POINTER_DEFINITIONS(DrawCommand);

} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_DRAWCOMMAND_HPP_ */
