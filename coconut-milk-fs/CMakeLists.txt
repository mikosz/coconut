set(DEPENDENCY_LIBRARIES
    ${CoconutTools_EXCEPTIONS_LIBRARY}
    )

set(TEST_LIBRARIES
    ${CoconutTools_SYSTEM_LIBRARY}
    )

library_module(coconut-milk-fs "${TEST_LIBRARIES}" "${DEPENDENCY_LIBRARIES}" USE_GMOCK ${LIBRARY_TYPE})

cotire(coconut-milk-fs)
cotire(coconut-milk-fs_unit-test)
