#include "ModelFactory.hpp"

#include <coconut-tools/serialisation/BinaryDeserialiser.hpp>
#include <coconut-tools/logger.hpp>

#include "coconut/pulp/mesh/obj/Importer.hpp"

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
			CT_LOG_INFO << "Found " << path;
			sourceFiles_.emplace(path.stem().string(), filesystemContext.makeAbsolute(path));
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
			CT_LOG_INFO << "Found " << path;
			modelFiles_.emplace(path.stem().string(), filesystemContext.makeAbsolute(path));
		}
	}
}

bool detail::ModelCreator::hasGenerator(const std::string& id) const noexcept {
	return generators_.count(id) != 0;
}

void detail::ModelCreator::registerGenerator(std::string id, Generator generator) {
	generators_.emplace(std::move(id), std::move(generator));
}

auto detail::ModelCreator::doCreate(
	const std::string& id,
	milk::graphics::Renderer& graphicsRenderer,
	shader::PassFactory& passFactory,
	const milk::fs::FilesystemContext& filesystemContext
	) -> Instance
{
	if (hasGenerator(id)) {
		return generators_[id](id, graphicsRenderer, passFactory, filesystemContext);
	} else if (modelFiles_.count(id)) {
		auto modelIS = filesystemContext.open(modelFiles_[id]);
		coconut_tools::serialisation::BinaryDeserialiser deserialiser(*modelIS);

		auto mesh = pulp::Mesh();
		deserialiser >> mesh;

		return std::make_unique<pulp::renderer::Model>(
			id,
			std::move(mesh),
			graphicsRenderer,
			passFactory,
			filesystemContext
			);
	} else if (sourceFiles_.count(id)) {
		mesh::obj::Importer importer;
		const auto path = sourceFiles_[id];
		if (path.extension() == ".obj") {
			auto directoryContext = filesystemContext; // TODO: need a constructor that creates a context off another context
			directoryContext.changeWorkingDirectory(path.parent());
			return std::make_unique<pulp::renderer::Model>(
				id,
				importer.import(*filesystemContext.load(path), directoryContext),
				graphicsRenderer,
				passFactory,
				filesystemContext
				);
		} else {
			assert(false);
			throw "aaa"; // TODO
		}
	} else {
		throw coconut_tools::factory::error_policy::NoSuchType<std::string>(id);
	}
}
