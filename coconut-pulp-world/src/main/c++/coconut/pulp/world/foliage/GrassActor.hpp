#ifndef _COCONUT_PULP_WORLD_FOLIAGE_GRASS_HPP_
#define _COCONUT_PULP_WORLD_FOLIAGE_GRASS_HPP_

#include "coconut/milk/fs.hpp"

#include "coconut/pulp/renderer/shader/InputElementFactory.hpp"
#include "coconut/pulp/renderer/shader/ParameterFactory.hpp"
#include "coconut/pulp/renderer/shader/ResourceFactory.hpp"
#include "coconut/pulp/renderer/Actor.hpp"
#include "coconut/pulp/math/Vector.hpp"

namespace coconut {
namespace pulp {
namespace world {
namespace foliage {

class GrassActor : public renderer::Actor {
public:

	static void registerShaderInputElements(renderer::shader::InputElementFactory& inputElementFactory);

	static void registerParameters(renderer::shader::ParameterFactory& parameterFactory);

	static void registerResources(renderer::shader::ResourceFactory& resourceFactory);

	static const milk::graphics::Texture& grassPatchPositionsTexture(milk::graphics::Renderer& graphicsRenderer);

	GrassActor(const math::Vec3& patchPosition);

	void update(std::chrono::milliseconds dt) override {
	}

	const math::Vec3& patchPosition() const noexcept {
		return patchPosition_;
	}

private:

	static std::vector<math::Vec3> allPatchPositions_;

	math::Vec3 patchPosition_;

};

} // namespace foliage
} // namespace world
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_WORLD_FOLIAGE_GRASS_HPP_ */
