#include "Filesystem.hpp"

using namespace coconut;
using namespace coconut::milk;
using namespace coconut::milk::filesystem;

void Filesystem::mount(Path directory) {
	mounts_.emplace_back(std::move(directory));
}
