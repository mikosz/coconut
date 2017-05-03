#ifndef _COCONUT_PULP_WORLD_FOLIAGE_GRASS_HPP_
#define _COCONUT_PULP_WORLD_FOLIAGE_GRASS_HPP_

#include "coconut/milk/fs.hpp"

#include "coconut/pulp/renderer/shader/Property.hpp"
#include "coconut/pulp/renderer/shader/InputElementFactory.hpp"
#include "coconut/pulp/renderer/ModelFactory.hpp"
#include "coconut/pulp/renderer/Actor.hpp"
#include "coconut/pulp/math/Vector.hpp"

namespace coconut {
namespace pulp {
namespace world {

class Heightmap;

namespace foliage {

// TODO: fix PropertyHolder to not have to be a superclass
class GrassActor : public renderer::Actor {
public:

	static void registerShaderInputElements(renderer::shader::InputElementFactory& inputElementFactory);

	static void registerModels(renderer::ModelFactory& modelFactory, const Heightmap& heightmap);

	GrassActor(const math::Vec3& patchPosition);

	void update(std::chrono::milliseconds dt) override {
	}

	void bindShaderProperties(
		renderer::shader::Properties& properties,
		std::string objectId
		) const override;

	const math::Vec3& patchPosition() const noexcept { // TODO: could just use Actors position... O__o
		return patchPosition_;
	}

private:

	math::Vec3 patchPosition_;

};

} // namespace foliage
} // namespace world
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_WORLD_FOLIAGE_GRASS_HPP_ */
