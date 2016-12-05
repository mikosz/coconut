set(DEPENDENCY_LIBRARIES
	coconut-tools-exceptions
    )

set(TEST_LIBRARIES
    ${Boost_TEST_EXEC_MONITOR_LIBRARY}
	coconut-tools-system
    )

library_module(coconut-milk-fs "${TEST_LIBRARIES}" "${DEPENDENCY_LIBRARIES}" USE_GMOCK ${LIBRARY_TYPE})
