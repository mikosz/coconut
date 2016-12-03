set(DEPENDENCY_LIBRARIES
    coconut-milk-graphics-${MODULE_RENDERER}
    coconut-milk-math-${MODULE_MATHS}
    coconut-tools-logger
    coconut-tools-configuration
	coconut-tools-serialisation
    coconut-tools-system
    coconut-tools-exceptions
    )

set(TEST_LIBRARIES
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
    )

library_module(coconut-pulp-model "${TEST_LIBRARIES}" "${DEPENDENCY_LIBRARIES}" ${LIBRARY_TYPE})
