#define BOOST_TEST_MAIN

#include <boost/test/auto_unit_test.hpp>

#include <coconut-tools/system/platform.hpp>

#ifdef CT_COMPILER_VISUAL_CXX
#	pragma comment(linker, "/subsystem:console")
#endif
