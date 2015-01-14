#include "ObjMaterialLibParser.hpp"

#include <fstream>
#include <cassert>

#include <boost/spirit/include/phoenix.hpp>
#include <boost/bind.hpp>

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::renderer;
using namespace coconut::pulp::renderer::model_loader;

namespace spirit = boost::spirit;
namespace qi = spirit::qi;
namespace ascii = spirit::ascii;
namespace phoenix = boost::phoenix;

ObjMaterialLibParser::ObjMaterialLibParser() :
	ObjMaterialLibParser::base_type(startRule_)
{
	blankRule_ = -(qi::char_('#') >> *(qi::char_ - qi::eol)) >> qi::eol;
	endRule_ = (qi::eol | qi::eoi) >> *blankRule_;
	newMaterialRule_ = qi::lit("newmtl") >> (*(qi::char_ - ascii::space))[boost::bind(&ObjMaterialLibParser::addMaterial, this, _1)] >> endRule_;
	ambientColourRule_ = qi::lit("Ka") >> qi::repeat(3)[qi::double_][boost::bind(&ObjMaterialLibParser::setAmbientColour, this, _1)] >> endRule_;
	diffuseColourRule_ = qi::lit("Kd") >> qi::repeat(3)[qi::double_][boost::bind(&ObjMaterialLibParser::setDiffuseColour, this, _1)] >> endRule_;
	diffuseColourMapRule_ = qi::lit("map_Kd") >> (*(qi::char_ - ascii::space))[boost::bind(&ObjMaterialLibParser::setDiffuseColourMap, this, _1)] >> endRule_;
	specularColourRule_ = qi::lit("Ks") >> qi::repeat(3)[qi::double_][boost::bind(&ObjMaterialLibParser::setSpecularColour, this, _1)] >> endRule_;
	specularExponentRule_ = qi::lit("Ns") >> qi::double_[boost::bind(&ObjMaterialLibParser::setSpecularExponent, this, _1)] >> endRule_;
	illuminationModelRule_ = qi::lit("illum") >> qi::int_ >> endRule_;
	dissolveRule_ = qi::lit("d") >> qi::double_ >> endRule_;
	opticalDensityRule_ = qi::lit("Ni") >> qi::double_ >> endRule_;

	startRule_ = *blankRule_
		>> *(
		newMaterialRule_ |
		ambientColourRule_ |
		diffuseColourRule_ |
		diffuseColourMapRule_ |
		specularColourRule_ |
		specularExponentRule_ |
		illuminationModelRule_ |
		dissolveRule_ |
		opticalDensityRule_
		);
}

void ObjMaterialLibParser::parse(std::istream& is) {
	clear();

	is.unsetf(std::istream::skipws);
	spirit::istream_iterator it(is), end;

	bool result = qi::phrase_parse(it, end, *this, ascii::blank);

	if (!result || it != end) {
		std::runtime_error("Failed to parse material lib file");
	}
}

void ObjMaterialLibParser::clear() {
	materials_.clear();
}

void ObjMaterialLibParser::addMaterial(const std::vector<char>& newMaterialChars) {
	materials_.push_back(Material());
	materials_.back().name = std::string(newMaterialChars.begin(), newMaterialChars.end());
}

void ObjMaterialLibParser::setAmbientColour(const std::vector<double>& rgbColour) {
	assert(rgbColour.size() == 3);

	if (materials_.empty()) {
		throw std::runtime_error("Can't set ambient colour with no material added");
	}

	if (materials_.back().ambientColour != milk::math::Vector3d::ZERO) {
		throw std::runtime_error(
			"Ambient colour for material " + materials_.back().name + " has already been set");
	}

	materials_.back().ambientColour = RGBColour(
		static_cast<float>(rgbColour[0]),
		static_cast<float>(rgbColour[1]),
		static_cast<float>(rgbColour[2])
		);
}

void ObjMaterialLibParser::setDiffuseColour(const std::vector<double>& rgbColour) {
	assert(rgbColour.size() == 3);

	if (materials_.empty()) {
		throw std::runtime_error("Can't set diffuse colour with no material added");
	}

	if (materials_.back().diffuseColour != milk::math::Vector3d::ZERO) {
		throw std::runtime_error(
			"Diffuse colour for material " + materials_.back().name + " has already been set");
	}

	materials_.back().diffuseColour = RGBColour(
		static_cast<float>(rgbColour[0]),
		static_cast<float>(rgbColour[1]),
		static_cast<float>(rgbColour[2])
		);
}

void ObjMaterialLibParser::setSpecularColour(const std::vector<double>& rgbColour) {
	assert(rgbColour.size() == 3);

	if (materials_.empty()) {
		throw std::runtime_error("Can't set specular colour with no material added");
	}

	if (materials_.back().specularColour != milk::math::Vector3d::ZERO) {
		throw std::runtime_error(
			"Specular colour for material " + materials_.back().name + " has already been set");
	}

	materials_.back().specularColour = RGBColour(
		static_cast<float>(rgbColour[0]),
		static_cast<float>(rgbColour[1]),
		static_cast<float>(rgbColour[2])
		);
}

void ObjMaterialLibParser::setDiffuseColourMap(const std::vector<char>& newMaterialChars) {

}

void ObjMaterialLibParser::setSpecularExponent(double specularExponent) {
	if (materials_.empty()) {
		throw std::runtime_error("Can't set specular exponent with no material added");
	}

	if (materials_.back().specularExponent != 0.0f) {
		throw std::runtime_error(
			"Specular colour for material " + materials_.back().name + " has already been set");
	}

	materials_.back().specularExponent = static_cast<float>(specularExponent);
}
