set(DEPENDENCY_LIBRARIES
    coconut-milk-system-${MODULE_SYSTEM}
    coconut-tools-exceptions
    coconut-tools-logger
    coconut-tools-configuration
    coconut-tools-system
    )

set(TEST_LIBRARIES
    ${Boost_TEST_EXEC_MONITOR_LIBRARY}
    )

library_module(coconut-milk-graphics-dx11 "${TEST_LIBRARIES}" "${DEPENDENCY_LIBRARIES}" ${LIBRARY_TYPE})
