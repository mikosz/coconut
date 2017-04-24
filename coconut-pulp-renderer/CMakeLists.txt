set(DEPENDENCY_LIBRARIES
    coconut-milk-fs
    coconut-milk-graphics-${MODULE_RENDERER}
    coconut-pulp-math
    coconut-pulp-mesh
    ${CoconutTools_LOGGER_LIBRARY}
    ${CoconutTools_CONFIGURATION_LIBRARY}
    ${CoconutTools_SYSTEM_LIBRARY}
    ${CoconutTools_EXCEPTIONS_LIBRARY}
    ${CoconutTools_UTILS_LIBRARY}
    )

set(TEST_LIBRARIES
  )

library_module(coconut-pulp-renderer "${TEST_LIBRARIES}" "${DEPENDENCY_LIBRARIES}" USE_GMOCK ${LIBRARY_TYPE})
