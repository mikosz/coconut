#ifndef _COCONUT_PULP_WORLD_WINDMAP_HPP_
#define _COCONUT_PULP_WORLD_WINDMAP_HPP_

#include <chrono>
#include <vector>

#include "coconut/milk/graphics/Renderer.hpp"
#include "coconut/milk/graphics/Texture2d.hpp"
#include "coconut/milk/graphics/Sampler.hpp"
#include "coconut/pulp/renderer/shader/ReflectiveObject.hpp"
#include "coconut/pulp/math/Vector.hpp"

namespace coconut {
namespace pulp {
namespace world {

class Windmap {
public:

	Windmap(milk::graphics::Renderer& graphicsRenderer, size_t width, size_t height);

	void update(
		std::chrono::milliseconds dt,
		milk::graphics::Renderer& graphicsRenderer
		);

private:

	size_t width_;

	size_t height_;

	Vec2 globalWind_;

	std::vector<Vec2> map_;

	std::vector<Vec2> updateMap_;

	milk::graphics::Texture2d texture_;

	milk::graphics::Sampler sampler_;

	friend class renderer::shader::ReflectiveInterface<Windmap>;

};

} // namespace world
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_WORLD_WINDMAP_HPP_ */
