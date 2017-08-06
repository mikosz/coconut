#include "Primitive.hpp"

#include <cassert>

#include <coconut-tools/enums.hpp>
#include <coconut-tools/exceptions/RuntimeError.hpp>

#include "coconut/milk/utils/integralValue.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::primitive;

namespace /* anonymous */ {

CT_ENUM(
	PrimitiveDataType,
	(SCALAR)
	(POSITION)
	(VECTOR)
	(COLOUR)
	(TEXTURE_COORDINATE)
	);

class StoreAsVisitor : public boost::static_visitor<void> {
public:

	StoreAsVisitor(void* buffer, milk::graphics::PixelFormat pixelFormat) :
		buffer_(buffer),
		pixelFormat_(pixelFormat)
	{
	}

	template <class T>
	void operator()(const T& value) const {
		value.storeAs(buffer_, pixelFormat_);
	}

private:

	void* buffer_;

	milk::graphics::PixelFormat pixelFormat_;

};

class SerialiseVisitor : public boost::static_visitor<void> {
public:

	SerialiseVisitor(coconut_tools::Serialiser& serialiser) :
		serialiser_(serialiser)
	{
	}

	void operator()(const Scalar& value) const {
		serialiser_(coconut_tools::Serialiser::Label("type"), PrimitiveDataType::SCALAR);
		serialiser_(coconut_tools::Serialiser::Label("value"), value);
	}

	void operator()(const Position& value) const {
		serialiser_(coconut_tools::Serialiser::Label("type"), PrimitiveDataType::POSITION);
		serialiser_(coconut_tools::Serialiser::Label("value"), value);
	}

	void operator()(const primitive::Vector& value) const {
		serialiser_(coconut_tools::Serialiser::Label("type"), PrimitiveDataType::VECTOR);
		serialiser_(coconut_tools::Serialiser::Label("value"), value);
	}

	void operator()(const Colour& value) const {
		serialiser_(coconut_tools::Serialiser::Label("type"), PrimitiveDataType::COLOUR);
		serialiser_(coconut_tools::Serialiser::Label("value"), value);
	}

	void operator()(const TextureCoordinate& value) const {
		serialiser_(coconut_tools::Serialiser::Label("type"), PrimitiveDataType::TEXTURE_COORDINATE);
		serialiser_(coconut_tools::Serialiser::Label("value"), value);
	}

private:

	coconut_tools::Serialiser& serialiser_;

};

} // anonymous namespace

Primitive::IncompatiblePixelFormat::IncompatiblePixelFormat(milk::graphics::PixelFormat pixelFormat) :
	coconut_tools::exceptions::RuntimeError("Incompatible pixel format: " + toString(pixelFormat))
{
}

void Primitive::storeAs(void* buffer, milk::graphics::PixelFormat pixelFormat) const {
	auto visitor = StoreAsVisitor(buffer, pixelFormat);
	data_.apply_visitor(visitor);
}

void primitive::serialise(coconut_tools::Serialiser& serialiser, const Primitive& aPrimitive) {
	auto visitor = SerialiseVisitor(serialiser);
	aPrimitive.data_.apply_visitor(visitor);
}

void primitive::serialise(coconut_tools::Deserialiser& deserialiser, Primitive& aPrimitive) {
	PrimitiveDataType dataType;
	deserialiser(coconut_tools::Deserialiser::Label("type"), dataType);
	const auto valueLabel = coconut_tools::Deserialiser::Label("type");
	switch (dataType) {
	case PrimitiveDataType::SCALAR: {
			auto value = Scalar();
			deserialiser(valueLabel, value);
			aPrimitive = value;
			break;
		}
	case PrimitiveDataType::POSITION: {
			auto value = Position();
			deserialiser(valueLabel, value);
			aPrimitive = value;
			break;
		}
	case PrimitiveDataType::VECTOR: {
			auto value = Vector();
			deserialiser(valueLabel, value);
			aPrimitive = value;
			break;
		}
	case PrimitiveDataType::COLOUR: {
			auto value = Colour();
			deserialiser(valueLabel, value);
			aPrimitive = value;
			break;
		}
	case PrimitiveDataType::TEXTURE_COORDINATE: {
			auto value = TextureCoordinate();
			deserialiser(valueLabel, value);
			aPrimitive = value;
			break;
		}
	default:
		throw coconut_tools::exceptions::RuntimeError(
			"Deserialisation failed, unexpected primitive type value: " +
			std::to_string(milk::utils::integralValue(dataType))
			);
	}
}
