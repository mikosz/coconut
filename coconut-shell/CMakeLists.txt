set(DEPENDENCY_LIBRARIES
    ${CoconutTools_LOGGER_LIBRARY}
    ${CoconutTools_CONFIGURATION_LIBRARY}
    ${CoconutTools_SERIALISATION_LIBRARY}
    ${CoconutTools_SYSTEM_LIBRARY}
    ${CoconutTools_EXCEPTIONS_LIBRARY}
    coconut-milk-fs
    coconut-pulp-renderer
    coconut-pulp-world
    )

set(TEST_LIBRARIES
    )
    
executable_module(
  coconut-shell
  "${TEST_LIBRARIES}"
  "${DEPENDENCY_LIBRARIES}"
  )

cotire(coconut-shell)
  