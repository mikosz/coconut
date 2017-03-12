#ifndef _COCONUT_PULP_RENDERER_PASSCONTEXT_HPP_
#define _COCONUT_PULP_RENDERER_PASSCONTEXT_HPP_

#include <string>

#include "coconut/milk/graphics/Texture2d.hpp"
#include "coconut/milk/graphics/Viewport.hpp"

#include "coconut/pulp/mesh/MaterialConfiguration.hpp"

#include "shader/Pass.hpp"

namespace coconut {
namespace pulp {
namespace renderer {

class Scene;
class Actor;
class Model;
class Material;

// TODO: I don't like these pointers, move fields to private, stop passing this as value
class PassContext {
public:

	milk::graphics::Viewport* viewport = nullptr;

	milk::graphics::Texture2d* backBuffer = nullptr;

	milk::graphics::Texture2d* screenDepthStencil = nullptr;

	const Scene* scene = nullptr;

	const Actor* actor = nullptr;

	const Model* model = nullptr;

	const Material* material = nullptr;

	std::string shader;

	mesh::MaterialConfiguration::PassType passType; // TODO: I don't like this

	void reset() {
		viewport = nullptr;
		backBuffer = nullptr;
		screenDepthStencil = nullptr;
		scene = nullptr;
		actor = nullptr;
		model = nullptr;
		material = nullptr;
		shader.clear();
	}

};

} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_PASSCONTEXT_HPP_ */
