#ifndef _COCONUT_PULP_RENDERER_MODEL_HPP_
#define _COCONUT_PULP_RENDERER_MODEL_HPP_

#include <vector>

#include <boost/optional.hpp>

#include "coconut/milk/fs.hpp"

#include "coconut/milk/graphics/Renderer.hpp"
#include "coconut/milk/graphics/IndexBuffer.hpp"
#include "coconut/milk/graphics/VertexBuffer.hpp"
#include "coconut/milk/graphics/PrimitiveTopology.hpp"

#include "coconut/milk/utils/MakePointerDefinitionsMacro.hpp"

#include "coconut/pulp/mesh/Mesh.hpp"

#include "shader/Input.hpp"
#include "PassContext.hpp"
#include "CommandBuffer.hpp"

namespace coconut {
namespace pulp {
namespace renderer {

class CommandBuffer;

class Model {
public:

	Model(
		Mesh mesh,
		milk::graphics::Renderer& graphicsRenderer,
		const shader::Input& input,
		const milk::FilesystemContext& filesystemContext
		);

	void render(CommandBuffer& commandBuffer, PassContext PassContext);

private:

	struct DrawGroup {
	public:
	
		milk::graphics::VertexBuffer vertexBuffer;

		boost::optional<milk::graphics::VertexBuffer> instanceDataBuffer;

		size_t instanceCount;

		boost::optional<milk::graphics::IndexBuffer> indexBuffer;

		size_t indexCount;

		milk::graphics::PrimitiveTopology primitiveTopology;

		mesh::Material material;

		DrawGroup(
			milk::graphics::Renderer& graphicsRenderer,
			mesh::Mesh::Submeshes::iterator submeshIt,
			mesh::Mesh::Submeshes::iterator submeshEnd
			);

		void render(CommandBuffer& commandBuffer, PassContext passContext);

	};

	using DrawGroups = std::vector<DrawGroup>;

	DrawGroups drawGroups_;

};

CCN_MAKE_POINTER_DEFINITIONS(Model);

} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_MODEL_HPP_ */
