#include "CommandBuffer.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::renderer;

void CommandBuffer::add(DrawCommandUniquePtr drawCommand) {
	commands_.emplace_back(std::move(drawCommand));
}

void CommandBuffer::submit(milk::graphics::CommandList& commandList) {
	for (auto& command: commands_) { // TODO: obviously do sorting and stuff
		command->submit(commandList);
	}

	commands_.clear();
}
