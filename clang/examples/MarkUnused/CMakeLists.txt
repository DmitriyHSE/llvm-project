add_clang_library(MarkUnused
  MODULE
  MarkUnused.cpp
)

if(CLANG_BUILD_EXAMPLES)
  # Предполагаем, что lit и FileCheck доступны
  add_lit_testsuite(check-markunused "Running MarkUnused plugin tests"
    ${CMAKE_CURRENT_SOURCE_DIR}/test
    DEPENDS MarkUnused
  )
  set_target_properties(check-markunused PROPERTIES FOLDER "Tests")
endif()