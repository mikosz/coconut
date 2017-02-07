set(DEPENDENCY_LIBRARIES
	coconut-milk-math-${MODULE_MATHS}
	coconut-tools-utils
    )

set(TEST_LIBRARIES
  )

library_module(coconut-pulp-primitive "${TEST_LIBRARIES}" "${DEPENDENCY_LIBRARIES}" USE_GMOCK ${LIBRARY_TYPE})
