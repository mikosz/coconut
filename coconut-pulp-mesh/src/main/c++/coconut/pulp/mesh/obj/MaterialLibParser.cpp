#include "coconut-tools/system/platform.hpp"

#if defined(CT_COMPILER_VISUAL_CXX)
#	pragma warning(disable: 4244) // boost spirit spews conversion warnings
#endif /* CT_COMPILER_VISUAL_CXX */

#include "MaterialLibParser.hpp"

#include <cassert>
#include <iterator>

#include <boost/spirit/include/phoenix.hpp>
#include <boost/bind.hpp>

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::mesh;
using namespace coconut::pulp::mesh::obj;

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
	shaderRule_ = qi::lit("@shader") >> qi::lexeme[*(qi::char_ - qi::eol - qi::eoi)][boost::bind(&MaterialLibParser::setShader, this, _1)] >> endRule_;
	noiseMapRule_ = qi::lit("@noise") >> qi::lexeme[*(qi::char_ - qi::eol - qi::eoi)][boost::bind(&MaterialLibParser::setNoiseMap, this, _1)] >> endRule_;

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
			noiseMapRule_ |
			illuminationModelRule_ |
			dissolveRule_ |
			opticalDensityRule_ |
			shaderRule_
			);
}

void MaterialLibParser::parse(const milk::fs::RawData& data) {
	clear();

	auto dataBegin = data.begin();
	const auto dataEnd = data.end();

	bool result = qi::phrase_parse(dataBegin, dataEnd, *this, ascii::blank);

	if (!result || dataBegin != dataEnd) {
		std::ostringstream err;
		throw std::runtime_error("Failed to parse material lib file"); // TODO: exc
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

	materials_.back().ambientColour = Colour(
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

	materials_.back().diffuseColour = Colour(
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

	materials_.back().specularColour = Colour(
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

	materials_.back().specularExponent = static_cast<float>(specularExponent);
}

void MaterialLibParser::setBumpMap(const std::vector<char>& bumpMapChars) {
	materials_.back().bumpMap = std::string(bumpMapChars.begin(), bumpMapChars.end());
}

void MaterialLibParser::setDissolveMap(const std::vector<char>& dissolveMapChars) {
	materials_.back().dissolveMap = std::string(dissolveMapChars.begin(), dissolveMapChars.end());
}

void MaterialLibParser::setShader(const std::vector<char>& dissolveMapChars) {
	materials_.back().shader = std::string(dissolveMapChars.begin(), dissolveMapChars.end());
}

void MaterialLibParser::setNoiseMap(const std::vector<char>& noiseMapChars) {
	materials_.back().noiseMap = std::string(noiseMapChars.begin(), noiseMapChars.end());
}
