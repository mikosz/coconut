#include "JSONSerialiser.hpp"

#include <cassert>

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::file_io;

void JSONSerialiser::writeObjectStart() {
	flushComma();
	indent();
	os_ << '{';
	++indentation_;
	newLinePending_ = true;
}

void JSONSerialiser::writeObjectEnd() {
	assert(indentation_ > 0);
	--indentation_;
	indent();
	os_ << '}';
	commaPending_ = true;
	newLinePending_ = true;
}

void JSONSerialiser::writeArrayStart(std::uint32_t /* elementCount */) {
	flushComma();
	indent();
	os_ << '[';
	++indentation_;
	newLinePending_ = true;
}

void JSONSerialiser::writeArrayEnd() {
	assert(indentation_ > 0);
	--indentation_;
	indent();
	os_ << ']';
	commaPending_ = true;
	newLinePending_ = true;
}

void JSONSerialiser::writeLabel(const std::string& label) {
	flushComma();
	indent();
	os_ << '"' << label << "\": ";
}

void JSONSerialiser::write(std::uint8_t i) {
	os_ << i;
	commaPending_ = true;
	newLinePending_ = true;
}

void JSONSerialiser::write(std::int8_t i) {
	os_ << i;
	commaPending_ = true;
	newLinePending_ = true;
}

void JSONSerialiser::write(std::uint16_t i) {
	os_ << i;
	commaPending_ = true;
	newLinePending_ = true;
}

void JSONSerialiser::write(std::int16_t i) {
	os_ << i;
	commaPending_ = true;
	newLinePending_ = true;
}

void JSONSerialiser::write(std::uint32_t i) {
	os_ << i;
	commaPending_ = true;
	newLinePending_ = true;
}

void JSONSerialiser::write(std::int32_t i) {
	os_ << i;
	commaPending_ = true;
	newLinePending_ = true;
}

void JSONSerialiser::write(std::uint64_t i) {
	os_ << i;
	commaPending_ = true;
	newLinePending_ = true;
}

void JSONSerialiser::write(std::int64_t i) {
	os_ << i;
	commaPending_ = true;
	newLinePending_ = true;
}

void JSONSerialiser::write(float f) {
	os_ << f;
	commaPending_ = true;
	newLinePending_ = true;
}

void JSONSerialiser::write(const std::string& s) {
	os_ << '"' << s << '"';
	commaPending_ = true;
	newLinePending_ = true;
}

void JSONSerialiser::flushComma() {
	if (commaPending_) {
		os_ << ",";
		commaPending_ = false;
	}
}

void JSONSerialiser::indent() {
	if (newLinePending_) {
		os_ << '\n';
		newLinePending_ = false;
		for (size_t i = indentation_; i > 0; --i) {
			os_ << '\t';
		}
	}
}
