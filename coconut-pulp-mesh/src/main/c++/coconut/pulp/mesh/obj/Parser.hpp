#ifndef _COCONUT_PULP_RENDERER_MESH_OBJ_PARSER_HPP_
#define _COCONUT_PULP_RENDERER_MESH_OBJ_PARSER_HPP_

#include <array>
#include <vector>
#include <string>
#include <memory>
#include <iosfwd>
#include <unordered_map>

#include <boost/spirit/include/qi.hpp>

#include "coconut/milk/fs.hpp"

#include "coconut/milk/utils/MakePointerDefinitionsMacro.hpp"

#include "coconut/milk/graphics/PrimitiveTopology.hpp"

#include "coconut/pulp/primitive.hpp"

#include "MaterialLibParser.hpp"

namespace coconut {
namespace pulp {
namespace mesh {
namespace obj {

class Parser :
	public boost::spirit::qi::grammar<
		milk::fs::RawData::const_iterator,
		void(),
		boost::spirit::ascii::blank_type
		>
{
public:

	struct Vertex {

		size_t positionIndex;

		size_t textureCoordinateIndex;

		size_t normalIndex;

	};

	struct Face {

		std::array<Vertex, 3> vertices;

	};

	using Faces = std::vector<Face>;

	struct Group {

		milk::graphics::PrimitiveTopology primitiveTopology;

		Faces faces;

		std::string material;

	};

	using Groups = std::vector<Group>;

	struct Object {

		Groups groups;

	};

	using Objects = std::vector<Object>;

	using Positions = std::vector<primitive::Position>;

	using TextureCoordinates = std::vector<primitive::TextureCoordinate>;

	using Normals = std::vector<primitive::Vector>;

	using Material = MaterialLibParser::Material;

	using Materials = std::unordered_map<std::string, Material>;

	static const size_t NORMAL_INDEX_UNKNOWN;

	Parser();

	void parse(const milk::fs::RawData& data, const milk::FilesystemContext& filesystemContext);

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
		milk::fs::RawData::const_iterator,
		void(),
		boost::spirit::ascii::blank_type
	>;

	using VertexRule = boost::spirit::qi::rule<
		milk::fs::RawData::const_iterator,
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

	Rule topologyRule_;

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

	void setTopology(const std::vector<char>& topologyNameChars);

	void addMaterialLib(const std::vector<char>& materialLibChars);

};

CCN_MAKE_POINTER_DEFINITIONS(Parser);

} // namespace obj
} // namespace mesh
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_MESH_OBJ_PARSER_HPP_ */
