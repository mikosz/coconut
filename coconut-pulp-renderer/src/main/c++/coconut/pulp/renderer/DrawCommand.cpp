#include "DrawCommand.hpp"

#include <cassert>
#include <algorithm>
#include <iterator>

#include <iostream>
#include <fstream>

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::renderer;

void DrawCommand::submit(milk::graphics::CommandList& commandList) {
	assert(rasteriser_ != nullptr);
	assert(vertexShader_ != nullptr);
	assert(pixelShader_ != nullptr);

	commandList.setRasteriser(*rasteriser_);
	
	for (auto& sampler: samplers_) {
		commandList.setSampler(sampler.sampler, sampler.stage, sampler.slot);
	}

	for (auto& constantBufferData: constantBuffersData_) {
		auto lockedData = commandList.lock(
			*constantBufferData.constantBuffer, milk::graphics::CommandList::LockPurpose::WRITE_DISCARD);
		std::copy(constantBufferData.data.begin(), constantBufferData.data.end(), lockedData.get());

		commandList.setConstantBuffer(
			*constantBufferData.constantBuffer,
			constantBufferData.stage,
			constantBufferData.slot
			);

	}

	for (auto& texture: textures_) {
		commandList.setTexture(*texture.texture, texture.stage, texture.slot);
	}

	commandList.setInputLayout(*inputLayout_);
	commandList.setVertexShader(*vertexShader_);
	commandList.setPixelShader(*pixelShader_);

	commandList.setRenderTarget(*renderTarget_, *depthStencil_); // TODO: needs to work with null

	commandList.setViewport(*viewport_);
}

DrawCommand::ConstantBufferData::ConstantBufferData(
	milk::graphics::ConstantBuffer* constantBuffer,
	std::uint8_t* dataPtr,
	size_t size,
	milk::graphics::ShaderType stage,
	size_t slot
	) :
	constantBuffer(constantBuffer),
	stage(stage),
	slot(slot)
{
	data.reserve(size);
	std::copy(dataPtr, dataPtr + size, std::back_inserter(data));
}
