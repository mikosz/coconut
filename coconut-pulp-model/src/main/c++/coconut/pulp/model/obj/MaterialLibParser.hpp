#ifndef _COCONUT_PULP_RENDERER_MODEL_OBJ_MATERIALLIBPARSER_HPP_
#define _COCONUT_PULP_RENDERER_MODEL_OBJ_MATERIALLIBPARSER_HPP_

#include <iosfwd>
#include <vector>
#include <string>

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/support_istream_iterator.hpp>
#include <boost/spirit/include/classic_position_iterator.hpp>

#include "coconut/milk/math/Vector.hpp"

namespace coconut {
namespace pulp {
namespace model {
namespace obj {

class MaterialLibParser :
	public boost::spirit::qi::grammar<
	boost::spirit::classic::position_iterator2<boost::spirit::istream_iterator>,
	void(),
	boost::spirit::ascii::blank_type
	> {
public:

	using RGBColour = milk::math::Vector3d;

	struct Material {

		std::string name;

		RGBColour ambientColour;

		RGBColour diffuseColour;

		RGBColour specularColour;

		float specularExponent;

		std::string diffuseMap;

		std::string bumpMap;

		std::string dissolveMap;

		Material() :
			ambientColour(0.0f, 0.0f, 0.0f),
			diffuseColour(0.0f, 0.0f, 0.0f),
			specularColour(0.0f, 0.0f, 0.0f),
			specularExponent(0.0f)
		{
		}

	};

	using Materials = std::vector<Material>;

	MaterialLibParser();

	void parse(std::istream& is);

	const Materials& materials() const {
		return materials_;
	}

private:

	using Rule = boost::spirit::qi::rule<
		boost::spirit::classic::position_iterator2<boost::spirit::istream_iterator>,
		void(),
		boost::spirit::ascii::blank_type
		>;

	Rule blankRule_;

	Rule endRule_;

	Rule newMaterialRule_;

	Rule ambientColourRule_;

	Rule diffuseColourRule_;

	Rule diffuseColourMapRule_;

	Rule specularColourRule_;

	Rule specularExponentRule_;

	Rule bumpMapRule_;

	Rule dissolveMapRule_;

	Rule illuminationModelRule_;

	Rule dissolveRule_;

	Rule opticalDensityRule_;

	Rule startRule_;

	Materials materials_;

	void clear();

	void addMaterial(const std::vector<char>& newMaterialChars);

	void setAmbientColour(const std::vector<double>& rgbColour);

	void setDiffuseColour(const std::vector<double>& rgbColour);

	void setSpecularColour(const std::vector<double>& rgbColour);

	void setDiffuseColourMap(const std::vector<char>& newMaterialChars);

	void setSpecularExponent(double specularExponent);

	void setBumpMap(const std::vector<char>& bumpMapChars);

	void setDissolveMap(const std::vector<char>& dissolveMapChars);

};

} // namespace obj
} // namespace model
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_MODEL_OBJ_MATERIALLIBPARSER_HPP_ */
