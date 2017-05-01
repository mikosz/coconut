set(DEPENDENCY_LIBRARIES
    # ${CoconutTools_EXCEPTIONS_LIBRARY}
    # ${Boost_THREAD_LIBRARY}
    # ${Boost_SYSTEM_LIBRARY}
    )

set(TEST_LIBRARIES
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
    )

library_module(coconut-milk-utils "${TEST_LIBRARIES}" "${DEPENDENCY_LIBRARIES}" ${LIBRARY_TYPE})

cotire(coconut-milk-utils)
cotire(coconut-milk-utils_unit-test)
