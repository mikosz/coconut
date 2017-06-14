set(DEPENDENCY_LIBRARIES
    )

set(TEST_LIBRARIES
    )
    
library_module(coconut-pulp-math "${TEST_LIBRARIES}" "${DEPENDENCY_LIBRARIES}" ${LIBRARY_TYPE})

cotire(coconut-pulp-math)
