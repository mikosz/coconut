#ifndef _COCONUT_PULP_RENDERER_MODEL_LOADER_OBJMATERIALLIBPARSER_HPP_
#define _COCONUT_PULP_RENDERER_MODEL_LOADER_OBJMATERIALLIBPARSER_HPP_

#include <iosfwd>
#include <vector>
#include <string>

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/support_istream_iterator.hpp>

#include "milk/math/Vector.hpp"

namespace coconut {
namespace pulp {
namespace renderer {
namespace model_loader {

class ObjMaterialLibParser :
	public boost::spirit::qi::grammar<
		boost::spirit::istream_iterator,
		void(),
		boost::spirit::ascii::blank_type
		> {
public:

	typedef milk::math::Vector3d RGBColour;

	struct Material {

		std::string name;

		RGBColour ambientColour;

		RGBColour diffuseColour;

		RGBColour specularColour;

		float specularExponent;

		Material() :
			ambientColour(0.0f, 0.0f, 0.0f),
			diffuseColour(0.0f, 0.0f, 0.0f),
			specularColour(0.0f, 0.0f, 0.0f),
			specularExponent(0.0f)
		{
		}

	};

	typedef std::vector<Material> Materials;

	ObjMaterialLibParser();

	void parse(std::istream& is);

	const Materials& materials() const {
		return materials_;
	}

private:

	typedef boost::spirit::qi::rule<
		boost::spirit::istream_iterator,
		void(),
		boost::spirit::ascii::blank_type
	> Rule;

	Rule blankRule_;

	Rule endRule_;

	Rule newMaterialRule_;

	Rule ambientColourRule_;

	Rule diffuseColourRule_;

	Rule diffuseColourMapRule_;

	Rule specularColourRule_;

	Rule specularExponentRule_;

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

};

} // namespace model_loader
} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_MODEL_LOADER_OBJMATERIALLIBPARSER_HPP_ */
