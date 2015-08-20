#ifndef _COCONUT_PULP_RENDERER_MODEL_LOADER_OBJ_MODEL_PARSER_HPP_
#define _COCONUT_PULP_RENDERER_MODEL_LOADER_OBJ_MODEL_PARSER_HPP_

#include <vector>
#include <string>
#include <memory>
#include <iosfwd>
#include <unordered_map>

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/support_istream_iterator.hpp>

#include <boost/filesystem/path.hpp>

#include "coconut/milk/math/Vector.hpp"

#include "ObjMaterialLibParser.hpp"

namespace coconut {
namespace pulp {
namespace renderer {
namespace model_loader {

class ObjModelParser :
	public boost::spirit::qi::grammar<
		boost::spirit::istream_iterator,
		void(),
		boost::spirit::ascii::blank_type
		> {
public:

	class MaterialFileOpener {
	public:

		typedef std::shared_ptr<std::istream> IStreamPtr;

		MaterialFileOpener() {
		}

		MaterialFileOpener(const boost::filesystem::path& baseDirectory) :
			baseDirectory_(baseDirectory)
		{
		}

		virtual ~MaterialFileOpener() {
		}

		virtual IStreamPtr open(const boost::filesystem::path& path) const;

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

	typedef std::vector<Face> Faces;

	struct Group {

		Faces faces;

		std::string material;

	};

	typedef std::vector<Group> Groups;

	struct Object {

		Groups groups;

	};

	typedef std::vector<Object> Objects;

	typedef std::vector<milk::math::Vector3d> Positions;

	typedef std::vector<milk::math::Vector2d> TextureCoordinates;

	typedef std::vector<milk::math::Vector3d> Normals;

	typedef ObjMaterialLibParser::Material Material;

	typedef std::unordered_map<std::string, Material> Materials;

	static const size_t NORMAL_INDEX_UNKNOWN;

	ObjModelParser();

	void parse(std::istream& is, const MaterialFileOpener& fileOpener);

	const Objects& objects() const {
		return objects_;
	}

	const Positions& positions() const {
		return positions_;
	}

	const TextureCoordinates& textureCoordinates() const {
		return textureCoordinates_;
	}

	const Normals& normals() const {
		return normals_;
	}

	const Materials& materials() const {
		return materials_;
	}

private:

	typedef boost::spirit::qi::rule<
		boost::spirit::istream_iterator,
		void(),
		boost::spirit::ascii::blank_type
		> Rule;

	typedef boost::spirit::qi::rule<
		boost::spirit::istream_iterator,
		Vertex(),
		boost::spirit::ascii::blank_type
		> VertexRule;

	typedef std::vector<std::string> MaterialLibs;

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

	Vertex makeVertex(const std::vector<size_t>& vertexData);

	void addFace(const std::vector<Vertex>& face);

	void addPosition(const std::vector<double>& vector);

	void addTextureCoordinate(const std::vector<double>& vector);

	void addNormal(const std::vector<double>& vector);

	void newObject(const std::vector<char>& nameChars);

	void newGroup(const std::vector<char>& nameChars);

	void addMaterialLib(const std::vector<char>& materialLibChars);

};

} // namespace model_loader
} // namespace renderer
} // namespace pulp
} // namespace coconut

#endif /* _COCONUT_PULP_RENDERER_MODEL_LOADER_OBJ_MODEL_PARSER_HPP_ */
