#ifndef _COCONUT_PULP_WORLD_HEIGHTMAP_HPP_
#define _COCONUT_PULP_WORLD_HEIGHTMAP_HPP_

#include <vector>
#include <tuple>

#include "coconut/milk/graphics/Renderer.hpp"
#include "coconut/milk/graphics/ShaderResourceView.hpp"
#include "coconut/milk/graphics/Sampler.hpp"
#include "coconut/milk/fs.hpp"
#include "coconut/pulp/math/Vector.hpp"

namespace coconut {
namespace pulp {
namespace world {

class Heightmap {
public:

	Heightmap(milk::graphics::Renderer& graphicsRenderer, const milk::FilesystemContext& fs);

	float heightAt(float x, float z) const;

	// TODO: extent type
	std::tuple<size_t, size_t> cellCount() const noexcept {
		return std::make_tuple(columnCount_, cellHeights_.size() / columnCount_);
	}

	float width() const noexcept {
		return cellEdgeLength_ * columnCount_;
	}

	float depth() const noexcept {
		return cellEdgeLength_ * (cellHeights_.size() / columnCount_);
	}

	float cellEdgeLength() const noexcept {
		return cellEdgeLength_;
	}

	const milk::graphics::ShaderResourceView& texture() const noexcept {
		return textureSRV_;
	}

	// TODO: temp
	const milk::graphics::Sampler& sampler() const noexcept {
		return sampler_;
	}

private:

	float cellEdgeLength_;

	size_t columnCount_;

	std::vector<float> cellHeights_;

	milk::graphics::ShaderResourceView textureSRV_;

	milk::graphics::Sampler sampler_;

};

} // namespace world
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_WORLD_HEIGHTMAP_HPP_ */
