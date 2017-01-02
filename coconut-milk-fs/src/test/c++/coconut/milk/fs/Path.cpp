#define BOOST_TEST_NO_LIB
#include <boost/test/auto_unit_test.hpp>

#include "coconut/milk/fs/Path.hpp"

using namespace coconut;
using namespace coconut::milk;
using namespace coconut::milk::fs;

using namespace std::string_literals;

namespace /* anonymous */ {

BOOST_AUTO_TEST_SUITE(MilkFsPathTestSuite);

BOOST_AUTO_TEST_CASE(UnixStylePathRemainsUnchanged) {
	BOOST_CHECK_EQUAL(Path("/data/file.txt").string(), "/data/file.txt"s);
	BOOST_CHECK_EQUAL(Path("data/file.txt").string(), "data/file.txt"s);
}

BOOST_AUTO_TEST_CASE(WindowsAndMixedStylePathConvertsToUnixStylePath) {
	BOOST_CHECK_EQUAL(Path("data\\file.txt").string(), "data/file.txt"s);
	BOOST_CHECK_EQUAL(Path("/dir/subdir\\file.txt").string(), "/dir/subdir/file.txt"s);
}

BOOST_AUTO_TEST_CASE(WindowsDriveIdentifiersThrowException) {
	BOOST_CHECK_THROW(Path("C:\\Windows"), InvalidPath);
}

BOOST_AUTO_TEST_CASE(RedundantSlashesAndDotsAreRemoved) {
	BOOST_CHECK_EQUAL(Path("trailing/slash/").string(), "trailing/slash");
	BOOST_CHECK_EQUAL(Path("trailing\\backslash\\").string(), "trailing/backslash");
	BOOST_CHECK_EQUAL(Path("double//slash").string(), "double/slash");
	BOOST_CHECK_EQUAL(Path("double\\\\backslash").string(), "double/backslash");

	BOOST_CHECK_EQUAL(Path("no/./dot").string(), "no/dot");
	BOOST_CHECK_EQUAL(Path("no\\.\\dot").string(), "no/dot");

	BOOST_CHECK_EQUAL(Path("no-trailing-dot/./").string(), "no-trailing-dot");
	BOOST_CHECK_EQUAL(Path("no-trailing-dot/.").string(), "no-trailing-dot");
	BOOST_CHECK_EQUAL(Path("/no-trailing-dot/.").string(), "/no-trailing-dot");
	BOOST_CHECK_EQUAL(Path("no-trailing-dot\\.").string(), "no-trailing-dot");

	BOOST_CHECK_EQUAL(Path("./no-starting-dot").string(), "no-starting-dot");
	BOOST_CHECK_EQUAL(Path("/./no-starting-dot").string(), "/no-starting-dot");
	BOOST_CHECK_EQUAL(Path(".\\no-starting-dot").string(), "no-starting-dot");

	BOOST_CHECK_EQUAL(Path(".").string(), "");
	BOOST_CHECK_EQUAL(Path("./").string(), "");
	BOOST_CHECK_EQUAL(Path("/.").string(), "/");

	BOOST_CHECK_EQUAL(Path("..").string(), "..");
	BOOST_CHECK_EQUAL(Path("../").string(), "..");
	BOOST_CHECK_EQUAL(Path("/..").string(), "/..");
}

BOOST_AUTO_TEST_CASE(DotDotsCollapse) {
	BOOST_CHECK_EQUAL(Path("dotdot/../collapsed").string(), "collapsed");
	BOOST_CHECK_EQUAL(Path("/dotdot/../collapsed").string(), "/collapsed");
	BOOST_CHECK_EQUAL(Path("dotdot\\..\\collapsed").string(), "collapsed");
}

BOOST_AUTO_TEST_CASE(WindowsAndUnixPathEquality) {
	BOOST_CHECK_EQUAL(Path("some\\path\\to.txt"), "some/path/to.txt");
	BOOST_CHECK_NE(Path("relative/not-equals/absolute"), "/relative/not-equals/absolute");
}

BOOST_AUTO_TEST_CASE(OperatorSlashMaintainsUnixStyle) {
	const auto lhs = Path("root\\dir");
	const auto rhs = Path("subdir\\file");
	const auto empty = Path();
	BOOST_CHECK_EQUAL((lhs / rhs).string(), "root/dir/subdir/file");
	BOOST_CHECK_EQUAL((lhs / empty).string(), lhs.string());
}

BOOST_AUTO_TEST_CASE(OperatorSlashAcceptsOnlyLhsAbsolutePath) {
	const auto absolute = Path("/root");
	const auto relative = Path("dir");

	BOOST_CHECK_EQUAL(absolute / relative, "/root/dir");
	BOOST_CHECK_EQUAL(relative / relative, "dir/dir");
	BOOST_CHECK_THROW(relative / absolute, InvalidPath);
}

BOOST_AUTO_TEST_CASE(OnlyEmptyPathYieldsEmpty) {
	BOOST_CHECK(Path("").empty());
	BOOST_CHECK(Path(".").empty());
	BOOST_CHECK(!Path("..").empty());
	BOOST_CHECK(!Path("/").empty());
	BOOST_CHECK(!Path("dir").empty());
	BOOST_CHECK(!Path("dir/").empty());
}

BOOST_AUTO_TEST_CASE(OnlyEqualPathsYieldEqualHashValues) {
	auto hasher = std::hash<Path>();

	BOOST_CHECK_NE(hasher("/root"), hasher("root"));
	BOOST_CHECK_EQUAL(hasher("root/"), hasher("root"));
}

BOOST_AUTO_TEST_CASE(OnlyAbsolutePathsYieldAbsolute) {
	BOOST_CHECK(Path("/").absolute());
	BOOST_CHECK(!Path("/").relative());

	BOOST_CHECK(Path("/dir").absolute());
	BOOST_CHECK(!Path("/dir").relative());

	BOOST_CHECK(Path("/dir/file.txt").absolute());
	BOOST_CHECK(!Path("/dir/file.txt").relative());

	BOOST_CHECK((Path("/dir") / "subdir").absolute());
	BOOST_CHECK(!(Path("/dir") / "subdir").relative());
}

BOOST_AUTO_TEST_CASE(OnlyRelativePathsYieldRelative) {
	BOOST_CHECK(!Path("").absolute());
	BOOST_CHECK(Path("").relative());

	BOOST_CHECK(!Path("dir").absolute());
	BOOST_CHECK(Path("dir").relative());

	BOOST_CHECK(!Path("dir/file.txt").absolute());
	BOOST_CHECK(Path("dir/file.txt").relative());

	BOOST_CHECK(!(Path("dir") / "subdir").absolute());
	BOOST_CHECK((Path("dir") / "subdir").relative());
}

BOOST_AUTO_TEST_CASE(ParentMaintainsUnixStyle) {
	BOOST_CHECK_EQUAL(Path("dir\\subdir\\file.txt").parent().string(), "dir/subdir");
	BOOST_CHECK_EQUAL(Path("/dir/subdir/file.txt").parent().string(), "/dir/subdir");
}

BOOST_AUTO_TEST_CASE(ParentYieldsPathParent) {
	BOOST_CHECK(Path("").parent().empty());
	BOOST_CHECK(Path("..").parent().empty());
	BOOST_CHECK(Path("/").parent().empty());
	BOOST_CHECK(Path("dir").parent().empty());

	BOOST_CHECK_EQUAL(Path("/dir").parent(), "/");
	BOOST_CHECK_EQUAL(Path("/dir/subdir").parent(), "/dir");
	BOOST_CHECK_EQUAL(Path("dir/subdir").parent(), "dir");
}

BOOST_AUTO_TEST_SUITE_END(/* MilkFsPathTestSuite */);

} // anonymous namespace
