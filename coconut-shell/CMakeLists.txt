set(DEPENDENCY_LIBRARIES
    coconut-pulp-renderer
    coconut-tools-logger
    coconut-tools-configuration
	coconut-tools-serialisation
    coconut-tools-system
    coconut-tools-exceptions
    )

set(TEST_LIBRARIES
    ${Boost_TEST_EXEC_MONITOR_LIBRARY}
    )
    
executable_module(
  coconut-shell
  "${TEST_LIBRARIES}"
  "${DEPENDENCY_LIBRARIES}"
  )
