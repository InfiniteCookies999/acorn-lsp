# CMakeList.txt : CMake project for acorn-lsp, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (acorn-lsp "acorn-lsp.cpp" )

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET acorn-lsp PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
