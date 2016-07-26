set(DEPENDENCY_LIBRARIES
    coconut-pulp-renderer
    coconut-pulp-file-io
    coconut-tools-logger
    coconut-tools-configuration
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
