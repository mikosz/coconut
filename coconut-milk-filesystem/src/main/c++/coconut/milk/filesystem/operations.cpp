#include "operations.hpp"

#include <istream>

using namespace coconut;
using namespace coconut::milk;
using namespace coconut::milk::filesystem;

RawData coconut::milk::filesystem::readRawData(const Path& path, std::istream& is) {
	try {
		is.exceptions(std::ios::badbit);

		is.seekg(0u, std::ios::end);
		RawData rawData(static_cast<size_t>(is.tellg()));
		is.seekg(0u, std::ios::beg);
		is.read(reinterpret_cast<char*>(rawData.data()), rawData.size());

		return rawData;
	}
	catch (const std::runtime_error& e) {
		throw FailedToReadData(path, e);
	}
}
