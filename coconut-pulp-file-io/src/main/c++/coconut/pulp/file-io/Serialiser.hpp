#ifndef _COCONUT_PULP_FILE_IO_SERIALISER_HPP_
#define _COCONUT_PULP_FILE_IO_SERIALISER_HPP_

#include <string>
#include <vector>
#include <cstddef>

namespace coconut {
namespace pulp {
namespace file_io {

class Serialiser {
public:

	template <class T>
	struct LabelledValue {
	
		LabelledValue(const std::string& label, const T& value) : // value to universal ref or boost::call_traits
			label(label),
			value(value)
		{
		}

		const std::string& label;

		const T& value;

	};

	virtual ~Serialiser() = default;
	
	template <class T>
	static LabelledValue<T> labelled(const std::string& label, const T& value) {
		return LabelledValue<T>(label, value);
	}

	template <class T>
	Serialiser& operator<<(const T& value) {
		writeObjectStart();
		serialise(*this, value);
		writeObjectEnd();
		return *this;
	}

	template <class T>
	Serialiser& operator<<(const LabelledValue<T>& labelledValue) {
		writeLabel(labelledValue.label);
		*this << labelledValue.value;
		return *this;
	}

	template <class T>
	Serialiser& operator<<(const std::vector<T>& vector) {
		// TODO: verify array not larger than max uint32_t
		writeArrayStart(static_cast<std::uint32_t>(vector.size()));
		for (const auto& element : vector) {
			*this << element;
		}
		writeArrayEnd();
		return *this;
	}

	Serialiser& operator<<(std::uint8_t i) {
		write(i);
		return *this;
	}

	Serialiser& operator<<(std::int8_t i) {
		write(i);
		return *this;
	}

	Serialiser& operator<<(std::uint16_t i) {
		write(i);
		return *this;
	}

	Serialiser& operator<<(std::int16_t i) {
		write(i);
		return *this;
	}

	Serialiser& operator<<(std::uint32_t i) {
		write(i);
		return *this;
	}

	Serialiser& operator<<(std::int32_t i) {
		write(i);
		return *this;
	}

	Serialiser& operator<<(std::uint64_t i) {
		write(i);
		return *this;
	}

	Serialiser& operator<<(std::int64_t i) {
		write(i);
		return *this;
	}

	Serialiser& operator<<(float f) {
		write(f);
		return *this;
	}

	Serialiser& operator<<(const char* s) {
		write(std::string(s));
		return *this;
	}

	Serialiser& operator<<(const std::string& s) {
		write(s);
		return *this;
	}

protected:

	Serialiser() = default;

	virtual void writeObjectStart() = 0;

	virtual void writeObjectEnd() = 0;

	virtual void writeArrayStart(std::uint32_t elementCount) = 0;

	virtual void writeArrayEnd() = 0;

	virtual void writeLabel(const std::string& label) = 0;

	virtual void write(std::uint8_t i) = 0;

	virtual void write(std::int8_t i) = 0;

	virtual void write(std::uint16_t i) = 0;

	virtual void write(std::int16_t i) = 0;

	virtual void write(std::uint32_t i) = 0;

	virtual void write(std::int32_t i) = 0;

	virtual void write(std::uint64_t i) = 0;

	virtual void write(std::int64_t i) = 0;

	virtual void write(float f) = 0;

	virtual void write(const std::string& s) = 0;

private:

	Serialiser(const Serialiser&) = delete;

	void operator=(const Serialiser&) = delete;

};

} // namespace file_io
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_FILE_IO_SERIALISER_HPP_ */
