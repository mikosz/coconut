#ifndef _COCONUT_PULP_WORLD_WINDMAP_HPP_
#define _COCONUT_PULP_WORLD_WINDMAP_HPP_

#include <chrono>

#include "coconut/milk/graphics/Renderer.hpp"
#include "coconut/milk/graphics/Texture1d.hpp"
#include "coconut/milk/graphics/Sampler.hpp"
#include "coconut/pulp/renderer/shader/ReflectiveObject.hpp"
#include "coconut/pulp/math/Vector.hpp"
#include "coconut/pulp/math/PerlinNoise.hpp"

namespace coconut {
namespace pulp {
namespace world {

class Windmap {
public:

	Windmap(milk::graphics::Renderer& graphicsRenderer);

	void update(milk::graphics::Renderer& graphicsRenderer, std::chrono::milliseconds dt);

private:

	pulp::math::PerlinNoise perlin_;

	Vec2 windDir_;

	float baseIntensity_;

	float intensityAmplitude_;

	float textureOffset_;

	milk::graphics::Texture1d powerTexture_;

	milk::graphics::Texture1d secondaryPowerTexture_;

	milk::graphics::Sampler sampler_;

	friend class renderer::shader::ReflectiveInterface<Windmap>;

};

} // namespace world
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_WORLD_WINDMAP_HPP_ */
