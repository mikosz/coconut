set(DEPENDENCY_LIBRARIES
    coconut-milk-math-${MODULE_MATHS}
    coconut-milk-graphics-${MODULE_RENDERER}
    )

set(TEST_LIBRARIES
	${Boost_TEST_EXEC_MONITOR_LIBRARY}
	)

library_module(coconut-pulp-renderer "${TEST_LIBRARIES}" "${DEPENDENCY_LIBRARIES}")
