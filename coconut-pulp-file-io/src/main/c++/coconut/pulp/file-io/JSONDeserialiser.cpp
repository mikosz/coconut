#include "JSONDeserialiser.hpp"

#include <cstddef>
#include <iostream>
#include <cassert>

#include "coconut-tools/system/platform.hpp"

#if defined(CT_COMPILER_VISUAL_CXX)
#	pragma warning(push)
#	pragma warning(disable: 4715) // boost json_parser emits "not all control paths return a value"
#endif /* CT_COMPILER_VISUAL_CXX */

#include <boost/property_tree/json_parser.hpp>

#if defined(CT_COMPILER_VISUAL_CXX)
#	pragma warning(pop)
#endif /* CT_COMPILER_VISUAL_CXX */

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::file_io;

class JSONDeserialiser::PropertyTreeNode {
public:

	virtual ~PropertyTreeNode() = default;

	virtual bool atEnd() const = 0;

	virtual void next() = 0;

	virtual boost::property_tree::ptree get() = 0;

};

class JSONDeserialiser::ValueNode : public JSONDeserialiser::PropertyTreeNode {
public:

	ValueNode(boost::property_tree::ptree node) :
		node_(node)
	{
	}

	bool atEnd() const override {
		return true;
	}

	void next() override {
	}

	boost::property_tree::ptree get() override {
		return node_;
	}

private:

	boost::property_tree::ptree node_;

};

class JSONDeserialiser::ArrayNode : public JSONDeserialiser::PropertyTreeNode {
public:

	ArrayNode(boost::property_tree::ptree parent) :
		parent_(parent),
		current_(parent_.begin())
	{
	}

	bool atEnd() const override {
		return current_ == parent_.end();
	}

	void next() override {
		assert(!atEnd());
		++current_;
	}

	boost::property_tree::ptree get() override {
		return current_->second;
	}

private:

	boost::property_tree::ptree parent_;

	boost::property_tree::ptree::const_iterator current_;

};

JSONDeserialiser::JSONDeserialiser(std::istream& is) :
	is_(is)
{
	is_.exceptions(std::ios::badbit | std::ios::failbit);

	boost::property_tree::read_json(is, propertyTree_);
	currentNode_ = std::move(std::make_unique<ValueNode>(propertyTree_));
}

JSONDeserialiser::~JSONDeserialiser() {
}

void JSONDeserialiser::readObjectStart() {
	path_.push(std::move(currentNode_));
	currentNode_ = std::make_unique<ValueNode>(path_.top()->get());
}

void JSONDeserialiser::readObjectEnd() {
	currentNode_ = std::move(path_.top());
	path_.pop();
	currentNode_->next();
}

std::uint32_t JSONDeserialiser::readArrayStart() {
	std::uint32_t count = static_cast<std::uint32_t>(currentNode_->get().count(""));
	path_.push(std::move(currentNode_));
	currentNode_ = std::make_unique<ArrayNode>(path_.top()->get());

	return count;
}

void JSONDeserialiser::readArrayEnd() {
	currentNode_ = std::move(path_.top());
	path_.pop();
	currentNode_->next();
}

void JSONDeserialiser::readLabel(std::string label) {
	currentNode_ = std::make_unique<ValueNode>(path_.top()->get().get_child(label));
}

void JSONDeserialiser::read(bool& b) {
	readValue(b);
}

void JSONDeserialiser::read(std::uint8_t& i) {
	readValue(i);
}

void JSONDeserialiser::read(std::int8_t& i) {
	readValue(i);
}

void JSONDeserialiser::read(std::uint16_t& i) {
	readValue(i);
}

void JSONDeserialiser::read(std::int16_t& i) {
	readValue(i);
}

void JSONDeserialiser::read(std::uint32_t& i) {
	readValue(i);
}

void JSONDeserialiser::read(std::int32_t& i) {
	readValue(i);
}

void JSONDeserialiser::read(std::uint64_t& i) {
	readValue(i);
}

void JSONDeserialiser::read(std::int64_t& i) {
	readValue(i);
}

void JSONDeserialiser::read(float& f) {
	readValue(f);
}

void JSONDeserialiser::read(std::string& s) {
	readValue(s);
}

template <class T>
void JSONDeserialiser::readValue(T& v) {
	v = currentNode_->get().get_value<T>();
	currentNode_->next();
}
