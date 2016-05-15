#include "Colour.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::renderer;

float Colour::red() const {
	switch (colourFormat_) {
	case ColourFormat::RGBA:
	case ColourFormat::RGB:
	case ColourFormat::R:
		return channels_[0];
	default:
		return 0.0f;
	}
}

float Colour::green() const {
	switch (colourFormat_) {
	case ColourFormat::RGBA:
	case ColourFormat::RGB:
		return channels_[1];
	case ColourFormat::G:
		return channels_[0];
	default:
		return 0.0f;
	}
}

float Colour::blue() const {
	switch (colourFormat_) {
	case ColourFormat::RGBA:
	case ColourFormat::RGB:
		return channels_[2];
	case ColourFormat::B:
		return channels_[0];
	default:
		return 0.0f;
	}
}

float Colour::alpha() const {
	switch (colourFormat_) {
	case ColourFormat::RGBA:
		return channels_[3];
	case ColourFormat::A:
		return channels_[0];
	default:
		return 0.0f;
	}
}

Colour::Colour(ColourFormat colourFormat, float channel0, float channel1, float channel2, float channel3) :
	colourFormat_(colourFormat)
{
	channels_[0] = channel0;
	channels_[1] = channel1;
	channels_[2] = channel2;
	channels_[3] = channel3;
}
