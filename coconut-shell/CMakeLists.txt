set(DEPENDENCY_LIBRARIES
    coconut-tools-logger
    coconut-tools-configuration
    coconut-tools-serialisation
    coconut-tools-system
    coconut-tools-exceptions
	coconut-milk-fs
    coconut-pulp-renderer
	coconut-pulp-world
    )

set(TEST_LIBRARIES
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
    )
    
executable_module(
  coconut-shell
  "${TEST_LIBRARIES}"
  "${DEPENDENCY_LIBRARIES}"
  )
