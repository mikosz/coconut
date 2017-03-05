#include "Model.hpp"

#include <algorithm>

#include "coconut/milk/graphics/Texture2d.hpp"
#include "coconut/milk/graphics/Sampler.hpp"
#include "coconut/milk/graphics/ImageLoader.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::renderer;

Model::Model(
	Mesh mesh,
	milk::graphics::Renderer& graphicsRenderer,
	const shader::Input& input,
	const milk::FilesystemContext& filesystemContext
	) :
	materials_(std::move(mesh.materials()))
{
	std::sort(materials_.begin(), materials_.end(), [](const auto& lhs, const auto& rhs) {
			return lhs.materialId < rhs.materialId;
		});

	std::transform(
		mesh.submeshes().begin(),
		mesh.submeshes().end(),
		[](const auto& submesh) {
			return std::make_shared<DrawGroup>(graphicsRenderer, input, submesh);
		});
}

void Model::render(CommandBuffer& commandBuffer, PassContext PassContext) {
	PassContext.model = this;

	for (auto drawGroup : drawGroups_) {
		drawGroup->render(commandBuffer, PassContext);
	}
}
