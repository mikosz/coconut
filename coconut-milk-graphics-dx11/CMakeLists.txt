set(DEPENDENCY_LIBRARIES
    coconut-milk-system-${MODULE_SYSTEM}
    )

set(TEST_LIBRARIES
	${Boost_TEST_EXEC_MONITOR_LIBRARY}
	)

library_module(coconut-milk-graphics-dx11 "${TEST_LIBRARIES}" "${DEPENDENCY_LIBRARIES}")
