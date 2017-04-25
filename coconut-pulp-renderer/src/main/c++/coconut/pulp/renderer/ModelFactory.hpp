#ifndef _COCONUT_PULP_RENDERER_MODELFACTORY_HPP_
#define _COCONUT_PULP_RENDERER_MODELFACTORY_HPP_

#include <functional>
#include <string>
#include <unordered_map>

#include <coconut-tools/factory.hpp>

#include "coconut/milk/fs.hpp"

#include "shader/PassFactory.hpp"
#include "Model.hpp"

namespace coconut {
namespace pulp {
namespace renderer {

namespace detail {

class ModelCreator {
public:

	using Instance = std::unique_ptr<Model>;

	using Generator = std::function<
		Instance (
			const std::string& id,
			milk::graphics::Renderer& graphicsRenderer,
			shader::PassFactory& passFactory,
			const milk::fs::FilesystemContext& filesystemContext
			)
		>;

	void scanSourceDirectory(
		const milk::fs::FilesystemContext& filesystemContext,
		const milk::fs::Path& directory
		);

	void scanModelDirectory(
		const milk::fs::FilesystemContext& filesystemContext,
		const milk::fs::Path& directory
		);

	bool hasGenerator(const std::string& id) const noexcept;

	void registerGenerator(std::string id, Generator generator);

protected:

	Instance doCreate(
		const std::string& id,
		milk::graphics::Renderer& graphicsRenderer,
		shader::PassFactory& passFactory,
		const milk::fs::FilesystemContext& filesystemContext
	);

private:

	using SourceFiles = std::unordered_map<std::string, milk::AbsolutePath>;

	using ModelFiles = std::unordered_map<std::string, milk::AbsolutePath>;

	using Generators = std::unordered_map<std::string, Generator>;

	SourceFiles sourceFiles_;

	ModelFiles modelFiles_;

	Generators generators_;

};

} // namespace detail

using ModelFactory = 
	coconut_tools::Factory<
		std::string,
		detail::ModelCreator,
		coconut_tools::factory::storage::Volatile,
		std::mutex
		>;

} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_MODELFACTORY_HPP_ */
