#include "coconut-tools/system/platform.hpp"

#if defined(CT_COMPILER_VISUAL_CXX)
#	pragma warning(disable: 4244) // boost spirit spews conversion warnings
#endif /* CT_COMPILER_VISUAL_CXX */

#include "MaterialLibParser.hpp"

#include <fstream>
#include <cassert>

#include <boost/spirit/include/phoenix.hpp>
#include <boost/bind.hpp>

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::model;
using namespace coconut::pulp::model::obj;

namespace spirit = boost::spirit;
namespace qi = spirit::qi;
namespace ascii = spirit::ascii;
namespace phoenix = boost::phoenix;

MaterialLibParser::MaterialLibParser() :
	MaterialLibParser::base_type(startRule_)
{
	// TODO: reformat the more complex parsers, this is unreadable
	blankRule_ = -(qi::char_('#') >> *(qi::char_ - qi::eol)) >> qi::eol;
	endRule_ = (qi::eol | qi::eoi) >> *blankRule_;
	newMaterialRule_ = qi::lit("newmtl") >> qi::lexeme[*(qi::char_ - qi::eol - qi::eoi)][boost::bind(&MaterialLibParser::addMaterial, this, _1)] >> endRule_;
	ambientColourRule_ = qi::lit("Ka") >> qi::repeat(3)[qi::double_][boost::bind(&MaterialLibParser::setAmbientColour, this, _1)] >> endRule_;
	diffuseColourRule_ = qi::lit("Kd") >> qi::repeat(3)[qi::double_][boost::bind(&MaterialLibParser::setDiffuseColour, this, _1)] >> endRule_;
	diffuseColourMapRule_ = qi::lit("map_Kd") >> qi::lexeme[*(qi::char_ - qi::eol - qi::eoi)][boost::bind(&MaterialLibParser::setDiffuseColourMap, this, _1)] >> endRule_;
	specularColourRule_ = qi::lit("Ks") >> qi::repeat(3)[qi::double_][boost::bind(&MaterialLibParser::setSpecularColour, this, _1)] >> endRule_;
	specularExponentRule_ = qi::lit("Ns") >> qi::double_[boost::bind(&MaterialLibParser::setSpecularExponent, this, _1)] >> endRule_;
	bumpMapRule_ = qi::lit("map_Bump") >> qi::lexeme[*(qi::char_ - qi::eol - qi::eoi)][boost::bind(&MaterialLibParser::setBumpMap, this, _1)] >> endRule_;
	dissolveMapRule_ = qi::lit("map_d") >> qi::lexeme[*(qi::char_ - qi::eol - qi::eoi)][boost::bind(&MaterialLibParser::setDissolveMap, this, _1)] >> endRule_;
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
			bumpMapRule_ |
			dissolveMapRule_ |
			illuminationModelRule_ |
			dissolveRule_ |
			opticalDensityRule_
			);
}

void MaterialLibParser::parse(std::istream& is) {
	clear();

	is.unsetf(std::istream::skipws);
	spirit::istream_iterator it(is), end;

	spirit::classic::position_iterator2<spirit::istream_iterator> posIt(it, end), posEnd;

	bool result = qi::phrase_parse(posIt, posEnd, *this, ascii::blank);

	if (!result || posIt != posEnd) {
		std::ostringstream err;
		const auto& position = posIt.get_position();
		err << "Failed to parse material lib file at line: " << position.line << ", column: " << position.column;
		throw std::runtime_error(err.str()); // TODO: exc
	}
}

void MaterialLibParser::clear() {
	materials_.clear();
}

void MaterialLibParser::addMaterial(const std::vector<char>& newMaterialChars) {
	materials_.push_back(Material());
	materials_.back().name = std::string(newMaterialChars.begin(), newMaterialChars.end());
}

void MaterialLibParser::setAmbientColour(const std::vector<double>& rgbColour) {
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

void MaterialLibParser::setDiffuseColour(const std::vector<double>& rgbColour) {
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

void MaterialLibParser::setSpecularColour(const std::vector<double>& rgbColour) {
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

void MaterialLibParser::setDiffuseColourMap(const std::vector<char>& newMaterialChars) {
	materials_.back().diffuseMap = std::string(newMaterialChars.begin(), newMaterialChars.end());
}

void MaterialLibParser::setSpecularExponent(double specularExponent) {
	if (materials_.empty()) {
		throw std::runtime_error("Can't set specular exponent with no material added");
	}

	if (materials_.back().specularExponent != 0.0f) {
		throw std::runtime_error(
			"Specular colour for material " + materials_.back().name + " has already been set");
	}

	materials_.back().specularExponent = static_cast<float>(specularExponent);
}

void MaterialLibParser::setBumpMap(const std::vector<char>& bumpMapChars) {
	materials_.back().bumpMap = std::string(bumpMapChars.begin(), bumpMapChars.end());
}

void MaterialLibParser::setDissolveMap(const std::vector<char>& dissolveMapChars) {
	materials_.back().dissolveMap = std::string(dissolveMapChars.begin(), dissolveMapChars.end());
}
