#include "ModelFactory.hpp"

#include <coconut-tools/serialisation/BinaryDeserialiser.hpp>
#include <coconut-tools/logger.hpp>

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::renderer;

namespace /* anonymous */ {

CT_LOGGER_CATEGORY("COCONUT.PULP.RENDERER.MODEL_FACTORY");

} // anonymous namespace

void detail::ModelCreator::scanSourceDirectory(
	const milk::fs::FilesystemContext& filesystemContext,
	const milk::fs::Path& directory
	)
{
	CT_LOG_INFO << "Scanning model source directory: " << directory;
	
	const auto names = filesystemContext.list(directory);

	for (const auto& name : names) {
		const auto path = directory / name;
		if (path.extension() == ".obj") {
			assert(!"unimplemented");
		}
	}
}

void detail::ModelCreator::scanModelDirectory(
	const milk::fs::FilesystemContext& filesystemContext,
	const milk::fs::Path& directory
	)
{
	CT_LOG_INFO << "Scanning model directory: " << directory;
	
	const auto names = filesystemContext.list(directory);

	for (const auto& name : names) {
		const auto path = directory / name;
		if (path.extension() == ".model") {
			modelFiles_.emplace(path.stem().string(), filesystemContext.makeAbsolute(path));
		}
	}
}

auto detail::ModelCreator::doCreate(
	const std::string& id,
	milk::graphics::Renderer& graphicsRenderer,
	shader::PassFactory& passFactory,
	const milk::fs::FilesystemContext& filesystemContext
	) -> Instance
{
	if (modelFiles_.count(id)) {
		auto modelIS = filesystemContext.open(modelFiles_[id]);
		coconut_tools::serialisation::BinaryDeserialiser deserialiser(*modelIS);

		auto modelData = pulp::Mesh();
		deserialiser >> modelData;

		return std::make_unique<pulp::renderer::Model>(
			std::move(modelData),
			graphicsRenderer,
			passFactory,
			filesystemContext
			);
	} else {
		throw coconut_tools::factory::error_policy::NoSuchType<std::string>(id);
	}
}
