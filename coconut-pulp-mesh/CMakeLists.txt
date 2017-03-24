set(DEPENDENCY_LIBRARIES
    coconut-pulp-primitive
    coconut-milk-fs
    coconut-milk-graphics-${MODULE_RENDERER}
    coconut-pulp-math
    coconut-tools-logger
    coconut-tools-configuration
    coconut-tools-serialisation
    coconut-tools-system
    coconut-tools-exceptions
    )

set(TEST_LIBRARIES
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
    )

library_module(coconut-pulp-mesh "${TEST_LIBRARIES}" "${DEPENDENCY_LIBRARIES}" ${LIBRARY_TYPE})
