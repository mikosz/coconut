# TODO: move finding DirectXTex to a find module
if(DEFINED DirectXTex_ROOT)
  set(DirectXTex_ALL_HINT ${DirectXTex_ROOT})
  set(DirectXTex_INCLUDE_HINT ${DirectXTex_ROOT})
  set(DirectXTex_LIBRARY_HINT ${DirectXTex_ROOT}/DirectXTex/Bin/*/*)
endif()

find_path(
  DirectXTex_INCLUDE_DIR
  "DirectXTex/DirectXTex.h"
  HINTS ${DirectXTex_INCLUDE_HINT}
  )

include_directories(${DirectXTex_INCLUDE_DIR})

find_library(
  DirectXTex_LIBRARY_RELEASE
  "DirectXTex"
  HINTS ${DirectXTex_LIBRARY_HINT}
  PATH_SUFFIXES Release
  )

find_library(
  DirectXTex_LIBRARY_DEBUG
  "DirectXTex"
  HINTS ${DirectXTex_LIBRARY_HINT}
  PATH_SUFFIXES Debug
  )

set(
  DirectXTex_LIBRARY
  debug ${DirectXTex_LIBRARY_DEBUG}
  optimized ${DirectXTex_LIBRARY_RELEASE}
  )

set(DEPENDENCY_LIBRARIES
    coconut-milk-system-${MODULE_SYSTEM}
    ${CoconutTools_EXCEPTIONS_LIBRARY}
    ${CoconutTools_LOGGER_LIBRARY}
    ${CoconutTools_CONFIGURATION_LIBRARY}
    ${CoconutTools_SYSTEM_LIBRARY}
    ${DirectXTex_LIBRARY}
    )

set(TEST_LIBRARIES
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
    )

library_module(coconut-milk-graphics-dx11 "${TEST_LIBRARIES}" "${DEPENDENCY_LIBRARIES}" ${LIBRARY_TYPE})

cotire(coconut-milk-graphics-dx11)
