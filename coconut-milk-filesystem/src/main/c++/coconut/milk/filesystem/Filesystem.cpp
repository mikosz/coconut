#include "Filesystem.hpp"

#include <istream>

using namespace coconut;
using namespace coconut::milk;
using namespace coconut::milk::filesystem;

void Filesystem::mount(Path directory) {
	mounts_.emplace_back(std::move(directory));
}

std::shared_ptr<const RawData> coconut::milk::filesystem::readRawData(const Path& path, const IStream& is) {
	try {
		is->exceptions(std::ios::badbit);

		is->seekg(0u, std::ios::end);
		auto rawData = std::make_shared<RawData>(static_cast<size_t>(is->tellg()));
		is->seekg(0u, std::ios::beg);
		is->read(reinterpret_cast<char*>(rawData->data()), rawData->size());

		return rawData;
	} catch (const std::runtime_error& e) {
		throw FailedToReadData(path, e);
	}
}
