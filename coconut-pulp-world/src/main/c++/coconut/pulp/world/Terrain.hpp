#ifndef _COCONUT_PULP_WORLD_TERRAIN_HPP_
#define _COCONUT_PULP_WORLD_TERRAIN_HPP_

#include "coconut/milk/fs.hpp"

#include "Heightmap.hpp"

namespace coconut {
namespace pulp {
namespace world {

class Terrain {
public:

	Terrain(const milk::FilesystemContext& fs);

private:

	Heightmap heightmap_;

};

} // namespace world
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_WORLD_TERRAIN_HPP_ */
