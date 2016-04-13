#include "DrawCommand.hpp"

#include <cassert>
#include <algorithm>
#include <iterator>

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::renderer;

void DrawCommand::run(milk::graphics::CommandList& commandList) {
	assert(rasteriser_ != nullptr);

	commandList.setRasteriser(*rasteriser_);
	
	for (auto& sampler: samplers_) {
		commandList.setSampler(*sampler.sampler, sampler.stage, sampler.slot);
	}

	for (auto& constantBufferData: constantBuffersData_) {
		auto lockedData = commandList.lock(
			*constantBufferData.constantBuffer, milk::graphics::CommandList::LockPurpose::WRITE_DISCARD);
		std::copy(constantBufferData.data.begin(), constantBufferData.data.end(), lockedData.get());
	}

	commandList.setVertexShader(*vertexShader_);
	commandList.setPixelShader(*pixelShader_);
}

DrawCommand::ConstantBufferData::ConstantBufferData(
	std::uint8_t* dataPtr, size_t size, milk::graphics::ShaderType stage, size_t slot)
	:
	data(size),
	stage(stage),
	slot(slot)
{
	std::copy(dataPtr, dataPtr + size, std::back_inserter(data));
}
