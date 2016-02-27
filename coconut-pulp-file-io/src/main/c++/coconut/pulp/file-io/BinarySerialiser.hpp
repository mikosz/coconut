#ifndef _COCONUT_PULP_FILE_IO_BINARYSERIALISER_HPP_
#define _COCONUT_PULP_FILE_IO_BINARYSERIALISER_HPP_

#include <iosfwd>

#include "Serialiser.hpp"

namespace coconut {
namespace pulp {
namespace file_io {

class BinarySerialiser : public Serialiser {
public:

	BinarySerialiser(std::ostream& os); // TODO: needs to remain valid through lifetime of this object - enforce?
		
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

};

} // namespace file_io
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_FILE_IO_BINARYSERIALISER_HPP_ */
