#ifndef _COCONUT_PULP_RENDERER_MODEL_HPP_
#define _COCONUT_PULP_RENDERER_MODEL_HPP_

#include <string>
#include <vector>

#include <boost/optional.hpp>

#include "coconut/milk/fs.hpp"

#include "coconut/milk/graphics/Renderer.hpp"
#include "coconut/milk/graphics/RenderState.hpp"
#include "coconut/milk/graphics/IndexBuffer.hpp"
#include "coconut/milk/graphics/VertexBuffer.hpp"
#include "coconut/milk/graphics/PrimitiveTopology.hpp"

#include "coconut/milk/utils/MakePointerDefinitionsMacro.hpp"

#include "coconut/pulp/mesh/Mesh.hpp"
#include "coconut/pulp/mesh/MaterialConfiguration.hpp"

#include "shader/Input.hpp"
#include "shader/PassFactory.hpp"
#include "PassContext.hpp"
#include "CommandBuffer.hpp"
#include "Material.hpp"

namespace coconut {
namespace pulp {
namespace renderer {

class CommandBuffer;

class Model {
public:

	Model(
		std::string id,
		Mesh mesh,
		milk::graphics::Renderer& graphicsRenderer,
		shader::PassFactory& passFactory,
		const milk::FilesystemContext& filesystemContext
		);

	void render(CommandBuffer& commandBuffer, PassContext PassContext); // TODO: make const

	const std::string& id() const {
		return id_;
	}

	// TODO: TEMP TEMP TEMP!
	Material& material() {
		return drawGroups_.front().material;
	}

private:

	struct DrawGroup {
	public:
	
		milk::graphics::VertexBuffer vertexBuffer;

		milk::graphics::IndexBuffer indexBuffer;

		milk::graphics::VertexBuffer instanceDataBuffer;

		size_t indexCount;

		milk::graphics::PrimitiveTopology primitiveTopology;

		Material material;

		DrawGroup(
			milk::graphics::Renderer& graphicsRenderer,
			shader::PassFactory& passFactory,
			const milk::fs::FilesystemContext& filesystemContext,
			mesh::Mesh::Submeshes::iterator submeshIt,
			mesh::Mesh::Submeshes::iterator submeshEnd,
			const mesh::MaterialConfiguration& materialConfiguration
			);

		void render(CommandBuffer& commandBuffer, PassContext passContext);

	};

	using DrawGroups = std::vector<DrawGroup>;

	std::string id_;

	DrawGroups drawGroups_;

};

CCN_MAKE_POINTER_DEFINITIONS(Model);

} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_MODEL_HPP_ */
