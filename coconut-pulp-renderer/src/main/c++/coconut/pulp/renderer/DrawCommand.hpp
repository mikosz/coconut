#ifndef _COCONUT_PULP_RENDERER_DRAWCOMMAND_HPP_
#define _COCONUT_PULP_RENDERER_DRAWCOMMAND_HPP_

#include <cstdint>
#include <vector>

#include "coconut/milk/graphics/CommandList.hpp"
#include "coconut/milk/graphics/ConstantBuffer.hpp"
#include "coconut/milk/graphics/RenderState.hpp"
#include "coconut/milk/graphics/Sampler.hpp"
#include "coconut/milk/graphics/ShaderType.hpp"
#include "coconut/milk/graphics/InputLayout.hpp"
#include "coconut/milk/graphics/Shader.hpp"
#include "coconut/milk/graphics/Texture2d.hpp"
#include "coconut/milk/graphics/Viewport.hpp"
#include "coconut/milk/graphics/VertexBuffer.hpp"
#include "coconut/milk/graphics/IndexBuffer.hpp"
#include "coconut/milk/graphics/PrimitiveTopology.hpp"

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

	Key key() const {
		return 0ull; // TODO
	}

	void submit(milk::graphics::CommandList& commandList);

	void setInputLayout(milk::graphics::InputLayout* inputLayout) { // TODO: POINTERS!!! CONST!
		inputLayout_ = inputLayout;
	}

	void setVertexShader(milk::graphics::VertexShader* vertexShader) {
		vertexShader_ = vertexShader;
	}

	void setGeometryShader(milk::graphics::GeometryShader* geometryShader) {
		geometryShader_ = geometryShader;
	}

	void setHullShader(milk::graphics::HullShader* hullShader) {
		hullShader_ = hullShader;
	}

	void setDomainShader(milk::graphics::DomainShader* domainShader) {
		domainShader_ = domainShader;
	}

	void setPixelShader(milk::graphics::PixelShader* pixelShader) {
		pixelShader_ = pixelShader;
	}

	void setRenderState(const milk::graphics::RenderState* renderState) {
		renderState_ = renderState;
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
		const milk::graphics::Texture2d texture,
		milk::graphics::ShaderType stage,
		size_t slot
		) {
		textures2d_.emplace_back(texture, stage, slot);
	}

	void addSampler(milk::graphics::Sampler sampler, milk::graphics::ShaderType stage, size_t slot) {
		samplers_.emplace_back(std::move(sampler), stage, slot);
	}

	void setRenderTarget(milk::graphics::Texture2d* texture) { // TODO: pointers
		renderTarget_ = texture;
	}

	void setDepthStencil(milk::graphics::Texture2d* texture) {
		depthStencil_ = texture;
	}

	void setViewport(milk::graphics::Viewport* viewport) {
		viewport_ = viewport;
	}

		// TODO: pointers!
	void setVertexBuffer(milk::graphics::VertexBuffer* vertexBuffer) {
		vertexBuffer_ = vertexBuffer;
	}

	void setIndexBuffer(milk::graphics::IndexBuffer* indexBuffer) { // TODO: index count and index buffer in one call?
		indexBuffer_ = indexBuffer;
	}

	void setIndexCount(size_t indexCount) {
		indexCount_ = indexCount;
	}

	void setPrimitiveTopology(milk::graphics::PrimitiveTopology primitiveTopology) {
		primitiveTopology_ = primitiveTopology;
	}

	void setInstanceDataBuffer(milk::graphics::VertexBuffer* instanceDataBuffer) {
		instanceDataBuffer_ = instanceDataBuffer;
	}

	void setInstanceCount(size_t instanceCount) {
		instanceCount_ = instanceCount;
	}

private:

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

	struct Resource {

		milk::graphics::ShaderType stage;

		size_t slot;

		Resource(milk::graphics::ShaderType stage, size_t slot) :
			stage(stage),
			slot(slot)
		{
		}

	};

	struct Texture2d : Resource {

		milk::graphics::Texture2d texture;

		Texture2d(milk::graphics::Texture2d texture, milk::graphics::ShaderType stage, size_t slot) :
			Resource(stage, slot),
			texture(texture)
		{
		}

	};

	struct Sampler : Resource {

		milk::graphics::Sampler sampler;

		Sampler(milk::graphics::Sampler sampler, milk::graphics::ShaderType stage, size_t slot) :
			Resource(stage, slot),
			sampler(sampler)
		{
		}

	};

	using ConstantBuffersData = std::vector<ConstantBufferData>; // TODO: ,,

	using Textures2d = std::vector<Texture2d>; // TODO: ,,

	using Samplers = std::vector<Sampler>; // TODO: use array to avoid allocs

	// TODO: pointers
	milk::graphics::Viewport* viewport_ = nullptr;

	milk::graphics::Texture2d* renderTarget_ = nullptr;

	milk::graphics::Texture2d* depthStencil_ = nullptr;

	milk::graphics::InputLayout* inputLayout_ = nullptr;

	milk::graphics::VertexShader* vertexShader_ = nullptr;

	milk::graphics::GeometryShader* geometryShader_ = nullptr;

	milk::graphics::HullShader* hullShader_ = nullptr;

	milk::graphics::DomainShader* domainShader_ = nullptr;

	milk::graphics::PixelShader* pixelShader_ = nullptr;

	const milk::graphics::RenderState* renderState_ = nullptr;

	Samplers samplers_;

	ConstantBuffersData constantBuffersData_;

	Textures2d textures2d_;

	milk::graphics::VertexBuffer* vertexBuffer_ = nullptr;

	milk::graphics::VertexBuffer* instanceDataBuffer_ = nullptr;

	size_t instanceCount_ = 0u;

	milk::graphics::IndexBuffer* indexBuffer_ = nullptr;

	size_t indexCount_ = 0u;

	milk::graphics::PrimitiveTopology primitiveTopology_ = milk::graphics::PrimitiveTopology::INVALID;

};

CCN_MAKE_POINTER_DEFINITIONS(DrawCommand);

} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_DRAWCOMMAND_HPP_ */
