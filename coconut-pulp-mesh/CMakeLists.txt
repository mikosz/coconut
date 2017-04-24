set(DEPENDENCY_LIBRARIES
    coconut-pulp-primitive
    coconut-milk-fs
    coconut-milk-graphics-${MODULE_RENDERER}
    coconut-pulp-math
    ${CoconutTools_LOGGER_LIBRARY}
    ${CoconutTools_CONFIGURATION_LIBRARY}
    ${CoconutTools_SERIALISATION_LIBRARY}
    ${CoconutTools_SYSTEM_LIBRARY}
    ${CoconutTools_EXCEPTIONS_LIBRARY}
    )

set(TEST_LIBRARIES
    )

library_module(coconut-pulp-mesh "${TEST_LIBRARIES}" "${DEPENDENCY_LIBRARIES}" ${LIBRARY_TYPE})
