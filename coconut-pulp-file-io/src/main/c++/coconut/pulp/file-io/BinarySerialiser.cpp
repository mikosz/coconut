#include "BinarySerialiser.hpp"

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
void writeBinaryData(std::ostream& os, const T& value) { // TODO: move to tools, also this should only accept POD values
	const auto* ptr = reinterpret_cast<const char*>(&value);
	os.write(ptr, sizeof(value));
}

} // anonymous namespace

BinarySerialiser::BinarySerialiser(std::ostream& os) :
	os_(os)
{
	os_.exceptions(std::ios::badbit | std::ios::failbit);

	writeBinaryData(os_, static_cast<char>(milk::utils::isBigEndian()));
}

void BinarySerialiser::writeObjectStart() {
}

void BinarySerialiser::writeObjectEnd() {
}

void BinarySerialiser::writeArrayStart(std::uint32_t elementCount) {
	writeBinaryData(os_, elementCount);
}

void BinarySerialiser::writeArrayEnd() {
}

void BinarySerialiser::writeLabel(const std::string& label) {
}

void BinarySerialiser::write(bool b) {
	writeBinaryData(os_, b);
}

void BinarySerialiser::write(std::uint8_t i) {
	writeBinaryData(os_, i);
}

void BinarySerialiser::write(std::int8_t i) {
	writeBinaryData(os_, i);
}

void BinarySerialiser::write(std::uint16_t i) {
	writeBinaryData(os_, i);
}

void BinarySerialiser::write(std::int16_t i) {
	writeBinaryData(os_, i);
}

void BinarySerialiser::write(std::uint32_t i) {
	writeBinaryData(os_, i);
}

void BinarySerialiser::write(std::int32_t i) {
	writeBinaryData(os_, i);
}

void BinarySerialiser::write(std::uint64_t i) {
	writeBinaryData(os_, i);
}

void BinarySerialiser::write(std::int64_t i) {
	writeBinaryData(os_, i);
}

void BinarySerialiser::write(float f) {
	writeBinaryData(os_, f);
}

void BinarySerialiser::write(const std::string& s) {
	os_.write(s.c_str(), s.length());
	writeBinaryData(os_, '\0');
}
