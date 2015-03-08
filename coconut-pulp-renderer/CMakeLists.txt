set(DEPENDENCY_LIBRARIES
    # coconut-tools-exceptions
	# ${Boost_THREAD_LIBRARY}
    # ${Boost_SYSTEM_LIBRARY}
    )

set(TEST_LIBRARIES
	${Boost_TEST_EXEC_MONITOR_LIBRARY}
	)

library_module(coconut-pulp-renderer "${TEST_LIBRARIES}" "${DEPENDENCY_LIBRARIES}")
