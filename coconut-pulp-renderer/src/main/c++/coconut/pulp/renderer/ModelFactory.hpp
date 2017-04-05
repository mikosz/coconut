#ifndef _COCONUT_PULP_RENDERER_MODELFACTORY_HPP_
#define _COCONUT_PULP_RENDERER_MODELFACTORY_HPP_

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

	void scanSourceDirectory(
		const milk::fs::FilesystemContext& filesystemContext,
		const milk::fs::Path& directory
		);

	void scanModelDirectory(
		const milk::fs::FilesystemContext& filesystemContext,
		const milk::fs::Path& directory
		);

protected:

	Instance doCreate(
		const std::string& id,
		milk::graphics::Renderer& graphicsRenderer,
		shader::PassFactory& passFactory,
		const milk::fs::FilesystemContext& filesystemContext
	);

private:

	using ModelFiles = std::unordered_map<std::string, milk::AbsolutePath>;

	ModelFiles modelFiles_;

};

} // namespace detail

using PassFactory = 
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
