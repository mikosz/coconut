set(DEPENDENCY_LIBRARIES
    coconut-tools-logger
    coconut-tools-configuration
    coconut-tools-system
    coconut-tools-exceptions
    )

set(TEST_LIBRARIES
	${Boost_TEST_EXEC_MONITOR_LIBRARY}
	)

library_module(coconut-pulp-file-io "${TEST_LIBRARIES}" "${DEPENDENCY_LIBRARIES}")
