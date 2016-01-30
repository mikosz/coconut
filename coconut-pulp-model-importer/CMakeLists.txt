set(DEPENDENCY_LIBRARIES
    coconut-milk-math-${MODULE_MATHS}
    coconut-tools-logger
    coconut-tools-configuration
    coconut-tools-system
    coconut-tools-exceptions
    )

set(TEST_LIBRARIES
	${Boost_TEST_EXEC_MONITOR_LIBRARY}
	)

library_module(coconut-pulp-model-importer "${TEST_LIBRARIES}" "${DEPENDENCY_LIBRARIES}")
