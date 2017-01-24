#include "coconut-tools/system/platform.hpp"

#if defined(CT_COMPILER_VISUAL_CXX)
#	pragma warning(disable: 4244) // boost spirit spews conversion warnings
#endif /* CT_COMPILER_VISUAL_CXX */

#include "Parser.hpp"

#include <fstream>

#include <boost/spirit/include/phoenix.hpp>
#include <boost/bind.hpp>

#include <coconut-tools/logger.hpp>

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::model;
using namespace coconut::pulp::model::obj;

namespace spirit = boost::spirit;
namespace qi = spirit::qi;
namespace ascii = spirit::ascii;
namespace phoenix = boost::phoenix;

CT_LOGGER_CATEGORY("COCONUT.PULP.RENDERER.OBJ_MODEL_PARSER");

const size_t Parser::NORMAL_INDEX_UNKNOWN = std::numeric_limits<size_t>::max();

Parser::Parser() :
	Parser::base_type(startRule_)
{
	// TODO: reformat the more complex parsers, this is unreadable
	blankRule_ = -(qi::char_('#') >> *(qi::char_ - qi::eol)) >> qi::eol;
	endRule_ = (qi::eol | qi::eoi) >> *blankRule_;
	smoothingGroupRule_ = 's' >> (qi::lit("off") | qi::int_) >> endRule_;
	materialRule_ = qi::lit("usemtl") >> qi::lexeme[*(qi::char_ - qi::eol - qi::eoi)][boost::bind(&Parser::setMaterial, this, _1)] >> endRule_;
	vertexRule_ = (qi::uint_ % '/')[qi::_val = phoenix::bind(&Parser::makeVertex, this, qi::_1)];
	faceRule_ = 'f' >> qi::repeat(3)[vertexRule_][boost::bind(&Parser::addFace, this, _1)] >> endRule_;
	positionRule_ = 'v' >> qi::repeat(3)[qi::double_][boost::bind(&Parser::addPosition, this, _1)] >> endRule_;
	textureCoordinateRule_ = qi::lit("vt") >> qi::repeat(2)[qi::double_][boost::bind(&Parser::addTextureCoordinate, this, _1)] >> endRule_;
	normalRule_ = qi::lit("vn") >> qi::repeat(3)[qi::double_][boost::bind(&Parser::addNormal, this, _1)] >> endRule_;
	objectNameRule_ = (qi::char_('o')) >> qi::lexeme[*(qi::char_ - qi::eol - qi::eoi)][boost::bind(&Parser::newObject, this, _1)] >> endRule_;
	groupNameRule_ = (qi::char_('g')) >> qi::lexeme[*(qi::char_ - qi::eol - qi::eoi)][boost::bind(&Parser::newGroup, this, _1)] >> endRule_;
	materialLibRule_ = qi::lit("mtllib") >> qi::lexeme[*(qi::char_ - qi::eol - qi::eoi)][boost::bind(&Parser::addMaterialLib, this, _1)] >> endRule_;

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

void Parser::parse(std::istream& is, const milk::FilesystemContext& filesystemContext) {
	clear();

	CT_LOG_DEBUG << "Beginning ObjModel parsing...";

	is.unsetf(std::istream::skipws);
	spirit::istream_iterator it(is), end;

	bool result = qi::phrase_parse(it, end, *this, ascii::blank);

	if (!result || it != end) {
		std::runtime_error("Failed to parse model file");
	}

	MaterialLibParser materialLibParser;
	for (auto materialLib : materialLibs_) {
		auto materialIS = filesystemContext.open(materialLib);
		materialLibParser.parse(*materialIS);

		for (auto material : materialLibParser.materials()) {
			if (materials_.count(material.name) != 0) {
				throw std::runtime_error("Duplicate material entry " + material.name);
			}
			materials_.insert(std::make_pair(material.name, material));
		}
	}

	CT_LOG_DEBUG << "ObjModel parsing completed";
}

void Parser::clear() {
	objects_.clear();
	positions_.clear();
	textureCoordinates_.clear();
	normals_.clear();
	materialLibs_.clear();
	materials_.clear();
}

void Parser::setMaterial(const std::vector<char>& materialChars) {
	CT_LOG_TRACE << "Set material: " << std::string(materialChars.begin(), materialChars.end());

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

Parser::Vertex Parser::makeVertex(const std::vector<unsigned int>& vertexData) {
	CT_LOG_TRACE << "Make vertex: " << vertexData;

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

void Parser::addFace(const std::vector<Vertex>& face) {
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

void Parser::addPosition(const std::vector<double>& vector) {
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

void Parser::addTextureCoordinate(const std::vector<double>& vector) {
	assert(vector.size() == 2);
	// right-hand to left-hand system conversion (v mirrored)
	textureCoordinates_.push_back(
		milk::math::Vector2d(
			static_cast<float>(vector[0]),
			1.0f - static_cast<float>(vector[1])
			)
		);
}

void Parser::addNormal(const std::vector<double>& vector) {
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

void Parser::newObject(const std::vector<char>& nameChars) {
	CT_LOG_TRACE << "New object: " << std::string(nameChars.begin(), nameChars.end());

	objects_.push_back(Object());
}

void Parser::newGroup(const std::vector<char>& nameChars) {
	CT_LOG_TRACE << "New group: " << std::string(nameChars.begin(), nameChars.end());

	if (objects_.empty()) {
		throw std::runtime_error("Attempted to add a group with no object specified");
	}

	objects_.back().groups.push_back(Group());
}

void Parser::addMaterialLib(const std::vector<char>& materialLibChars) {
	materialLibs_.push_back(std::string(materialLibChars.begin(), materialLibChars.end()));
}
