#ifndef _COCONUT_PULP_FILE_IO_DESERIALISER_HPP_
#define _COCONUT_PULP_FILE_IO_DESERIALISER_HPP_

#include <string>
#include <vector>
#include <cstddef>

#include "coconut/milk/math/Vector.hpp"

namespace coconut {
namespace pulp {
namespace file_io {

class Deserialiser {
public:

	template <class T>
	using SerialiseArgument = T&;

	struct Label {

		std::string label;

		Label(std::string label) :
			label(std::move(label))
		{
		}

	};

	virtual ~Deserialiser() = default;

	template <class T>
	Deserialiser& operator>>(T& value) {
		readObjectStart();
		serialise(*this, value);
		readObjectEnd();
		return *this;
	}

	template <class T>
	Deserialiser& operator>>(std::vector<T>& vector) {
		auto arrayElementsCount = readArrayStart();
		vector.reserve(arrayElementsCount);
		T element;
		for (auto arrayElementsLeft = arrayElementsCount; arrayElementsLeft > 0; --arrayElementsLeft) {
			*this >> element;
			vector.emplace_back(std::move(element));
		}
		readArrayEnd();
		return *this;
	}

	Deserialiser& operator>>(Label label) {
		readLabel(std::move(label.label));
		return *this;
	}

	// TODO: make this a template accepting all integral types
	Deserialiser& operator>>(std::uint8_t& i) {
		read(i);
		return *this;
	}

	Deserialiser& operator>>(std::int8_t& i) {
		read(i);
		return *this;
	}

	Deserialiser& operator>>(std::uint16_t& i) {
		read(i);
		return *this;
	}

	Deserialiser& operator>>(std::int16_t& i) {
		read(i);
		return *this;
	}

	Deserialiser& operator>>(std::uint32_t& i) {
		read(i);
		return *this;
	}

	Deserialiser& operator>>(std::int32_t& i) {
		read(i);
		return *this;
	}

	Deserialiser& operator>>(std::uint64_t& i) {
		read(i);
		return *this;
	}

	Deserialiser& operator>>(std::int64_t& i) {
		read(i);
		return *this;
	}

	Deserialiser& operator>>(float& f) {
		read(f);
		return *this;
	}

	Deserialiser& operator>>(std::string& s) {
		read(s);
		return *this;
	}

	template <class T>
	Deserialiser& operator()(T& value) {
		*this >> value;
		return *this;
	}

	template <class T>
	Deserialiser& operator()(const Label& label, T& value) {
		*this >> label >> value;
		return *this;
	}

protected:

	Deserialiser() = default;

	virtual void readObjectStart() = 0;

	virtual void readObjectEnd() = 0;

	virtual std::uint32_t readArrayStart() = 0;

	virtual void readArrayEnd() = 0;

	virtual void readLabel(std::string label) = 0;

	virtual void read(std::uint8_t& i) = 0;

	virtual void read(std::int8_t& i) = 0;

	virtual void read(std::uint16_t& i) = 0;

	virtual void read(std::int16_t& i) = 0;

	virtual void read(std::uint32_t& i) = 0;

	virtual void read(std::int32_t& i) = 0;

	virtual void read(std::uint64_t& i) = 0;

	virtual void read(std::int64_t& i) = 0;

	virtual void read(float& f) = 0;

	virtual void read(std::string& s) = 0; // TODO: avoid reading extra-long strings in error

private:

	Deserialiser(const Deserialiser&) = delete;

	void operator=(const Deserialiser&) = delete;

};

void serialise(Deserialiser& deserialiser, milk::math::Vector2d& vector);
void serialise(Deserialiser& deserialiser, milk::math::Vector3d& vector);
void serialise(Deserialiser& deserialiser, milk::math::Vector4d& vector);

template <class EnumType>
inline void serialise(file_io::Deserialiser& deserialiser, EnumType& enumValue, std::enable_if_t<std::is_enum<EnumType>::value>* = nullptr) {
	std::string name;
	deserialiser >> name;
	fromString(enumValue, name);
}

} // namespace file_io
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_FILE_IO_DESERIALISER_HPP_ */
