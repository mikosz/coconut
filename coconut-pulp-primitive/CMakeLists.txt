set(DEPENDENCY_LIBRARIES
	coconut-pulp-math
	${CoconutTools_UTILS_LIBRARY}
  )

set(TEST_LIBRARIES
  )

library_module(coconut-pulp-primitive "${TEST_LIBRARIES}" "${DEPENDENCY_LIBRARIES}" USE_GMOCK ${LIBRARY_TYPE})
