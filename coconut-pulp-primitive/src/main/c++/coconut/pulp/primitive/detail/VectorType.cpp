#include "VectorType.hpp"

#include "../Primitive.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::primitive;
using namespace coconut::pulp::primitive::detail;

namespace /* anonymous */ {

template <size_t DIMENSIONS, class T>
struct StoreFunctor;

template <class T>
struct StoreFunctor<2, T> {
	
	void operator()(const T& vector, void* buffer, milk::graphics::PixelFormat pixelFormat) const {
		auto* data = reinterpret_cast<float*>(buffer);

		switch (pixelFormat) {
		case milk::graphics::PixelFormat::R32G32_FLOAT:
			data[0] = vector.get<0>();
			data[1] = vector.get<1>();
			break;
		default:
			throw Primitive::IncompatiblePixelFormat(pixelFormat);
		}
	}

};

template <class T>
struct StoreFunctor<3, T> {
	
	void operator()(const T& vector, void* buffer, milk::graphics::PixelFormat pixelFormat) const {
		auto* data = reinterpret_cast<float*>(buffer);

		switch (pixelFormat) {
		case milk::graphics::PixelFormat::R32G32B32_FLOAT:
			data[0] = vector.get<0>();
			data[1] = vector.get<1>();
			data[2] = vector.get<2>();
			break;
		default:
			StoreFunctor<2, T>()(vector, buffer, pixelFormat);
			break;
		}
	}

};

template <class T>
struct StoreFunctor<4, T> {
	
	void operator()(const T& vector, void* buffer, milk::graphics::PixelFormat pixelFormat) const {
		auto* data = reinterpret_cast<float*>(buffer);

		switch (pixelFormat) {
		case milk::graphics::PixelFormat::R32G32B32A32_FLOAT:
			data[0] = vector.get<0>();
			data[1] = vector.get<1>();
			data[2] = vector.get<2>();
			data[3] = vector.get<3>();
			break;
		default:
			StoreFunctor<3, T>()(vector, buffer, pixelFormat);
			break;
		}
	}

};

} // anonymous namespace

template <size_t DIMENSIONS>
void VectorType<DIMENSIONS>::storeAs(void* buffer, milk::graphics::PixelFormat pixelFormat) const {
	const auto store = StoreFunctor<DIMENSIONS, VectorType<DIMENSIONS>>();
	store(*this, buffer, pixelFormat);
}

template class VectorType<2>;
template class VectorType<3>;
template class VectorType<4>;
