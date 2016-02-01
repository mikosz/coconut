#ifndef _COCONUT_PULP_FILE_IO_JSONSERIALISER_HPP_
#define _COCONUT_PULP_FILE_IO_JSONSERIALISER_HPP_

#include <iosfwd>

#include "Serialiser.hpp"

namespace coconut {
namespace pulp {
namespace file_io {

class JSONSerialiser : public Serialiser {
public:

	JSONSerialiser(std::ostream& os, Serialiser::Write) : // TODO: needs to remain valid through lifetime of this object - enforce?
		Serialiser(Serialiser::write),
		os_(os)
	{
	}

	JSONSerialiser(std::istream& is, Serialiser::Read) :
		Serialiser(Serialiser::read),
		is_(is)
	{
	}

protected:

	void writeObjectStart() override;

	void writeObjectEnd() override;

	void writeArrayStart(std::uint32_t elementCount) override;

	void writeArrayEnd() override;

	void writeLabel(const std::string& label) override;

	void write(std::uint8_t i) override;

	void write(std::int8_t i) override;

	void write(std::uint16_t i) override;

	void write(std::int16_t i) override;

	void write(std::uint32_t i) override;

	void write(std::int32_t i) override;

	void write(std::uint64_t i) override;

	void write(std::int64_t i) override;

	void write(float f) override;

	void write(const std::string& s) override;

private:

	std::ostream& os_;

	bool newLinePending_ = false;

	size_t indentation_ = 0;

	bool commaPending_ = false;

	void flushComma();

	void indent();

};

} // namespace file_io
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_FILE_IO_JSONSERIALISER_HPP_ */
