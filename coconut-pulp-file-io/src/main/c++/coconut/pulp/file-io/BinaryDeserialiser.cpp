#include "BinaryDeserialiser.hpp"

#include <cstddef>
#include <iostream>

#include "coconut-tools/logger.hpp"

#include "coconut/milk/utils/endianness.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::file_io;

namespace /* anonymous */ {

CT_LOGGER_CATEGORY("COCONUT.PULP.FILE_IO");

template <class T>
void readBinaryData(std::istream& is, T& value, bool changeEndianness) { // TODO: move to tools, also this should only accept POD values
	auto* ptr = reinterpret_cast<char*>(&value);
	is.read(ptr, sizeof(value));
	if (changeEndianness) {
		value = milk::utils::changeEndianness(value);
	}
}

} // anonymous namespace

BinaryDeserialiser::BinaryDeserialiser(std::istream& is) :
	is_(is)
{
	is_.exceptions(std::ios::badbit | std::ios::failbit);

	char dataEndianBig;
	readBinaryData(is_, dataEndianBig, false);
	changeEndianness_ = (dataEndianBig != static_cast<char>(milk::utils::isBigEndian()));
}

void BinaryDeserialiser::readObjectStart() {
}

void BinaryDeserialiser::readObjectEnd() {
}

std::uint32_t BinaryDeserialiser::readArrayStart() {
	std::uint32_t elementCount;
	readBinaryData(is_, elementCount, changeEndianness_);
	return elementCount;
}

void BinaryDeserialiser::readArrayEnd() {
}

void BinaryDeserialiser::readLabel(std::string /* label */) {
}

void BinaryDeserialiser::read(bool& b) {
	readBinaryData(is_, b, changeEndianness_);
}

void BinaryDeserialiser::read(std::uint8_t& i) {
	readBinaryData(is_, i, changeEndianness_);
}

void BinaryDeserialiser::read(std::int8_t& i) {
	readBinaryData(is_, i, changeEndianness_);
}

void BinaryDeserialiser::read(std::uint16_t& i) {
	readBinaryData(is_, i, changeEndianness_);
}

void BinaryDeserialiser::read(std::int16_t& i) {
	readBinaryData(is_, i, changeEndianness_);
}

void BinaryDeserialiser::read(std::uint32_t& i) {
	readBinaryData(is_, i, changeEndianness_);
}

void BinaryDeserialiser::read(std::int32_t& i) {
	readBinaryData(is_, i, changeEndianness_);
}

void BinaryDeserialiser::read(std::uint64_t& i) {
	readBinaryData(is_, i, changeEndianness_);
}

void BinaryDeserialiser::read(std::int64_t& i) {
	readBinaryData(is_, i, changeEndianness_);
}

void BinaryDeserialiser::read(float& f) {
	readBinaryData(is_, f, false);
}

void BinaryDeserialiser::read(std::string& s) {
	std::getline(is_, s, '\0');
}
