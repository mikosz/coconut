#ifndef _COCONUT_PULP_RENDERER_COLOUR_HPP_
#define _COCONUT_PULP_RENDERER_COLOUR_HPP_

#include <string>
#include <cstdint>
#include <type_traits>

#include <coconut-tools/exceptions/LogicError.hpp>

#include "ColourFormat.hpp"

namespace coconut {
namespace pulp {
namespace renderer {

class Colour {
public:

	float red() const;

	float green() const;

	float blue() const;

	float alpha() const;

	ColourFormat colourFormat() const {
		return colourFormat_;
	}

	const float* data() const {
		return channels_;
	}

protected:

	Colour(ColourFormat colourFormat, float channel0, float channel1 = 0.0f, float channel2 = 0.0f, float channel3 = 0.0f);

private:

	ColourFormat colourFormat_;

	float channels_[4];

};

struct Red : Colour {

	Red(float value) :
		Colour(ColourFormat::R, value)
	{
	}

};

inline Red operator""_r(long double value) {
	return Red(static_cast<float>(value));
}

struct Green : Colour {

	Green(float value) :
		Colour(ColourFormat::G, value)
	{
	}

};

inline Green operator""_g(long double value) {
	return Green(static_cast<float>(value));
}

struct Blue : Colour {

	Blue(float value) :
		Colour(ColourFormat::B, value)
	{
	}

};

inline Blue operator""_b(long double value) {
	return Blue(static_cast<float>(value));
}

struct Alpha : Colour {

	Alpha(float value) :
		Colour(ColourFormat::A, value)
	{
	}

};

inline Alpha operator""_a(long double value) {
	return Alpha(static_cast<float>(value));
}

struct RGB : Colour {

	RGB(float red, float green, float blue) :
		Colour(ColourFormat::RGB, red, green, blue)
	{
	}

};

struct RGBA : Colour {

	RGBA(float red, float green, float blue, float alpha) :
		Colour(ColourFormat::RGBA, red, green, blue, alpha)
	{
	}

};

} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_COLOUR_HPP_ */
