set(DEPENDENCY_LIBRARIES
    coconut-tools-exceptions
    # ${Boost_THREAD_LIBRARY}
    # ${Boost_SYSTEM_LIBRARY}
    )

set(TEST_LIBRARIES
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
    )

library_module(coconut-milk-system-win32 "${TEST_LIBRARIES}" "${DEPENDENCY_LIBRARIES}" ${LIBRARY_TYPE})
