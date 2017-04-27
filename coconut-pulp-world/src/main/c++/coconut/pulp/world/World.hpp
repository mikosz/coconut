#ifndef _COCONUT_PULP_WORLD_WORLD_HPP_
#define _COCONUT_PULP_WORLD_WORLD_HPP_

#include "coconut/milk/fs.hpp"

#include "Terrain.hpp"

namespace coconut {
namespace pulp {
namespace world {

class World {
public:

	World(const coconut::milk::FilesystemContext& fs) :
		terrain_(fs)
	{
	}

private:

	Terrain terrain_;

};

} // namespace world
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_WORLD_WORLD_HPP_ */
