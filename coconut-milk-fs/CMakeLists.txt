set(DEPENDENCY_LIBRARIES
    coconut-tools-exceptions
    )

set(TEST_LIBRARIES
    coconut-tools-system
    )

library_module(coconut-milk-fs "${TEST_LIBRARIES}" "${DEPENDENCY_LIBRARIES}" USE_GMOCK ${LIBRARY_TYPE})
