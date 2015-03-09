set(DEPENDENCY_LIBRARIES
    coconut-pulp-renderer
    )

set(TEST_LIBRARIES
	${Boost_TEST_EXEC_MONITOR_LIBRARY}
	)
	
executable_module(
  coconut-shell
  "${TEST_LIBRARIES}"
  "${DEPENDENCY_LIBRARIES}"
  )
