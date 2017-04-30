set(DEPENDENCY_LIBRARIES
    coconut-pulp-renderer
    )

set(TEST_LIBRARIES
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
    )
    
library_module(coconut-pulp-world "${TEST_LIBRARIES}" "${DEPENDENCY_LIBRARIES}" USE_GMOCK ${LIBRARY_TYPE})

cotire(coconut-pulp-world)
