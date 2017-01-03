set(DEPENDENCY_LIBRARIES
    coconut-milk-math-${MODULE_MATHS}
    coconut-milk-graphics-${MODULE_RENDERER}
    coconut-pulp-model
    coconut-tools-logger
    coconut-tools-configuration
    coconut-tools-system
    coconut-tools-exceptions
    )

set(TEST_LIBRARIES
  )

library_module(coconut-pulp-renderer "${TEST_LIBRARIES}" "${DEPENDENCY_LIBRARIES}" USE_GMOCK ${LIBRARY_TYPE})
