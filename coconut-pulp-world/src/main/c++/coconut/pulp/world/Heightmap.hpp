#ifndef _COCONUT_PULP_WORLD_HEIGHTMAP_HPP_
#define _COCONUT_PULP_WORLD_HEIGHTMAP_HPP_

#include <vector>

#include "coconut/milk/graphics/Texture2d.hpp"
#include "coconut/milk/fs.hpp"

namespace coconut {
namespace pulp {
namespace world {

class Heightmap {
public:

	Heightmap(const milk::FilesystemContext& fs);

private:

	float patchScale_;

	size_t rowPitch_;

	std::vector<float> data_;

	milk::graphics::Texture2d texture_;

};

} // namespace world
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_WORLD_HEIGHTMAP_HPP_ */
