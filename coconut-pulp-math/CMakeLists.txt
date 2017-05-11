set(DEPENDENCY_LIBRARIES
    )

set(TEST_LIBRARIES
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
    )
    
library_module(coconut-pulp-math "${TEST_LIBRARIES}" "${DEPENDENCY_LIBRARIES}" ${LIBRARY_TYPE})

cotire(coconut-pulp-math)
cotire(coconut-pulp-math_unit-test)
