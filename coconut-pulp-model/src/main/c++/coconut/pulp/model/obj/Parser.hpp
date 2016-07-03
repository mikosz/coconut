#ifndef _COCONUT_PULP_RENDERER_MODEL_OBJ_PARSER_HPP_
#define _COCONUT_PULP_RENDERER_MODEL_OBJ_PARSER_HPP_

#include <vector>
#include <string>
#include <memory>
#include <iosfwd>
#include <unordered_map>

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/support_istream_iterator.hpp>

#include <boost/filesystem/path.hpp>

#include "coconut/milk/math/Vector.hpp"

#include "coconut/milk/utils/MakePointerDefinitionsMacro.hpp"

#include "MaterialLibParser.hpp"

namespace coconut {
namespace pulp {
namespace model {
namespace obj {

class Parser :
	public boost::spirit::qi::grammar<
		boost::spirit::istream_iterator,
		void(),
		boost::spirit::ascii::blank_type
		> {
public:

	// TODO: this needs to be an external class
	class MaterialFileOpener {
	public:

		typedef std::unique_ptr<std::istream> IStreamPtr;

		MaterialFileOpener() {
		}

		MaterialFileOpener(const boost::filesystem::path& baseDirectory) :
			baseDirectory_(baseDirectory)
		{
		}

		virtual ~MaterialFileOpener() {
		}

		virtual IStreamPtr open(const std::string& name) const;

		virtual boost::filesystem::path pathTo(const std::string& name) const;

	private:

		boost::filesystem::path baseDirectory_;

	};

	struct Vertex {

		size_t positionIndex;

		size_t textureCoordinateIndex;

		size_t normalIndex;

	};

	struct Face {

		Vertex vertices[3];

	};

	using Faces = std::vector<Face>;

	struct Group {

		Faces faces;

		std::string material;

	};

	using Groups = std::vector<Group>;

	struct Object {

		Groups groups;

	};

	using Objects = std::vector<Object>;

	using Positions = std::vector<milk::math::Vector3d>;

	using TextureCoordinates = std::vector<milk::math::Vector2d>;

	using Normals = std::vector<milk::math::Vector3d>;

	using Material = MaterialLibParser::Material;

	using Materials = std::unordered_map<std::string, Material>;

	static const size_t NORMAL_INDEX_UNKNOWN;

	Parser();

	void parse(std::istream& is, const MaterialFileOpener& fileOpener);

	const Objects& objects() const {
		return objects_;
	}

	const Positions& positions() const {
		return positions_;
	}

	const TextureCoordinates& textureCoordinateIndex() const {
		return textureCoordinates_;
	}

	const Normals& normals() const {
		return normals_;
	}

	const Materials& materials() const {
		return materials_;
	}

private:

	using Rule = boost::spirit::qi::rule<
		boost::spirit::istream_iterator,
		void(),
		boost::spirit::ascii::blank_type
	>;

	using VertexRule = boost::spirit::qi::rule<
		boost::spirit::istream_iterator,
		Vertex(),
		boost::spirit::ascii::blank_type
	>;

	using MaterialLibs = std::vector<std::string>;

	Rule blankRule_;

	Rule endRule_;

	Rule smoothingGroupRule_;

	Rule materialRule_;

	VertexRule vertexRule_;

	Rule faceRule_;

	Rule positionRule_;

	Rule textureCoordinateRule_;

	Rule normalRule_;

	Rule objectNameRule_;

	Rule groupNameRule_;

	Rule materialLibRule_;

	Rule startRule_;

	Objects objects_;

	Positions positions_;

	TextureCoordinates textureCoordinates_;

	Normals normals_;

	MaterialLibs materialLibs_;

	Materials materials_;

	void clear();

	void setMaterial(const std::vector<char>& materialChars);

	Vertex makeVertex(const std::vector<unsigned int>& vertexData);

	void addFace(const std::vector<Vertex>& face);

	void addPosition(const std::vector<double>& vector);

	void addTextureCoordinate(const std::vector<double>& vector);

	void addNormal(const std::vector<double>& vector);

	void newObject(const std::vector<char>& nameChars);

	void newGroup(const std::vector<char>& nameChars);

	void addMaterialLib(const std::vector<char>& materialLibChars);

};

CCN_MAKE_POINTER_DEFINITIONS(Parser);

} // namespace obj
} // namespace model
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_MODEL_OBJ_PARSER_HPP_ */