#include "Terrain.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::world;

Terrain::Terrain(const milk::FilesystemContext& fs) :
	heightmap_(fs)
{
}
