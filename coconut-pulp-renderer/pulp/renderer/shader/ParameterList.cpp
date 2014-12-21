#include "ParameterList.hpp"

#include <numeric>

#include "ParameterWriter.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::renderer;
using namespace coconut::pulp::renderer::shader;

using milk::graphics::Buffer;

void ParameterList::add(milk::graphics::Device& device, const BufferDescription& bufferDescription) {
	Buffer::Configuration configuration;
	configuration.allowCPURead = false;
	configuration.allowGPUWrite = false;
	configuration.allowModifications = true;
	configuration.purpose = Buffer::CreationPurpose::CONSTANT_BUFFER;
	configuration.size = std::accumulate(
		bufferDescription.parameterIds.begin(),
		bufferDescription.parameterIds.end(),
		0,
		[](size_t sum, ParameterId id) {
			return sum + size(id);
		}
		);
	configuration.stride = 0;

	milk::graphics::BufferSharedPtr buffer(new milk::graphics::Buffer(device, configuration));

	bufferDescriptions_.push_back(bufferDescription);
	buffers_.push_back(buffer);
}

void ParameterList::bind(
	milk::graphics::Device& device,
	ParameterWriter& writer,
	milk::graphics::Buffer::ShaderType shaderType
	) {
	for (size_t bufferIndex = 0; bufferIndex < buffers_.size(); ++bufferIndex) {
		{
			Buffer::LockedData locked = buffers_[bufferIndex]->lock(device, Buffer::LockPurpose::WRITE_DISCARD);
			void* data = locked.data;		
			for (auto parameterId : bufferDescriptions_[bufferIndex].parameterIds) {
				data = writer.write(data, parameterId);
			}
		}

		buffers_[bufferIndex]->bind(device, shaderType, bufferIndex);
	}
}
