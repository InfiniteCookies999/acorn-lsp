project ("acorn-transpiler")

add_executable (acorn-transpiler "main.cpp" "parser.h" "parser.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET acorn-transpiler PROPERTY CXX_STANDARD 20)
endif()
