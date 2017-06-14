#define BOOST_TEST_NO_MAIN
#define BOOST_TEST_MODULE "renderer"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "coconut/milk/system/cleanup-windows-macros.hpp"

#include <boost/test/included/unit_test.hpp>

#include <coconut-tools/system/platform.hpp>

#include "coconut/milk/system/App.hpp"

#ifdef CT_COMPILER_VISUAL_CXX
#	pragma comment(linker, "/subsystem:windows")
#endif

namespace /* anonymous */
{

boost::unit_test::test_suite* initUnitTest(int, char**) {
	return nullptr;
}

} // anonymous namespace

// TODO: platform dependent
CT_BEGIN_MAIN();
	return boost::unit_test::unit_test_main(&initUnitTest, __argc, __argv);
CT_END_MAIN();
