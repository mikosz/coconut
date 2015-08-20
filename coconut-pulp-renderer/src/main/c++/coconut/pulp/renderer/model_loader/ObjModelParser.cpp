#include "ObjModelParser.hpp"

#include <fstream>

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

const size_t ObjModelParser::NORMAL_INDEX_UNKNOWN = std::numeric_limits<size_t>::max();

ObjModelParser::MaterialFileOpener::IStreamPtr ObjModelParser::MaterialFileOpener::open(
	const boost::filesystem::path& path) const {
	return IStreamPtr(new std::ifstream((baseDirectory_ / path).string().c_str()));
}

ObjModelParser::ObjModelParser() :
	ObjModelParser::base_type(startRule_)
{
	// TODO: reformat the more complex parsers, this is unreadable
	blankRule_ = -(qi::char_('#') >> *(qi::char_ - qi::eol)) >> qi::eol;
	endRule_ = (qi::eol | qi::eoi) >> *blankRule_;
	smoothingGroupRule_ = 's' >> (qi::lit("off") | qi::int_) >> endRule_;
	materialRule_ = qi::lit("usemtl") >> qi::lexeme[*(qi::char_ - qi::eol - qi::eoi)][boost::bind(&ObjModelParser::setMaterial, this, _1)] >> endRule_;
	vertexRule_ = (qi::uint_ % '/')[qi::_val = phoenix::bind(&ObjModelParser::makeVertex, this, qi::_1)];
	faceRule_ = 'f' >> qi::repeat(3)[vertexRule_][boost::bind(&ObjModelParser::addFace, this, _1)] >> endRule_;
	positionRule_ = 'v' >> qi::repeat(3)[qi::double_][boost::bind(&ObjModelParser::addPosition, this, _1)] >> endRule_;
	textureCoordinateRule_ = qi::lit("vt") >> qi::repeat(2)[qi::double_][boost::bind(&ObjModelParser::addTextureCoordinate, this, _1)] >> endRule_;
	normalRule_ = qi::lit("vn") >> qi::repeat(3)[qi::double_][boost::bind(&ObjModelParser::addNormal, this, _1)] >> endRule_;
	objectNameRule_ = (qi::char_('o')) >> qi::lexeme[*(qi::char_ - qi::eol - qi::eoi)][boost::bind(&ObjModelParser::newObject, this, _1)] >> endRule_;
	groupNameRule_ = (qi::char_('g')) >> qi::lexeme[*(qi::char_ - qi::eol - qi::eoi)][boost::bind(&ObjModelParser::newGroup, this, _1)] >> endRule_;
	materialLibRule_ = qi::lit("mtllib") >> qi::lexeme[*(qi::char_ - qi::eol - qi::eoi)][boost::bind(&ObjModelParser::addMaterialLib, this, _1)] >> endRule_;

	startRule_ = *blankRule_
		>> *(
		smoothingGroupRule_ |
		materialRule_ |
		materialLibRule_ |
		faceRule_ |
		positionRule_ |
		textureCoordinateRule_ |
		normalRule_ |
		objectNameRule_ |
		groupNameRule_
		);
}

void ObjModelParser::parse(std::istream& is, const MaterialFileOpener& fileOpener) {
	clear();

	is.unsetf(std::istream::skipws);
	spirit::istream_iterator it(is), end;

	bool result = qi::phrase_parse(it, end, *this, ascii::blank);

	if (!result || it != end) {
		std::runtime_error("Failed to parse model file");
	}

	ObjMaterialLibParser materialLibParser;
	for (auto materialLib : materialLibs_) {
		auto materialIS = fileOpener.open(materialLib);
		if (!materialIS->good()) {
			throw std::runtime_error("Failed to open material lib " + materialLib);
		}

		materialLibParser.parse(*materialIS);

		for (auto material : materialLibParser.materials()) {
			if (materials_.count(material.name) != 0) {
				throw std::runtime_error("Duplicate material entry " + material.name);
			}
			materials_.insert(std::make_pair(material.name, material));
		}
	}
}

void ObjModelParser::clear() {
	objects_.clear();
	positions_.clear();
	textureCoordinates_.clear();
	normals_.clear();
	materialLibs_.clear();
	materials_.clear();
}

void ObjModelParser::setMaterial(const std::vector<char>& materialChars) {
	if (objects_.empty()) {
		throw std::runtime_error("Attempted to set a material with no object specified");
	}
	if (objects_.back().groups.empty()) {
		throw std::runtime_error("Attempted to set a material with no group specified");
	}

	if (!objects_.back().groups.back().material.empty()) {
		throw std::runtime_error("Changing the used material within a group is not supported");
	}

	objects_.back().groups.back().material = std::string(materialChars.begin(), materialChars.end());
}

ObjModelParser::Vertex ObjModelParser::makeVertex(const std::vector<size_t>& vertexData) {
	if (vertexData.size() != 3 && vertexData.size() != 2) {
		throw std::runtime_error("Vertex data has size different than 2 and 3");
	}

	Vertex vertex;
	vertex.positionIndex = vertexData[0] - 1;
	vertex.textureCoordinateIndex = vertexData[1] - 1;

	if (vertexData.size() == 3) {
		vertex.normalIndex = vertexData[2] - 1;
	} else {
		vertex.normalIndex = NORMAL_INDEX_UNKNOWN;
	}

	return vertex;
}

void ObjModelParser::addFace(const std::vector<Vertex>& face) {
	if (face.size() != 3) {
		throw std::runtime_error("Currently supporting only faces with 3 vertices");
	}
	if (objects_.empty()) {
		throw std::runtime_error("Attempted to add a face with no object specified");
	}
	if (objects_.back().groups.empty()) {
		throw std::runtime_error("Attempted to add a face with no group specified");
	}

	// changing counter-clockwise to clockwise winding while storing indices
	Face storedFace;
	storedFace.vertices[0] = face[0];
	storedFace.vertices[2] = face[1];
	storedFace.vertices[1] = face[2];

	objects_.back().groups.back().faces.push_back(storedFace);
}

void ObjModelParser::addPosition(const std::vector<double>& vector) {
	assert(vector.size() == 3);
	// right-hand to left-hand system conversion (z inverted)
	positions_.push_back(
		milk::math::Vector3d(
			static_cast<float>(vector[0]),
			static_cast<float>(vector[1]),
			-static_cast<float>(vector[2])
			)
		);
}

void ObjModelParser::addTextureCoordinate(const std::vector<double>& vector) {
	assert(vector.size() == 2);
	// right-hand to left-hand system conversion (v mirrored)
	textureCoordinates_.push_back(
		milk::math::Vector2d(
			static_cast<float>(vector[0]),
			1.0f - static_cast<float>(vector[1])
			)
		);
}

void ObjModelParser::addNormal(const std::vector<double>& vector) {
	assert(vector.size() == 3);
	// right-hand to left-hand system conversion (z inverted)
	normals_.push_back(
		milk::math::Vector3d(
			static_cast<float>(vector[0]),
			static_cast<float>(vector[1]),
			-static_cast<float>(vector[2])
			)
		);
}

void ObjModelParser::newObject(const std::vector<char>& nameChars) {
	objects_.push_back(Object());
}

void ObjModelParser::newGroup(const std::vector<char>& nameChars) {
	if (objects_.empty()) {
		throw std::runtime_error("Attempted to add a group with no object specified");
	}

	objects_.back().groups.push_back(Group());
}

void ObjModelParser::addMaterialLib(const std::vector<char>& materialLibChars) {
	materialLibs_.push_back(std::string(materialLibChars.begin(), materialLibChars.end()));
}
